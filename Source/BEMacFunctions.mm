/*
 BEMacFunctions.cpp
 BaseElements Plug-In
 
 Copyright 2010 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#import <Cocoa/Cocoa.h>

#import "BEMacFunctions.h"
#import "BEPluginGlobalDefines.h"

NSString * NSStringFromStringAutoPtr ( StringAutoPtr text );
StringAutoPtr SelectFileOrFolder ( StringAutoPtr prompt, bool choose_file = YES );


#pragma mark -
#pragma mark Mac Utilities
#pragma mark -

NSString * NSStringFromStringAutoPtr ( StringAutoPtr text )
{
	NSString * new_string = [NSString stringWithCString: text->c_str() encoding: NSUTF8StringEncoding];

	return new_string;
}


#pragma mark -
#pragma mark Clipboard
#pragma mark -

StringAutoPtr ClipboardFormats ( void )
{
	NSArray *types = [[[NSPasteboard generalPasteboard] types] copy];
	NSMutableString *formats = [NSMutableString stringWithCapacity: 1];
	
	for ( NSString *type in types ) {
		[formats appendString: type];
		[formats appendString: @"\r"];
	}

	[types release];

	return StringAutoPtr ( new string ( [formats cStringUsingEncoding: NSUTF8StringEncoding] ) );

} // ClipboardFormats


StringAutoPtr ClipboardData ( StringAutoPtr atype )
{
	NSString * pasteboard_type = NSStringFromStringAutoPtr ( atype );
	NSData * pasteboard_data = [[[NSPasteboard generalPasteboard] dataForType: pasteboard_type] copy];				
	NSString * clipboard_data = [[NSString alloc] initWithData: pasteboard_data encoding: NSUTF8StringEncoding];

	[pasteboard_data release];
	
	return StringAutoPtr ( new string ( [clipboard_data cStringUsingEncoding: NSUTF8StringEncoding] ) );
	
} // ClipboardData


bool SetClipboardData ( StringAutoPtr data, StringAutoPtr atype )
{
	NSString * data_to_copy = NSStringFromStringAutoPtr ( data );
	NSString * data_type = NSStringFromStringAutoPtr ( atype );
	NSArray * new_types = [NSArray arrayWithObject: data_type];

	[[NSPasteboard generalPasteboard] declareTypes: new_types owner: nil];

	return [[NSPasteboard generalPasteboard] setString: data_to_copy forType: data_type];

} // Set_ClipboardData


#pragma mark -
#pragma mark Dialogs
#pragma mark -

StringAutoPtr SelectFileOrFolder ( StringAutoPtr prompt, bool choose_file )
{
	
	NSOpenPanel* file_dialog = [NSOpenPanel openPanel];
	[file_dialog setTitle: NSStringFromStringAutoPtr ( prompt )];
	[file_dialog setCanChooseFiles: choose_file];
	[file_dialog setCanChooseDirectories: !choose_file];
	
	NSString * file_path;

	if ( [file_dialog runModalForDirectory:nil file:nil] == NSOKButton ) {

		NSArray* files = [file_dialog filenames]; // full paths, not just names
		file_path = [files objectAtIndex: 0];
	
	} else {
	
		file_path = @""; // the user cancelled
	
	}
	
	return StringAutoPtr ( new string ( [file_path cStringUsingEncoding: NSUTF8StringEncoding] ) );
	
} // SelectFileOrFolder


StringAutoPtr SelectFile ( StringAutoPtr prompt )
{
	return SelectFileOrFolder ( prompt, YES );
}


StringAutoPtr SelectFolder ( StringAutoPtr prompt )
{
	return SelectFileOrFolder ( prompt, NO );
}


int DisplayDialog ( StringAutoPtr title, StringAutoPtr message, StringAutoPtr ok_button, StringAutoPtr cancel_button, StringAutoPtr alternate_button )
{
	int button_pressed = 0;

	int response = NSRunAlertPanel ( NSStringFromStringAutoPtr ( title ),
									@"%@", 
									NSStringFromStringAutoPtr ( ok_button ), 
									NSStringFromStringAutoPtr ( cancel_button ), 
									NSStringFromStringAutoPtr ( alternate_button ), 
									NSStringFromStringAutoPtr ( message )
									);
	
	/*
	 translate the response so that the plug-in returns the same value for the same action
	 on both OS X and Windows
	 */
	
	switch ( response ) {
			
		case NSAlertDefaultReturn:    /* user pressed OK */
			button_pressed = kBE_OKButton;
			break;
			
		case NSAlertAlternateReturn:  /* user pressed Cancel */
			button_pressed = kBE_CancelButton;
			break;
			
		case NSAlertOtherReturn:      /* user pressed the third button */
			button_pressed = kBE_AlternateButton;
			break;
			
		case NSAlertErrorReturn:      /* an error occurred */
			break;
			
	}
	
	return button_pressed;
	
} // DisplayDialog


#pragma mark -
#pragma mark Other
#pragma mark -


bool OpenURL ( StringAutoPtr url )
{
	return [[NSWorkspace sharedWorkspace] openURL: [NSURL URLWithString: NSStringFromStringAutoPtr ( url )]];
}


