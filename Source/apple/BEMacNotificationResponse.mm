/*
 BEMacNotificationResponse.mm
 BaseElements Plug-In
 
 Copyright 2023 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

#import "BEMacNotificationResponse.h"


@implementation BENotificationResponse

-(void)userNotificationCenter: center didReceiveNotificationResponse: response withCompletionHandler: (void (^)(void))completionHandler {
		
	auto userInfo = [[[[response notification] request] content] userInfo];
	NSString * fmp_uri = [userInfo objectForKey: @"fmp_uri"];

	if ( [[response actionIdentifier] isEqual:@"com.apple.UNNotificationDefaultActionIdentifier"] && ( 0 != [fmp_uri length] ) ) {
		[[NSWorkspace sharedWorkspace] openURL: [NSURL URLWithString: fmp_uri ]];
	} // else if [[response actionIdentifier] isEqual:@"com.apple.UNNotificationDismissActionIdentifier"]
		
	// must call the completion handler to let the os know we're
	completionHandler();

} // userNotificationCenter


@end


