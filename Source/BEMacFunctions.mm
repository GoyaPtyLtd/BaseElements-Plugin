/*
 BEMacFunctions.cpp
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#import "BENSString.h"
#import "BEMacFunctions.h"


WStringAutoPtr SelectFileOrFolder ( WStringAutoPtr prompt, bool choose_file );


#pragma mark -
#pragma mark Clipboard
#pragma mark -

WStringAutoPtr ClipboardFormats ( void )
{
	NSArray *types = [[[NSPasteboard generalPasteboard] types] copy];
	NSMutableString *formats = [NSMutableString stringWithCapacity: 1];
	
	for ( NSString *type in types ) {
		[formats appendString: type];
		[formats appendString: @"\r"];
	}

	[types release];

	return [formats WStringAutoPtrFromNSString];

} // ClipboardFormats


StringAutoPtr ClipboardData ( WStringAutoPtr atype )
{
	NSString * pasteboard_type = [NSString NSStringFromWStringAutoPtr: atype ];
	NSData * pasteboard_data = [[[NSPasteboard generalPasteboard] dataForType: pasteboard_type] copy];				
	NSString * clipboard_data = [[NSString alloc] initWithData: pasteboard_data encoding: NSUTF8StringEncoding];

//	[pasteboard_data release];
	
	return StringAutoPtr ( new string ( [clipboard_data cStringUsingEncoding: NSUTF8StringEncoding] ) );	

} // ClipboardData


bool SetClipboardData ( StringAutoPtr data, WStringAutoPtr atype )
{
	NSString * data_to_copy = [NSString NSStringFromStringAutoPtr: data ];
	NSString * data_type = [NSString NSStringFromWStringAutoPtr: atype ];
	NSArray * new_types = [NSArray arrayWithObject: data_type];

	[[NSPasteboard generalPasteboard] declareTypes: new_types owner: nil];

//	[new_types release];

	return [[NSPasteboard generalPasteboard] setString: data_to_copy forType: data_type];

} // Set_ClipboardData


#pragma mark -
#pragma mark Dialogs
#pragma mark -

WStringAutoPtr SelectFileOrFolder ( WStringAutoPtr prompt, bool choose_file )
{
	
	NSOpenPanel* file_dialog = [NSOpenPanel openPanel];

	NSString * prompt_string = [NSString NSStringFromWStringAutoPtr: prompt ];
	[file_dialog setTitle: prompt_string ];
	[file_dialog setCanChooseFiles: choose_file];
	[file_dialog setCanChooseDirectories: !choose_file];
	
	NSString * file_path;

	if ( [file_dialog runModalForDirectory:nil file:nil] == NSOKButton ) {

		NSArray* files = [file_dialog filenames]; // full paths, not just names
		file_path = [files objectAtIndex: 0];
//		[files release];
	
	} else {
	
		file_path = @""; // the user cancelled
	
	}
	
//	[prompt_string release];

	return [file_path WStringAutoPtrFromNSString];
	
} // SelectFileOrFolder


WStringAutoPtr SelectFile ( WStringAutoPtr prompt )
{
	return SelectFileOrFolder ( prompt, YES );
}


WStringAutoPtr SelectFolder ( WStringAutoPtr prompt )
{
	return SelectFileOrFolder ( prompt, NO );
}


int DisplayDialog ( WStringAutoPtr title, WStringAutoPtr message, WStringAutoPtr ok_button, WStringAutoPtr cancel_button, WStringAutoPtr alternate_button )
{
	int button_pressed = 0;

	NSString * title_string = [NSString NSStringFromWStringAutoPtr: title ];
	NSString * ok_button_string = [NSString NSStringFromWStringAutoPtr: ok_button ];
	NSString * cancel_button_string = [NSString NSStringFromWStringAutoPtr: cancel_button ];
	NSString * alternate_button_string = [NSString NSStringFromWStringAutoPtr: alternate_button ];
	NSString * message_string = [NSString NSStringFromWStringAutoPtr: message ];

	int response = NSRunAlertPanel (  ( title_string ),
									@"%@", 
									 ( ok_button_string ), 
									 ( cancel_button_string ), 
									 ( alternate_button_string ), 
									 ( message_string )
									);
	
//	[title_string release];
//	[ok_button_string release];
//	[cancel_button_string release];
//	[alternate_button_string release];
//	[message_string release];

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


bool OpenURL ( WStringAutoPtr url )
{	
	return [[NSWorkspace sharedWorkspace] openURL: [NSURL URLWithString: [NSString NSStringFromWStringAutoPtr: url] ]];
}


