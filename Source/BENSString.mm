/*
 BENSString.mm
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#import "BEMacFunctions.h"
#import "BEPluginGlobalDefines.h"
#import "BENSString.h"



#if TARGET_RT_BIG_ENDIAN
	#define ENCODING kCFStringEncodingUTF32BE
#else
	#define ENCODING kCFStringEncodingUTF32LE
#endif


const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding ( ENCODING );


@implementation NSString ( std_string_conversions )


+(NSString *) NSStringFromStringAutoPtr: ( const StringAutoPtr ) text
{
	NSString * new_string = [NSString stringWithCString: text->c_str() encoding: NSUTF8StringEncoding];
	
	return new_string;
}


/*
 NSStringFromWStringAutoPtr & WStringAutoPtrFromNSString from code at

	http://www.cocoabuilder.com/archive/cocoa/200434-nsstring-from-wstring.html
*/

+(NSString *) NSStringFromWStringAutoPtr: ( const WStringAutoPtr ) text
{
	char * string_data = (char *)text->data();
	unsigned size = text->size() * sizeof ( wchar_t );
	
	NSString* new_string = [[[NSString alloc] initWithBytes: string_data
													 length: size
												   encoding: kEncoding_wchar_t] autorelease];
	return new_string;
}


-(WStringAutoPtr) WStringAutoPtrFromNSString
{
	NSData * string_data = [self dataUsingEncoding: kEncoding_wchar_t];
	unsigned size = [string_data length] / sizeof ( wchar_t );
	
	return WStringAutoPtr ( new wstring ( (wchar_t *)[string_data bytes], size ) );
}


@end


