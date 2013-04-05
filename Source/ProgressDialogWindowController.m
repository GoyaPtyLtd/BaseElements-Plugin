/*
 ProgressDialogWindowController.m
 BaseElements Plug-In
 
 Copyright 2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#import "ProgressDialogWindowController.h"

#include "BEPluginGlobalDefines.h"


@implementation ProgressDialogWindowController

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if ( self ) {
		restart = NO;
		userCancelled = NO;
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
	
	[self.window center];
	[self.window makeKeyAndOrderFront: self];
}


-(bool) shouldRestart
{
	return restart;
}


-(void) closeWindow
{
	[self.window close];
	restart = YES;
}


-(void) show: (NSString *) _title description: (NSString *) _description maximumValue: (double) max canCancel: (bool) allowCancel
{
	
	[self.window setTitle: _title];
	[description setStringValue: _description];

	if ( 0 == max ) {
		
		[progressIndicator setIndeterminate: YES];
		[progressIndicator startAnimation: self];

	}
	
	[progressIndicator setMinValue: 0];
	[progressIndicator setMaxValue: max];
	
	[cancelButton setEnabled: allowCancel];
	[progressIndicator displayIfNeeded];

	[self.window update];
	
}

-(void) _update: ( NSString *) _description
{
	
	if ( _description ) {
		[description setStringValue: _description];
	}
	
	[progressIndicator displayIfNeeded];
	[self.window update];

}


-(int) update: (double) newValue description: (NSString *) _description
{
	int error = kNoError;
	
	if ( userCancelled ) {
		return kUserCancelledError;
	}
	
	
	if ( newValue <= [progressIndicator maxValue] ) {
		
		if ( ![progressIndicator isIndeterminate] ) {
			[progressIndicator setDoubleValue: newValue];
		}
		
		[self _update: _description];

	} else {
		
		[self closeWindow];
	
	}
	
	return error;
}



- (IBAction) cancel: (id) sender
{
#pragma unused ( sender )

	if ( [[cancelButton title] isEqualToString: @"Cancel"] ) {
		userCancelled = YES;
	}
	[self closeWindow];
}

@end

