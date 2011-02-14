/*
 BENSString.h
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BENSSTRING_H)
#define BENSSTRING_H

#import <Cocoa/Cocoa.h>

#import "BEMacFunctions.h"
#import "BEPluginGlobalDefines.h"
#import "BEPluginUtilities.h"


#include <string>


using namespace std;


@interface NSString ( std_string_conversions )

+(NSString *) NSStringFromStringAutoPtr: ( const StringAutoPtr ) text;
+(NSString *) NSStringFromWStringAutoPtr: ( const WStringAutoPtr ) text;

-(WStringAutoPtr) WStringAutoPtrFromNSString;

@end


#endif // BENSSTRING_H
