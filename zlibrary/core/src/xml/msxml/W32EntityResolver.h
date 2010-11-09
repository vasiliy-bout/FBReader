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

#ifndef __W32ENTITYRESOLVER_H__
#define __W32ENTITYRESOLVER_H__

#include <msxml2.h>


class ZLXMLReader;

class W32EntityResolver : public ISAXEntityResolver {

public:
	W32EntityResolver(ZLXMLReader &reader);
	virtual ~W32EntityResolver();

public:
	virtual HRESULT STDMETHODCALLTYPE resolveEntity(const wchar_t *pwchPublicId,
			const wchar_t *pwchSystemId, VARIANT *pvarInput);

private:
	ZLXMLReader &myReader;

	// Following code implements common IUnknown interface
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
private:
	volatile LONG myRefs;

private: // disable copying
	W32EntityResolver(const W32EntityResolver &);
};

#endif /* __W32ENTITYRESOLVER_H__ */
