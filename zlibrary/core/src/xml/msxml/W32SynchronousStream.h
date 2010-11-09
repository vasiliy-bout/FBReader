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

#ifndef __W32SYNCHRONOUSSTREAM_H__
#define __W32SYNCHRONOUSSTREAM_H__

#include <msxml2.h>

#include <shared_ptr.h>


class ZLInputStream;

class W32SynchronousStream : public ISequentialStream {

public:
	W32SynchronousStream(shared_ptr<ZLInputStream> stream);
	virtual ~W32SynchronousStream();

public:
	// from ISequentialStream
	virtual HRESULT STDMETHODCALLTYPE Read(void *pv, ULONG cb, ULONG *pcbRead);
	virtual HRESULT STDMETHODCALLTYPE Write(const void *pv, ULONG cb, ULONG *pcbWritten);

private:
	shared_ptr<ZLInputStream> myStream;

	// Following code implements common IUnknown interface
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
private:
	volatile LONG myRefs;

private: // disable copying
	W32SynchronousStream(const W32SynchronousStream &);
};

#endif /* __W32SYNCHRONOUSSTREAM_H__ */
