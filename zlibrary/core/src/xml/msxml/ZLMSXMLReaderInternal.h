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

#ifndef __ZLMSXMLREADERINTERNAL_H__
#define __ZLMSXMLREADERINTERNAL_H__

#include <msxml6.h>
#include <string>

#include <shared_ptr.h>


class ZLXMLReader;
class ZLInputStream;

class W32ContentHandler;
class W32ErrorHandler;
class W32EntityResolver;
class AsyncStream;


class ZLMSXMLReaderInternal {

public:
	ZLMSXMLReaderInternal(ZLXMLReader &reader, const char *encoding);
	~ZLMSXMLReaderInternal();

private:
	HRESULT setEncoding(const char *encoding);

public:
	void init(const char *encoding);
	bool readDocument(shared_ptr<ZLInputStream> stream);

	bool readAsync();
	bool readFromBuffer(const char *data, size_t len);
	bool finishAsync();

private:
	ZLXMLReader &myReader;

	ISAXXMLReader *mySaxReader;
	W32ContentHandler *myContentHandler;
	W32ErrorHandler *myErrorHandler;
	W32EntityResolver *myEntityResolver;

	shared_ptr<ZLInputStream> myAsyncStream;
	HANDLE myAsyncThread;

friend DWORD WINAPI asyncThread(LPVOID);

private: // disable copying
	ZLMSXMLReaderInternal(const ZLMSXMLReaderInternal &);
	const ZLMSXMLReaderInternal &operator = (const ZLMSXMLReaderInternal &);
};

#endif /* __ZLMSXMLREADERINTERNAL_H__ */
