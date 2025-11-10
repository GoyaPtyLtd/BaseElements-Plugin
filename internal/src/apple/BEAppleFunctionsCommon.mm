/*
 BEAppleFunctionsCommon.cpp
 BaseElements Plug-In
 
 Copyright 2017-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#import "BEAppleFunctionsCommon.h"


#if TARGET_RT_BIG_ENDIAN
	#define ENCODING kCFStringEncodingUTF32BE
#else
	#define ENCODING kCFStringEncodingUTF32LE
#endif

const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding ( ENCODING );


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
	
	return std::wstring ( (wchar_t *)[string_data bytes], size );
}


const unsigned long Sub_LoadString ( unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax, const std::string /* bundleId */ )
{
	unsigned long returnResult = 0;
	
	if( ( intoHere != NULL ) && ( intoHereMax > 1 ) ) {

		NSBundle *thisBundle = [NSBundle bundleWithIdentifier: @BE_PLUGIN_BUNDLE_IDENTIFIER];
		NSString *message = NULL;

		if ( kFMXT_AppConfigStr == stringID || PLUGIN_DESCRIPTION_STRING_ID == stringID ) {
			
			NSString *messageFormat = [thisBundle localizedStringForKey: @PLUGIN_DESCRIPTION_STRING_ID_STRING value: @"Version: %@\n\nThis plug-in provides additional functionality for BaseElements from Goya." table: nil ];
			message = [NSString stringWithFormat: messageFormat, @VERSION_STRING];
			
		} else {
			
			NSString * which_string = [NSString stringWithFormat:@"%lu", stringID];
			message = [thisBundle localizedStringForKey: which_string value: @"" table: nil];

		}
		
		[message getCharacters: (unichar*)intoHere range: {0, [message length]}];
		intoHere[ [message length] ] = '\0';

	} // intoHere
	
	return returnResult;
	
} // Sub_LoadString


#pragma mark -
#pragma mark User Preferences
#pragma mark -


const bool SetPreference ( std::string& key, std::string& value, std::string& domain )
{
	NSUserDefaults *standardUserDefaults = [NSUserDefaults standardUserDefaults];
	
	bool result = true;
	
	if ( standardUserDefaults ) {
		
		NSString * domain_name = NSStringFromString ( domain );
		NSDictionary * preferences = [standardUserDefaults persistentDomainForName: domain_name];
		
		NSString * preference_key = NSStringFromString ( key );
		NSString * preference_value = NSStringFromString ( value );
		
		NSMutableDictionary * new_preferences = [NSMutableDictionary dictionaryWithCapacity: [preferences count] + 1];
		[new_preferences addEntriesFromDictionary: preferences];
		[new_preferences setObject: preference_value forKey: preference_key];
		
		[standardUserDefaults setPersistentDomain: new_preferences forName: domain_name];
		[standardUserDefaults synchronize];
		
	} else {
		result = false;
	}
	
	return result;
}


const std::string GetPreference ( std::string& key, std::string& domain )
{
	NSUserDefaults *standardUserDefaults = [NSUserDefaults standardUserDefaults];
	NSString * preference_value = @"";
	
	if ( standardUserDefaults ) {
		
		[standardUserDefaults synchronize];
		
		NSString * domain_name = NSStringFromString ( domain );
		NSDictionary * preferences = [standardUserDefaults persistentDomainForName: domain_name];
		
		NSString * preference_key = NSStringFromString ( key );
		if ( [preferences objectForKey: preference_key] != nil ) {
			preference_value = [preferences objectForKey: preference_key];
		}
	}
	
	std::string preference ( [preference_value UTF8String] );
	
	return preference;
}


void DeletePreference ( std::string& key, std::string& domain )
{
	NSUserDefaults *standardUserDefaults = [NSUserDefaults standardUserDefaults];
	
	if ( standardUserDefaults ) {
		
		[standardUserDefaults synchronize];
		
		NSString * domain_name = NSStringFromString ( domain );
		NSDictionary * preferences = [standardUserDefaults persistentDomainForName: domain_name];
		
		NSMutableDictionary * new_preferences = [NSMutableDictionary dictionaryWithCapacity: [preferences count]];
		[new_preferences addEntriesFromDictionary: preferences];
		
		NSString * preference_key = NSStringFromString ( key );
		[new_preferences removeObjectForKey: preference_key];
		
		[standardUserDefaults setPersistentDomain: new_preferences forName: domain_name];
		[standardUserDefaults synchronize];
	}
	
}

