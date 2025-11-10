/*
 BEPDF.cpp
 BaseElements Plug-In

 Copyright 2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

*/

#include "BEPDF.h"


void BEPDFDocument::InsertPages ( const PoDoFo::PdfMemDocument & rDoc, std::vector<long> pageNumbers )
{
	std::unordered_set<PoDoFo::PdfObject*> totalSet;
	std::vector<PoDoFo::pdf_objnum> oldObjNumPages;
	std::unordered_map<PoDoFo::pdf_objnum, PoDoFo::pdf_objnum> oldObjNumToNewObjNum;

	std::vector<PoDoFo::PdfObject*> newPageObjects;

	// Collect all dependencies from all pages that are to be copied
	for ( unsigned int i = 0; i < pageNumbers.size(); ++i ) {
		BEPDFPage* page = (BEPDFPage*)rDoc.GetPage ( (int)pageNumbers[i] );
		if (page) {
			oldObjNumPages.push_back(page->GetObject()->Reference().ObjectNumber());
			std::unordered_set<PoDoFo::PdfObject*> *set = page->GetPageDependencies();
			totalSet.insert(set->begin(), set->end());
			delete set;
		}
	}

	// Create a new page object for every copied page from the old document
	// Copy all objects the pages depend on to the new document
	for (auto it = totalSet.begin(); it != totalSet.end(); ++it) {
		unsigned int length = static_cast<unsigned int>(GetObjects().GetSize() + GetObjects().GetFreeObjects().size());
		PoDoFo::PdfReference ref(static_cast<unsigned int>(length+1), 0);
		BEPDFObject* pObj = new BEPDFObject(ref, *(*it));
		pObj->SetOwner(&(GetObjects()));
		if ((*it)->HasStream()) {
			PoDoFo::PdfStream *stream = (*it)->GetStream();
			PoDoFo::pdf_long pdf_length;
			char* buf;
			stream->GetCopy(&buf, &pdf_length);
			PoDoFo::PdfMemoryInputStream inputStream(buf, pdf_length);
			pObj->GetStream()->SetRawData(&inputStream, pdf_length);
			free(buf);

		}
		oldObjNumToNewObjNum.insert(std::pair<PoDoFo::pdf_objnum, PoDoFo::pdf_objnum>((*it)->Reference().ObjectNumber(), length+1));
		GetObjects().push_back((PoDoFo::PdfObject*)pObj);
		newPageObjects.push_back((PoDoFo::PdfObject*)pObj);
	}

	// In all copied objects, fix the object numbers so they are valid in the new document
	for (auto it = newPageObjects.begin(); it != newPageObjects.end(); ++it) {
		FixPageReferences(GetObjects(), *it, oldObjNumToNewObjNum);
	}

	// Insert the copied pages into the pages tree
	for (auto it = oldObjNumPages.begin(); it != oldObjNumPages.end(); ++it) {
		PoDoFo::PdfObject* pageObject = GetObjects().GetObject(PoDoFo::PdfReference(oldObjNumToNewObjNum[(*it)], 0));
		std::unique_ptr<PoDoFo::PdfPage> page ( new PoDoFo::PdfPage(pageObject, std::deque<PoDoFo::PdfObject*>()) );
		GetPagesTree()->InsertPage(GetPageCount() - 1, page.get());
	}

}


void BEPDFObject::SetOwner ( PoDoFo::PdfVecObjects* pVecObjects )
{
	if ( m_pOwner == pVecObjects )
	{
		// The inner owner for variant data objects is guaranteed to be same
		return;
	}

	m_pOwner = pVecObjects;
	if ( DelayedLoadDone() )
		SetVariantOwner( GetDataType() );
}


PoDoFo::PdfStream* BEPDFObject::GetStream()
{
	DelayedStreamLoad();
	return GetStream_NoDL();
}


std::unordered_set<PoDoFo::PdfObject *>* BEPDFPage::GetPageDependencies() const
{
	std::unordered_set<PoDoFo::PdfObject *> *set = new std::unordered_set<PoDoFo::PdfObject *>();

	const PoDoFo::PdfObject* pageObj = GetObject();
	if (pageObj) {
		BEPDFVectorObjects* objects = (BEPDFVectorObjects*)pageObj->GetOwner();
		if (objects) {
			set->insert((PoDoFo::PdfObject*)pageObj);
			objects->GetObjectDependencies(pageObj, *set);
		}
	}

	return set;
}

// Optimized version of PdfVecObjects::GetObjectDependencies
void BEPDFVectorObjects::GetObjectDependencies (const PoDoFo::PdfObject* pObj, std::unordered_set<PoDoFo::PdfObject*> &refMap ) const
{
	// Check objects referenced from this object
	if (pObj->IsReference())
	{
		PoDoFo::PdfObject* referencedObject = GetObject(pObj->GetReference());
		if (referencedObject != NULL && refMap.count(referencedObject) < 1) {
			(refMap).insert((PoDoFo::PdfObject *)referencedObject); // Insert referenced object
			GetObjectDependencies((const PoDoFo::PdfObject*)referencedObject, refMap);
		}
	}
	else {
		// Recursion
		if (pObj->IsArray())
		{
			PoDoFo::PdfArray::const_iterator itArray = pObj->GetArray().begin();
			while (itArray != pObj->GetArray().end())
			{
				GetObjectDependencies(&(*itArray), refMap);
				++itArray;
			}
		}
		else if (pObj->IsDictionary())
		{
			PoDoFo::TCIKeyMap itKeys = pObj->GetDictionary().GetKeys().begin();
			while (itKeys != pObj->GetDictionary().GetKeys().end())
			{
				if ((*itKeys).first != PoDoFo::PdfName("Parent")) {
					GetObjectDependencies((*itKeys).second, refMap);
				}
				++itKeys;
			}
		}
	}
}

void FixPageReferences ( PoDoFo::PdfVecObjects& objects, PoDoFo::PdfObject* pObject, std::unordered_map<PoDoFo::pdf_objnum, PoDoFo::pdf_objnum>& oldNumToNewNum ) {
	if( !pObject)
	{
		PODOFO_RAISE_ERROR( PoDoFo::ePdfError_InvalidHandle );
	}
	if( pObject->IsDictionary() )
	{
		PoDoFo::TKeyMap::iterator it = pObject->GetDictionary().GetKeys().begin();

		while( it != pObject->GetDictionary().GetKeys().end() )
		{
			if ((*it).first != PoDoFo::PdfName("Parent")) {
				FixPageReferences(objects, (*it).second, oldNumToNewNum);
			}
			++it;
		}
	}
	else if( pObject->IsArray() )
	{
		PoDoFo::PdfArray::iterator it = pObject->GetArray().begin();

		while( it != pObject->GetArray().end() )
		{
			FixPageReferences(objects, &(*it), oldNumToNewNum);
			++it;
		}
	}
	else if( pObject->IsReference() )
	{
		//PdfObject* referencedObj = objects.GetObject(pObject->GetReference());

		PoDoFo::pdf_objnum oldnum = pObject->GetReference().ObjectNumber();
		PoDoFo::pdf_objnum newnum = oldNumToNewNum[oldnum];

		if (!newnum) throw new std::runtime_error("No new object number for old object number");

		*pObject = PoDoFo::PdfReference(newnum, 0);

	}
}
