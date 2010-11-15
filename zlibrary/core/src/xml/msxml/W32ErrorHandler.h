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

#ifndef __W32ERRORHANDLER_H__
#define __W32ERRORHANDLER_H__

#include <msxml6.h>

class W32ErrorHandler : public ISAXErrorHandler {

public:
	W32ErrorHandler();
	virtual ~W32ErrorHandler();

public:
	// from ISAXErrorHandler
	virtual HRESULT STDMETHODCALLTYPE error(ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode);
	virtual HRESULT STDMETHODCALLTYPE fatalError(ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode);
	virtual HRESULT STDMETHODCALLTYPE ignorableWarning(ISAXLocator *pLocator, const wchar_t *pwchErrorMessage, HRESULT hrErrorCode);


	// Following code implements common IUnknown interface
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
private:
	volatile LONG myRefs;

private: // disable copying
	W32ErrorHandler(const W32ErrorHandler &);
	const W32ErrorHandler &operator = (const W32ErrorHandler &);
};

#endif /* __W32ERRORHANDLER_H__ */
