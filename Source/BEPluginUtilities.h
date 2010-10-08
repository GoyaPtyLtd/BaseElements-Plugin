/*
 BEPluginUtilities.h
 BaseElements Plug-In
	
 Copyright 2010 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#if !defined(BEPLUGINUTILITIES_H)
	#define BEPLUGINUTILITIES_H

#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXText.h"
#include "FMWrapper/FMXCalcEngine.h"


using namespace std;
using namespace fmx;


typedef auto_ptr<string> StringAutoPtr;
typedef auto_ptr<wstring> WStringAutoPtr;


errcode TextConstantFunction ( StringAutoPtr text, Data& results );
errcode TextConstantFunction ( string text, Data& results );

void SetNumericResult ( long number, Data& results );
void SetUTF8Result ( StringAutoPtr text, Data& results );

StringAutoPtr ParameterAsUTF8String ( const DataVect& data_vect, unsigned long which );
WStringAutoPtr ParameterAsUnicodeString ( const DataVect& data_vect, unsigned long which );
StringAutoPtr ReadFileAsUTF8 ( StringAutoPtr path );
StringAutoPtr ConvertToUTF8 ( char * in, size_t length );


// extract localised strings from the plug-in resources

void Do_GetString ( unsigned long whichString, FMX_ULong winLangID, FMX_Long resultsize, FMX_Unichar* string );
void Do_GetString ( unsigned long whichString,  TextAutoPtr& intoHere, bool stripFunctionParams = false );

// used by Do_GetString
#if defined(FMX_MAC_TARGET)
	unsigned long  Sub_OSXLoadString ( unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax );
#endif


#endif // BEPLUGINUTILITIES_H
