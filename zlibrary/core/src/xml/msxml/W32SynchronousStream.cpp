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

#include "W32SynchronousStream.h"

#include <ZLInputStream.h>


W32SynchronousStream::W32SynchronousStream(shared_ptr<ZLInputStream> stream, bool closeStream) : myStream(stream), myCloseStream(closeStream), myRefs(1) {
}

W32SynchronousStream::~W32SynchronousStream() {
	if (myCloseStream && !myStream.isNull()) {
		myStream->close();
	}
}


HRESULT STDMETHODCALLTYPE W32SynchronousStream::Read(void *pv, ULONG cb, ULONG *pcbRead) {
	if (pcbRead == 0 || pv == 0) {
		return STG_E_INVALIDPOINTER;
	}
	if (myStream.isNull()) {
		*pcbRead = 0L;
		return S_FALSE;
	}
	const size_t length = myStream->read((char*)pv, cb);
	*pcbRead = length;
	return (length == cb) ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE W32SynchronousStream::Write(const void *pv, ULONG cb, ULONG *pcbWritten) {
	return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE W32SynchronousStream::QueryInterface(REFIID riid, void **ppvObject) {
	if (ppvObject == 0) {
		return E_POINTER;
	}
	if (riid == __uuidof(IUnknown) || riid == __uuidof(ISequentialStream)) {
		*ppvObject = this;
		AddRef();
		return S_OK;
	}
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE W32SynchronousStream::AddRef() {
	return (ULONG)InterlockedIncrement(&myRefs);
}

ULONG STDMETHODCALLTYPE W32SynchronousStream::Release() {
	ULONG res = (ULONG) InterlockedDecrement(&myRefs);
	if (res == 0) {
		delete this;
	}
	return res;
}
