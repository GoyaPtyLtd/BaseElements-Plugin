/*
 ProgressDialogWindowController.h
 BaseElements Plug-In
 
 Copyright 2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#import <Cocoa/Cocoa.h>


@interface ProgressDialogWindowController : NSWindowController
{
	
	IBOutlet NSProgressIndicator *progressIndicator;
	IBOutlet NSTextField * description;
	IBOutlet NSButton * cancelButton;
	
	bool restart;
	bool userCancelled;
	
}

-(void) show: (NSString *) _title description: (NSString *) _description maximumValue: (double) max canCancel: (bool) allowCancel;
-(int) update: (double) newValue description: (NSString *) _description;

- (IBAction) cancel: (id) sender;

-(bool) shouldRestart;
-(void) closeWindow;

@end

