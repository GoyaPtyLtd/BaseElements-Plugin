/*
 BEPDF.h
 BaseElements Plug-In

 Copyright 2021~2022 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

*/

#ifndef BEPDF_h
#define BEPDF_h

#include <stdio.h>

#include <unordered_map>
#include <unordered_set>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <podofo/podofo.h>
#pragma GCC diagnostic pop


/*
 the code for BEPDF.h/BEPDF.cpp is essentailly the code given at
 
 https://stackoverflow.com/questions/55283985/copy-only-necessary-objects-from-pdf-file
 
*/


class BEPDFDocument : public PoDoFo::PdfMemDocument {
	
public:

	void InsertPages ( const PoDoFo::PdfMemDocument& rDoc, std::vector<long> pageNumbers );

};


class BEPDFObject : public PoDoFo::PdfObject {
	
public:

	BEPDFObject ( const PoDoFo::PdfReference & rRef, const PoDoFo::PdfVariant & rVariant ) : PoDoFo::PdfObject ( rRef, rVariant ){};
	void SetOwner ( PoDoFo::PdfIndirectObjectList* pVecObjects );
	PoDoFo::PdfMemoryObjectStream* GetInputStream();

};


class BEPDFPage : public PoDoFo::PdfPage {
	
public:

	std::unordered_set<PoDoFo::PdfObject *>* GetPageDependencies() const;

};



class BEPDFVectorObjects : public PoDoFo::PdfIndirectObjectList {
	
public:

	void GetObjectDependencies ( const PoDoFo::PdfObject* pObj, std::unordered_set<PoDoFo::PdfObject*> &refMap ) const;

};


void FixPageReferences ( PoDoFo::PdfIndirectObjectList& objects, PoDoFo::PdfObject* pObject, std::unordered_map<PoDoFo::uint32_t, PoDoFo::uint32_t>& oldNumToNewNum );



#endif /* BEPDF_hpp */
