/*
 BEAppleFunctionsCommon.cpp
 BaseElements Plug-In
 
 Copyright 2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#import "BEAppleFunctionsCommon.h"

#import <Foundation/Foundation.h>


#pragma mark -
#pragma mark Other (FMX)
#pragma mark -

const unsigned long Sub_OSXLoadString ( unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax, const std::string bundleId )
{
	unsigned long returnResult = 0;
	
	if( (intoHere != NULL) && (intoHereMax > 1) ) {
		// Turn stringID to a textual identifier, then get the string from the .strings file as a null-term unichar array.
		CFStringRef 	strIdStr = CFStringCreateWithFormat( kCFAllocatorDefault, NULL, CFSTR("%ld"), stringID );
		CFStringRef 	bundleIdStr = CFStringCreateWithCString ( NULL, bundleId.c_str(), (unsigned int)bundleId.size() );
		
		// Note: The plug-in must be explicit about the bundle and file it wants to pull the string from.
		CFStringRef 	osxStr = CFBundleCopyLocalizedString ( reinterpret_cast<CFBundleRef>(gFMX_ExternCallPtr->instanceID), strIdStr, strIdStr, bundleIdStr );
		
		if((osxStr != NULL) && (osxStr != strIdStr)) {
			
			long	osxStrLen = CFStringGetLength(osxStr);
			
			if( osxStrLen < (intoHereMax-1) ) {
				
				CFRange		allChars;
				allChars.location = 0;
				allChars.length = osxStrLen;
				
				CFStringGetCharacters(osxStr, allChars, (UniChar*)(intoHere));
				intoHere[osxStrLen] = 0x0000;
				returnResult = (unsigned long)osxStrLen;
				
			} // osxStrLen
			
		} // osxStr
		
		if ( osxStr ) { CFRelease( osxStr ); }
		CFRelease( strIdStr );
		CFRelease( bundleIdStr );
		
	} // intoHere
	
	return(returnResult);
	
} // Sub_OSXLoadString


