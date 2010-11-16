/*
 * Copyright (C) 2010 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "ZLMSXMLReaderInternal.h"

#include <ZLInputStream.h>

#include "../ZLXMLReader.h"

#include "W32ContentHandler.h"
#include "W32ErrorHandler.h"
#include "W32SynchronousStream.h"
#include "W32EntityResolver.h"



class AsyncStream : public ZLInputStream {

public:
	AsyncStream();
	~AsyncStream();

public:
	bool open();
	size_t read(char *buffer, size_t maxSize);
	void close();
	void seek(int offset, bool absoluteOffset);
	size_t offset() const;
	size_t sizeOfOpened();

	void putData(const char *data, size_t size);

private:
	size_t fillBuffer(char *buffer, size_t maxSize);

private:
	HANDLE myDataEvent;
	HANDLE myDataMutex;

	char * volatile myBuffer;
	volatile size_t myBufferSize;
	volatile size_t myDataSize;
	volatile bool myEof;
};

AsyncStream::AsyncStream() : myBuffer(0), myBufferSize(0), myDataSize(0), myEof(false) {
	myDataEvent = CreateEventA(0, FALSE, FALSE, 0);
	myDataMutex = CreateMutexA(0, FALSE, 0);
}

AsyncStream::~AsyncStream() {
	if (myBuffer != 0) {
		delete[] myBuffer;
	}
	if (myDataEvent != 0) {
		CloseHandle(myDataEvent);
	}
	if (myDataMutex != 0) {
		CloseHandle(myDataMutex);
	}
}

bool AsyncStream::open() {
	if (myDataEvent == 0 || myDataMutex == 0) {
		return false;
	}
	return true;
}

size_t AsyncStream::read(char *buffer, size_t maxSize) {
	const HANDLE handles[2] = {myDataEvent, myDataMutex};
	size_t written;
	bool eof;

	WaitForSingleObject(myDataMutex, INFINITE);
	written = fillBuffer(buffer, maxSize);
	eof = myEof;
	ReleaseMutex(myDataMutex);

	while (written == 0 && !eof) {
		WaitForMultipleObjects(2, handles, TRUE, INFINITE);
		written = fillBuffer(buffer, maxSize);
		eof = myEof;
		ReleaseMutex(myDataMutex);
	}
	return written;
}

void AsyncStream::close() {
	WaitForSingleObject(myDataMutex, INFINITE);
	myEof = true;
	SetEvent(myDataEvent);
	ReleaseMutex(myDataMutex);
}

void AsyncStream::seek(int, bool) {
}

size_t AsyncStream::offset() const {
	return 0;
}

size_t AsyncStream::sizeOfOpened() {
	return 0;
}

size_t AsyncStream::fillBuffer(char *buffer, size_t maxSize) {
	if (myBuffer == 0 || myBufferSize == 0 || myDataSize == 0) {
		return 0;
	}
	const size_t toWrite = (maxSize > myDataSize) ? myDataSize : maxSize;
	memcpy(buffer, myBuffer, toWrite);
	myDataSize -= toWrite;
	if (myDataSize != 0) {
		memmove(myBuffer, myBuffer + toWrite, myDataSize);
	}
	return toWrite;
}

void AsyncStream::putData(const char *data, size_t size) {
	if (data == 0 || size == 0) {
		return;
	}
	WaitForSingleObject(myDataMutex, INFINITE);

	const size_t fullSize = myDataSize + size;

	if (myBuffer == 0) {
		myBuffer = new char[size];
		myBufferSize = size;
	} else if (myBufferSize < fullSize) {
		char *oldBuffer = myBuffer;
		myBuffer = new char[fullSize];
		myBufferSize = fullSize;
		memcpy(myBuffer, oldBuffer, myDataSize);
		delete[] oldBuffer;
	}

	memcpy(myBuffer + myDataSize, data, size);
	myDataSize = fullSize;

	SetEvent(myDataEvent);
	ReleaseMutex(myDataMutex);
}


static DWORD WINAPI asyncThread(LPVOID parameter) {
	ZLMSXMLReaderInternal *reader = (ZLMSXMLReaderInternal *) parameter;

	VARIANT input;
	VariantInit(&input);
	V_VT(&input) = VT_UNKNOWN;
	V_UNKNOWN(&input) = new W32SynchronousStream(reader->myAsyncStream);

	HRESULT res = reader->mySaxReader->parse(input);
	VariantClear(&input);
	return res;
}


bool ZLMSXMLReaderInternal::readAsync() {
	if (mySaxReader == 0) {
		return false;
	}

	myAsyncStream = new AsyncStream();
	if (!myAsyncStream->open()) {
		myAsyncStream.reset();
		return false;
	}

	myAsyncThread = CreateThread(0, 0, &asyncThread, this, CREATE_SUSPENDED, 0);
	if (myAsyncThread == 0) {
		return false;
	}
	ResumeThread(myAsyncThread);
	return true;
}

bool ZLMSXMLReaderInternal::readFromBuffer(const char *data, size_t len) {
	if (myAsyncStream.isNull() || myAsyncThread == 0) {
		return false;
	}
	((AsyncStream &)*myAsyncStream).putData(data, len);
	return true;
}

bool ZLMSXMLReaderInternal::finishAsync() {
	if (myAsyncStream.isNull() || myAsyncThread == 0) {
		return false;
	}
	((AsyncStream &)*myAsyncStream).close();
	WaitForSingleObject(myAsyncThread, INFINITE);

	HRESULT res;
	DWORD code;
	if (!GetExitCodeThread(myAsyncThread, &code)) {
		res = E_FAIL;
	} else {
		res = code;
	}
	CloseHandle(myAsyncThread);
	myAsyncThread = 0;

	if (res == E_ABORT && myReader.isInterrupted()) {
		res = S_OK;
	}
	return SUCCEEDED(res);
}
