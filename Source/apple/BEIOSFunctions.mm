/*
 BEIOSFunctions.cpp
 BaseElements Plug-In
 
 Copyright 2017-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#import "BEIOSFunctions.h"
#import "BEPluginGlobalDefines.h"

#import <UIKit/UIKit.h>


using namespace std;


const std::wstring SelectFileOrFolder ( const std::wstring& prompt, const std::wstring& in_folder, bool choose_file );


void InitialiseForPlatform ( )
{
//	progressDialog = nil;
}


#pragma mark -
#pragma mark Clipboard
#pragma mark -

const std::wstring ClipboardFormats ( void )
{

	return L"";

} // ClipboardFormats


const std::string ClipboardText ( const std::wstring& atype )
{
	
	NSString * pasteboard_type = NSStringFromWString ( atype );
	NSData * data = [[UIPasteboard generalPasteboard] dataForPasteboardType: pasteboard_type];
	NSString * clipboard_data =[[NSString alloc] initWithData:data encoding: NSUTF8StringEncoding];
	
	return [clipboard_data cStringUsingEncoding: NSUTF8StringEncoding];
	
} // ClipboardText


const bool SetClipboardText ( const std::string& data, const std::wstring& atype )
{
	
	NSString * data_to_copy = NSStringFromString ( data );
	NSString * pasteboard_type = NSStringFromWString ( atype );
	NSData * clipboard_data = [data_to_copy dataUsingEncoding: NSUTF8StringEncoding];
	[[UIPasteboard generalPasteboard] setData: clipboard_data forPasteboardType: pasteboard_type];

	return true;
	
} // SetClipboardText


const std::vector<unsigned char> ClipboardFile ( const std::wstring& atype )
{
	NSString * pasteboard_type = NSStringFromWString ( atype );
	NSData * pasteboard_data = [[UIPasteboard generalPasteboard] dataForPasteboardType: pasteboard_type];
	
	vector<unsigned char> result;
	const unsigned char * bytes = (const unsigned char *)[pasteboard_data bytes];
	result.assign ( bytes, bytes + [pasteboard_data length] );
	
	return result;
	
} // ClipboardData


const bool SetClipboardFile ( const std::vector<unsigned char>& data, const std::wstring& atype )
{
	NSData * data_to_copy = [NSData dataWithBytes: data.data() length: data.size()];
	NSString * data_type = NSStringFromWString ( atype );
	[[UIPasteboard generalPasteboard] setData: data_to_copy forPasteboardType: data_type];
	
	return true;

} // Set_ClipboardData


#pragma mark -
#pragma mark Dialogs
#pragma mark -

const std::wstring SelectFileOrFolder ( const std::wstring& /* prompt */, const std::wstring& /* in_folder */, bool /* choose_file */ )
{

	//	https://developer.apple.com/documentation/uikit/uidocumentpickerviewcontroller

	return L"";
	
} // SelectFileOrFolder


const std::wstring SelectFile ( const std::wstring& prompt, const std::wstring& in_folder )
{
	return SelectFileOrFolder ( prompt, in_folder, YES );
}


const std::wstring SelectFolder ( const std::wstring& prompt, const std::wstring& in_folder )
{
	return SelectFileOrFolder ( prompt, in_folder, NO );
}


const std::wstring SaveFileDialog ( std::wstring& /* prompt */, std::wstring& /* fileName */, std::wstring& /* inFolder */ )
{
	
	//	https://developer.apple.com/documentation/uikit/uidocumentpickerviewcontroller
	
	return L"";
	
} // SaveFileDialog




const int DisplayDialog ( std::wstring& /* title */, std::wstring& /* message */, std::wstring& /* ok_button */, std::wstring& /* cancel_button */, std::wstring& /* alternate_button */ )
{

	//	https://code.tutsplus.com/tutorials/ios-fundamentals-uialertview-and-uialertcontroller--cms-24038
	
	return 0;

} // DisplayDialog



#pragma mark -
#pragma mark Progress Dialog
#pragma mark -


const fmx::errcode DisplayProgressDialog ( const std::wstring& /* title */, const std::wstring& /* description*/, const long /* maximum */, const bool /* can_cancel */ )
{
	fmx::errcode error = kNoError;
	
	//	https://agilewarrior.wordpress.com/2012/04/03/how-to-display-uiactivityindicatorview-spinner-for-long-running-operations/
	
	return error;
	
}


const fmx::errcode UpdateProgressDialog ( const long /* value */, const std::wstring& /* description */ )
{
	fmx::errcode error = kNoError;
	return error;
}



#pragma mark -
#pragma mark Other
#pragma mark -


const bool OpenURL ( std::wstring& url )
{
	auto result = false;
	NSURL * open_this = [NSURL URLWithString: NSStringFromWString ( url ) ];
	if ( [[UIApplication sharedApplication] canOpenURL: open_this] ) {
		[[UIApplication sharedApplication] openURL: open_this options: @{} completionHandler: nil];
		result = true;
	}
	return result;
	
}


const bool OpenFile ( std::wstring& /* path */ )
{	
	return false;
}


const std::wstring get_machine_name ( )
{
	NSString * machine_name = [[UIDevice currentDevice] name];
	return WStringFromNSString ( machine_name );
}


const std::string get_system_drive ( )
{
	return ""; // matching Get ( SystemDrive )
}

