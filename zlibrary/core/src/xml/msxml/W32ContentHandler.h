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

#ifndef __W32CONTENTHANDLER_H__
#define __W32CONTENTHANDLER_H__

#include <msxml6.h>

#include <vector>


class ZLXMLReader;


class W32ContentHandler : public ISAXContentHandler {

public:
	W32ContentHandler(ZLXMLReader &reader);
	virtual ~W32ContentHandler();

public:
	// from ISAXContentHandler
	virtual HRESULT STDMETHODCALLTYPE characters(const wchar_t *pwchChars, int cchChars);
	virtual HRESULT STDMETHODCALLTYPE startElement(const wchar_t *pwchNamespaceUri, int cchNamespaceUri,
			const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName,
			ISAXAttributes *pAttributes);
	virtual HRESULT STDMETHODCALLTYPE endElement(const wchar_t *pwchNamespaceUri, int cchNamespaceUri,
			const wchar_t *pwchLocalName, int cchLocalName, const wchar_t *pwchQName, int cchQName);

	virtual HRESULT STDMETHODCALLTYPE skippedEntity(const wchar_t *pwchName, int cchName);

	virtual HRESULT STDMETHODCALLTYPE startDocument();
	virtual HRESULT STDMETHODCALLTYPE endDocument();
	virtual HRESULT STDMETHODCALLTYPE startPrefixMapping(const wchar_t *pwchPrefix, int cchPrefix,
			const wchar_t *pwchUri, int cchUri);
	virtual HRESULT STDMETHODCALLTYPE endPrefixMapping(const wchar_t *pwchPrefix, int cchPrefix);
	virtual HRESULT STDMETHODCALLTYPE ignorableWhitespace(const wchar_t *pwchChars, int cchChars);
	virtual HRESULT STDMETHODCALLTYPE processingInstruction(const wchar_t *pwchTarget, int cchTarget,
			const wchar_t *pwchData, int cchData);
	virtual HRESULT STDMETHODCALLTYPE putDocumentLocator(ISAXLocator *pLocator);

private:
	HRESULT decodeAttributes(ISAXAttributes *pAttributes);
	int setAttributeString(int index, const wchar_t *value, int len);
	void terminateAttributes(int index);

private:
	ZLXMLReader &myReader;
	char *myBuffer;
	unsigned myBufferSize;

	std::vector<char*> myAttributes;
	std::vector<unsigned> myAttributesSizes;

	// Following code implements common IUnknown interface
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
private:
	volatile LONG myRefs;

private: // disable copying
	W32ContentHandler(const W32ContentHandler &);
	const W32ContentHandler &operator = (const W32ContentHandler &);
};

#endif /* __W32CONTENTHANDLER_H__ */
