/*
 BEAppleFunctionsCommon.cpp
 BaseElements Plug-In
 
 Copyright 2017-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#import "BEAppleFunctionsCommon.h"

#import <Foundation/Foundation.h>


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


