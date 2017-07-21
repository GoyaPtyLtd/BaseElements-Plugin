/*
 BEIOSFunctions.cpp
 BaseElements Plug-In
 
 Copyright 2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#import "BEPluginGlobalDefines.h"

#import "BEIOSFunctions.h"
#import "BEPluginUtilities.h"

#import <Foundation/Foundation.h>


#if TARGET_RT_BIG_ENDIAN
	#define ENCODING kCFStringEncodingUTF32BE
#else
	#define ENCODING kCFStringEncodingUTF32LE
#endif


using namespace std;


const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding ( ENCODING );


NSString * NSStringFromString ( const std::string& text );
NSString * NSStringFromWString ( const std::wstring& text );
std::wstring WStringFromNSString ( const NSString * text );


const std::wstring SelectFileOrFolder ( const std::wstring& prompt, const std::wstring& in_folder, bool choose_file );


void InitialiseForPlatform ( )
{
//	progressDialog = nil;
}


#pragma mark -
#pragma mark String Utilities
#pragma mark -

NSString * NSStringFromString ( const std::string& text )
{
	NSString * new_string = [NSString stringWithCString: text.c_str() encoding: NSUTF8StringEncoding];
	
	return new_string;
}


/*
 NSStringFromWStringAutoPtr & WStringAutoPtrFromNSString from code at
 
 http://www.cocoabuilder.com/archive/cocoa/200434-nsstring-from-wstring.html
 */

NSString * NSStringFromWString ( const std::wstring& text )
{
	char * string_data = (char *)text.data();
	unsigned long size = text.size() * sizeof ( wchar_t );
	
	NSString* new_string = [[NSString alloc] initWithBytes: string_data length: size encoding: kEncoding_wchar_t];

//	return [new_string autorelease];
	return new_string;
}


std::wstring WStringFromNSString ( const NSString * text )
{
	NSData * string_data = [text dataUsingEncoding: kEncoding_wchar_t];
	size_t size = [string_data length] / sizeof ( wchar_t );
	
	return wstring ( (wchar_t *)[string_data bytes], size );
}


#pragma mark -
#pragma mark Clipboard
#pragma mark -

const std::wstring ClipboardFormats ( void )
{

	return L"";
	
} // ClipboardFormats


const std::string ClipboardData ( std::wstring& /* atype */ )
{
	
	return "";
	
} // ClipboardData


const bool SetClipboardData ( std::string& /* data */, std::wstring& /* atype */ )
{
	
	return L"";
	
} // Set_ClipboardData


#pragma mark -
#pragma mark Dialogs
#pragma mark -

const std::wstring SelectFileOrFolder ( const std::wstring& /* prompt */, const std::wstring& /* in_folder */, bool /* choose_file */ )
{
	
	return L"";
	
} // SelectFileOrFolder


const std::wstring SelectFile ( const std::wstring& prompt, const std::wstring& in_folder )
{
	return SelectFileOrFolder ( prompt, in_folder, YES );
}


const std::wstring SelectFolder ( const std::wstring& prompt, const std::wstring& in_folder )
{
	return SelectFileOrFolder ( prompt, in_folder, NO );
}


const std::wstring SaveFileDialog ( std::wstring& /* prompt */, std::wstring& /* fileName */, std::wstring& /* inFolder */ )
{
	
	return L"";
	
} // SaveFileDialog




const int DisplayDialog ( std::wstring& /* title */, std::wstring& /* message */, std::wstring& /* ok_button */, std::wstring& /* cancel_button */, std::wstring& /* alternate_button */ )
{

	return 0;

} // DisplayDialog



#pragma mark -
#pragma mark Progress Dialog
#pragma mark -


const fmx::errcode DisplayProgressDialog ( const std::wstring& /* title */, const std::wstring& /* description*/, const long /* maximum */, const bool /* can_cancel */ )
{
	fmx::errcode error = kNoError;
	return error;
	
}


const fmx::errcode UpdateProgressDialog ( const long /* value */, const std::wstring& /* description */ )
{
	fmx::errcode error = kNoError;
	return error;
}



#pragma mark -
#pragma mark User Preferences
#pragma mark -


const bool SetPreference ( std::wstring& /* key */, std::wstring& /* value */, std::wstring& /* domain */ )
{

	return false;
	
}


const std::wstring GetPreference ( std::wstring& /* key */, std::wstring& /* domain */ )
{

	return L"";
	
}



#pragma mark -
#pragma mark Other
#pragma mark -


const bool OpenURL ( std::wstring& /* url */ )
{	
	return false;
}


const bool OpenFile ( std::wstring& /* path */ )
{	
	return false;
}


const std::wstring get_machine_name ( )
{
	return L"";
}


