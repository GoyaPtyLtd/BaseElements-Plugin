/*
 BEMacFunctions.cpp
 BaseElements Plug-In
 
 Copyright 2010-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#import "BEMacFunctions.h"
#import "BEPluginGlobalDefines.h"
#import "ProgressDialogWindowController.h"

#import <Cocoa/Cocoa.h>


using namespace std;


const std::wstring SelectFileOrFolder ( const std::wstring& prompt, const std::wstring& in_folder, bool choose_file );


ProgressDialogWindowController* progressDialog;


void InitialiseForPlatform ( )
{
	progressDialog = nil;
}


#pragma mark -
#pragma mark Clipboard
#pragma mark -

const std::wstring ClipboardFormats ( void )
{
	NSArray *types = [[[NSPasteboard generalPasteboard] types] copy];
	NSMutableString *formats = [NSMutableString stringWithCapacity: 1];
	
	for ( NSString *type in types ) {
		[formats appendString: type];
		[formats appendString: @FILEMAKER_END_OF_LINE];
	}
	
	return WStringFromNSString ( (NSString*)formats );
	
} // ClipboardFormats


const std::string ClipboardText ( const std::wstring& atype )
{
	NSString * pasteboard_type = NSStringFromWString ( atype );
	NSData * pasteboard_data = [[NSPasteboard generalPasteboard] dataForType: pasteboard_type];
	NSStringEncoding string_encoding = NSUTF8StringEncoding;
	if ( [pasteboard_type containsString: @"public.utf16-plain-text"] ) { // copy fm custom menus
		string_encoding = NSUTF16LittleEndianStringEncoding;
	}
	NSString * clipboard_data = [[NSString alloc] initWithData: pasteboard_data encoding: string_encoding];
    if ( clipboard_data == nil ) {
        clipboard_data = @"";
    }
	
	return [clipboard_data cStringUsingEncoding: NSUTF8StringEncoding];

} // ClipboardText


const bool SetClipboardText ( const std::string& data, const std::wstring& atype )
{
	NSString * data_to_copy = NSStringFromString ( data );
	NSString * data_type = NSStringFromWString ( atype );
	NSArray * new_types = [NSArray arrayWithObject: data_type];
	
	[[NSPasteboard generalPasteboard] declareTypes: new_types owner: nil];
	
	//	[new_types release];
	
	return [[NSPasteboard generalPasteboard] setString: data_to_copy forType: data_type];
	
} // Set_ClipboardText


const std::vector<unsigned char> ClipboardFile ( const std::wstring& atype )
{
	NSString * pasteboard_type = NSStringFromWString ( atype );
	NSData * pasteboard_data = [[NSPasteboard generalPasteboard] dataForType: pasteboard_type];
	
	vector<unsigned char> result;
	const unsigned char * bytes = (const unsigned char *)[pasteboard_data bytes];
	result.assign ( bytes, bytes + [pasteboard_data length] );
	
	return result;
	
} // ClipboardData


const bool SetClipboardFile ( const std::vector<unsigned char>& data, const std::wstring& atype )
{
	NSData * data_to_copy = [NSData dataWithBytes: data.data() length: data.size()];
	NSString * data_type = NSStringFromWString ( atype );
	NSArray * new_types = [NSArray arrayWithObject: data_type];
	
	[[NSPasteboard generalPasteboard] declareTypes: new_types owner: nil];
	
	return [[NSPasteboard generalPasteboard] setData: data_to_copy forType: data_type];
	
} // Set_ClipboardData


#pragma mark -
#pragma mark Dialogs
#pragma mark -

const std::wstring SelectFileOrFolder ( const std::wstring& prompt, const std::wstring& in_folder, bool choose_file )
{
	
	NSOpenPanel* file_dialog = [NSOpenPanel openPanel];
	
	NSString * prompt_string = NSStringFromWString ( prompt );
	[file_dialog setMessage: prompt_string];

	NSString * default_directory = NSStringFromWString ( in_folder );
	if ( [default_directory length] != 0 ) {
		NSURL *directory_url = [NSURL fileURLWithPath: default_directory];
		[file_dialog setDirectoryURL: directory_url];
	}
	
	[file_dialog setCanChooseFiles: choose_file];
	[file_dialog setCanChooseDirectories: !choose_file];

	// allow new directories to be created when selecting directories
	if ( !choose_file ) {
		[file_dialog setCanCreateDirectories: YES];
	}
	
	if ( choose_file ) {
		[file_dialog setAllowsMultipleSelection: YES];
	}
	
	NSMutableString * file_path = [NSMutableString stringWithString: @""];
	
	if ( [file_dialog runModal ] == NSModalResponseOK ) {
		
		NSArray* files = [file_dialog URLs];
		NSUInteger number_of_files = [files count];
		
		// return the file paths as a value list
		
		for ( NSUInteger i = 0 ; i < number_of_files ; i++ ) {
			[file_path appendString: [[files objectAtIndex: i] path]];
			if ( i + 1 != number_of_files ) {
				[file_path appendString: @FILEMAKER_END_OF_LINE];
			}
		}
		
		// [files release];
		
	} else {
//		[file_path stringWithString: @""]; // the user cancelled
	}
	
	//	[prompt_string release];
	
	return WStringFromNSString ( file_path );
	
} // SelectFileOrFolder


const std::wstring SelectFile ( const std::wstring& prompt, const std::wstring& in_folder )
{
	return SelectFileOrFolder ( prompt, in_folder, YES );
}


const std::wstring SelectFolder ( const std::wstring& prompt, const std::wstring& in_folder )
{
	return SelectFileOrFolder ( prompt, in_folder, NO );
}


const std::wstring SaveFileDialog ( std::wstring& prompt, std::wstring& fileName, std::wstring& inFolder )
{
	NSSavePanel* file_dialog = [NSSavePanel savePanel];
	
	NSString * prompt_string = NSStringFromWString ( prompt );
	[file_dialog setMessage: prompt_string];

	NSString * filename_string = NSStringFromWString ( fileName );
	[file_dialog setNameFieldStringValue: filename_string];
		
	NSString * default_directory = NSStringFromWString ( inFolder );
	if ( [default_directory length] != 0 ) {
		NSURL *directory_url = [NSURL fileURLWithPath: default_directory];
		[file_dialog setDirectoryURL: directory_url];
	}
	
	[file_dialog setCanCreateDirectories: YES];
	
	NSMutableString * file_path = [NSMutableString stringWithString: @""];
	
	if ( [file_dialog runModal ] == NSModalResponseOK ) {
		file_path = (NSMutableString *)[[file_dialog URL] path];
	} else {
		// the user cancelled
	}
	
	//	[prompt_string release];
	//	[filename_string release];

	return WStringFromNSString ( file_path );
	
} // SaveFileDialog




const int DisplayDialog ( std::wstring& title, std::wstring& message, std::wstring& ok_button, std::wstring& cancel_button, std::wstring& alternate_button )
{
	int button_pressed = 0;
	
	NSString * title_string = NSStringFromWString ( title );
	NSString * ok_button_string = NSStringFromWString ( ok_button );
	NSString * cancel_button_string = NSStringFromWString ( cancel_button );
	NSString * alternate_button_string = NSStringFromWString ( alternate_button );
	NSString * message_string = NSStringFromWString ( message );
	
	NSAlert *alert = [[NSAlert alloc] init];
	[alert addButtonWithTitle: ok_button_string];
	[alert addButtonWithTitle: cancel_button_string];
	[alert addButtonWithTitle: alternate_button_string];
	[alert setMessageText: title_string];
	[alert setInformativeText: message_string];
	[alert setAlertStyle: NSAlertStyleWarning];
	NSInteger response = [alert runModal];
	
	/*
	 translate the response so that the plug-in returns the same value for the same action
	 on both OS X and Windows
	 */
	
	switch ( response ) {
			
		case NSAlertFirstButtonReturn:    /* user pressed OK */
			button_pressed = kBE_ButtonOK;
			break;
			
		case NSAlertSecondButtonReturn:  /* user pressed Cancel */
			button_pressed = kBE_ButtonCancel;
			break;
			
		case NSAlertThirdButtonReturn:      /* user pressed the third button */
			button_pressed = kBE_ButtonAlternate;
			break;
			
		case NSModalResponseCancel:
		case NSModalResponseStop:
		case NSModalResponseAbort:
			break;
			
	}
	
	return button_pressed;
	
} // DisplayDialog



#pragma mark -
#pragma mark Progress Dialog
#pragma mark -


const fmx::errcode DisplayProgressDialog ( const std::wstring& title, const std::wstring& description, const long maximum, const bool can_cancel )
{
	
	fmx::errcode error = kNoError;

	if ( (progressDialog != nil) && ([progressDialog closed] == YES) ) {
		progressDialog = nil;
	}
	
	if ( progressDialog == nil ) {
		
		progressDialog = [[ProgressDialogWindowController alloc] initWithWindowNibName: @"BEProgressDialog"];

		if ( progressDialog != nil ) {

			NSString * title_string = NSStringFromWString ( title );
			NSString * description_string = NSStringFromWString ( description );
	
			[progressDialog show: title_string description: description_string maximumValue: maximum canCancel: can_cancel];
			
			// force fmp to display the dialog immediately - Jira #26
			[NSApp runModalForWindow: (NSWindow *)progressDialog];
			[NSApp stopModal];

		} else {
			error = kWindowIsMissingError;
		}

	} else {
		error = kFileOrObjectIsInUse;
	}
	
	return error;
}


const fmx::errcode UpdateProgressDialog ( const long value, const std::wstring& description )
{
	fmx::errcode error = kNoError;
	
	if ( progressDialog != nil ) {
		
		NSString * description_string;
		
		if ( !description.empty() ) {
			description_string = NSStringFromWString ( description );
		} else {
			description_string = NULL;
		}
		
		error = [progressDialog update: value description: description_string];
		
		if ( [progressDialog closed] == YES ) {
			progressDialog = nil;
		}
		
	} else {
		error = kWindowIsMissingError;
	}
	
	return error;
}



#pragma mark -
#pragma mark Other
#pragma mark -


const bool OpenURL ( std::wstring& url )
{	
	return [[NSWorkspace sharedWorkspace] openURL: [NSURL URLWithString: NSStringFromWString ( url ) ]];
}


const bool OpenFile ( std::wstring& path )
{
	return [[NSWorkspace sharedWorkspace] openURL: [NSURL fileURLWithPath: NSStringFromWString ( path ) ]];
}


const std::wstring get_machine_name ( )
{
	NSString * machine_name = [[NSHost currentHost] localizedName];
	return WStringFromNSString ( machine_name );
}


const std::string get_system_drive ( )
{
	std::string system_drive;
	
	const DASessionRef session = DASessionCreate ( kCFAllocatorDefault );
	if ( session ) {
		
		const CFURLRef url = CFURLCreateWithFileSystemPath ( kCFAllocatorDefault, CFSTR("/"), kCFURLPOSIXPathStyle, true );
		if ( url ) {
			
			const DADiskRef disk = DADiskCreateFromVolumePath ( kCFAllocatorDefault, session, url );
			if ( disk ) {
				
				const CFDictionaryRef description = DADiskCopyDescription ( disk );
				if ( description ) {
					
					const CFStringRef volume_name = (CFStringRef)CFDictionaryGetValue ( description, kDADiskDescriptionVolumeNameKey );
					if ( volume_name ) {
						system_drive = "/";
						system_drive.append ( [(__bridge NSString *)volume_name UTF8String] );
						system_drive.append ( "/" );
					}
					
					CFRelease ( description );
				}
				
				CFRelease ( disk );

			}
			
			CFRelease ( url );

		}

		CFRelease ( session );

	}
	
	return system_drive;
	
}

