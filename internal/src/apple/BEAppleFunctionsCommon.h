/*
 BEAppleFunctionsCommon.h
 BaseElements Plug-In
 
 Copyright 2017-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEAppleFunctionsCommon_h)
	#define BEAppleFunctionsCommon_h



#include "BEPluginGlobalDefines.h"

#include <string>


#ifdef __OBJC__

#ifdef FMX_IOS_TARGET
#import <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif

#if TARGET_RT_BIG_ENDIAN
	#define ENCODING kCFStringEncodingUTF32BE
#else
	#define ENCODING kCFStringEncodingUTF32LE
#endif


NSString * NSStringFromString ( const std::string& text );
NSString * NSStringFromWString ( const std::wstring& text );
std::wstring WStringFromNSString ( const NSString * text );


#endif


// used by Do_GetString

const unsigned long  Sub_LoadString ( unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax, const std::string bundleId = "" );

// user preferences

const bool SetPreference ( std::string& key, std::string& value, std::string& domain );
const std::string GetPreference ( std::string& key, std::string& domain );
void DeletePreference ( std::string& key, std::string& domain );


#endif // BEAppleFunctionsCommon_h

