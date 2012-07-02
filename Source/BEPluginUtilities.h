/*
 BEPluginUtilities.h
 BaseElements Plug-In
	
 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#if !defined(BEPLUGINUTILITIES_H)
	#define BEPLUGINUTILITIES_H

#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXText.h"
#include "FMWrapper/FMXCalcEngine.h"

#include "BEPluginGlobalDefines.h"


#include <vector>

using namespace std;
using namespace fmx;



typedef auto_ptr<string> StringAutoPtr;
typedef auto_ptr<wstring> WStringAutoPtr;


errcode TextConstantFunction ( wstring text, Data& results );
errcode TextConstantFunction ( WStringAutoPtr text, Data& results );

void SetResult ( const long number, Data& results );
void SetResult ( const Text& text, Data& results );
void SetResult ( const StringAutoPtr text, Data& results );
void SetResult ( const WStringAutoPtr text, Data& results );
void SetResult ( vector<char> data, Data& results );
void SetResult ( const string filename, const vector<char> data, Data& results );

bool ParameterAsBoolean ( const DataVect& parameters, const unsigned long which, const bool default_value = true );
long ParameterAsLong ( const DataVect& parameters, const unsigned long which, const unsigned long default_value );
StringAutoPtr ParameterAsUTF8String ( const DataVect& parameters, unsigned long which );
WStringAutoPtr ParameterAsWideString ( const DataVect& parameters, unsigned long which );

StringAutoPtr ReadFileAsUTF8 ( WStringAutoPtr path );
vector<char> ConvertTextTo ( char * in, const size_t length, const string& encoding );
StringAutoPtr ConvertTextTo ( StringAutoPtr in, const string& encoding );
StringAutoPtr ConvertTextToUTF8 ( char * in, const size_t length );

void SetTextEncoding ( const string& encoding );

errcode NoError ( void );
errcode MapError ( const errcode error = kNoError, const bool map = false );


// extract localised strings from the plug-in resources

void Do_GetString ( unsigned long whichString, FMX_ULong winLangID, FMX_Long resultsize, FMX_Unichar* string );
void Do_GetString ( unsigned long whichString,  TextAutoPtr& intoHere, bool stripFunctionParams = false );

// used by Do_GetString
#if defined(FMX_MAC_TARGET)
	unsigned long  Sub_OSXLoadString ( unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax );
#endif


#endif // BEPLUGINUTILITIES_H
