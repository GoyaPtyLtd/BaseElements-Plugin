/*
 BEWStringVector.h
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEWSTRINGVECTOR_H)
	#define BEWSTRINGVECTOR_H

#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXCalcEngine.h"
#include "FMWrapper/FMXText.h"

#include <vector>


using namespace std;
using namespace fmx;


// A container that preserves instertion order but disallows duplicate entries.

class BEWStringVector {
	
	vector<wstring> contents;
	
public:
	
	// add an item to the end of the vector, only unique items are added
	void PushBack ( wstring );
	
	// compile a FileMaker "value list" of the contents
	TextAutoPtr AsValueList();
	
};

#endif // BEWSTRINGVECTOR_H
