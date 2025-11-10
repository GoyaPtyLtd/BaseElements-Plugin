/*
 BEMacNotificationResponse.h
 BaseElements Plug-In
 
 Copyright 2023 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef BEMacNotificationResponse_h
#define BEMacNotificationResponse_h

#import <UserNotifications/UserNotifications.h>


@interface BENotificationResponse<UNUserNotificationCenterDelegate> : NSObject
 
-(void)userNotificationCenter: center didReceiveNotificationResponse: response withCompletionHandler: (void (^)(void))completionHandler;

@end


#endif /* BEMacNotificationResponse_h */
