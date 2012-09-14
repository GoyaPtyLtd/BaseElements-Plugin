/*
 BEPluginUtilities.h
 BaseElements Plug-In
	
 Copyright 2010-2012 Goya. All rights reserved.
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
#include <stdint.h>


using namespace std;


typedef auto_ptr<string> StringAutoPtr;
typedef auto_ptr<wstring> WStringAutoPtr;


fmx::errcode TextConstantFunction ( wstring text, fmx::Data& results );
fmx::errcode TextConstantFunction ( WStringAutoPtr text, fmx::Data& results );

void SetResult ( const uintmax_t number, fmx::Data& results );
void SetResult ( const fmx::Text& text, fmx::Data& results );
void SetResult ( const StringAutoPtr text, fmx::Data& results );
void SetResult ( const WStringAutoPtr text, fmx::Data& results );
void SetResult ( vector<char> data, fmx::Data& results );
void SetResult ( const string filename, const vector<char> data, fmx::Data& results );

bool ParameterAsBoolean ( const fmx::DataVect& parameters, const FMX_UInt32 which, const bool default_value = true );
long ParameterAsLong ( const fmx::DataVect& parameters, const FMX_UInt32 which, const unsigned long default_value );
StringAutoPtr ParameterAsUTF8String ( const fmx::DataVect& parameters, FMX_UInt32 which );
WStringAutoPtr ParameterAsWideString ( const fmx::DataVect& parameters, FMX_UInt32 which );

StringAutoPtr ReadFileAsUTF8 ( WStringAutoPtr path );
vector<char> ConvertTextTo ( char * in, const size_t length, const string& encoding );
StringAutoPtr ConvertTextTo ( StringAutoPtr in, const string& encoding );
StringAutoPtr ConvertTextToUTF8 ( char * in, const size_t length );

void SetTextEncoding ( const string& encoding );

fmx::errcode NoError ( void );
fmx::errcode MapError ( const fmx::errcode error = kNoError, const bool map = false );


// extract localised strings from the plug-in resources

void Do_GetString ( unsigned long whichString, FMX_PtrType winLangID, FMX_PtrType resultsize, FMX_Unichar* string );
void Do_GetString ( unsigned long whichString,  fmx::TextAutoPtr& intoHere, bool stripFunctionParams = false );

// used by Do_GetString
#if defined(FMX_MAC_TARGET)
	unsigned long  Sub_OSXLoadString ( unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax );
#endif


#endif // BEPLUGINUTILITIES_H
