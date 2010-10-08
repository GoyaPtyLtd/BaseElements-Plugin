/*
 BEWinFunctions.cpp
 BaseElements Plug-in
	
 Copyright 2010 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#include "BEWinFunctions.h"
#include "afxdlgs.h"
#include "Shlwapi.h"
#include "ShlObj.h"

#include "BEPluginGlobalDefines.h"


// functions & globals for the dialog callback

LRESULT CALLBACK DialogCallback ( int nCode, WPARAM wParam, LPARAM lParam );

HHOOK g_window_hook;

string g_button1_name;
string g_button2_name;
string g_button3_name;

// clipbaord functions

StringAutoPtr ClipboardFormats ( void )
{
	std::string format_list = "";

	if ( OpenClipboard ( NULL ) ) {
		
		UINT formats = CountClipboardFormats();
		UINT next_format = 0;
		
		for ( UINT i = 0 ; i < formats ; i++ ) {
			
			next_format = EnumClipboardFormats ( next_format );

			int max_count = 4096;
			HGLOBAL memory_handle = GlobalAlloc ( GMEM_SHARE | GMEM_MOVEABLE, max_count );
			LPTSTR format_name = (LPTSTR)GlobalLock ( memory_handle );
			int name_length = GetClipboardFormatName ( next_format, format_name, max_count );
			
			if ( name_length > 0 ) {
				format_list += format_name;
				format_list += "\r";
			}
			
			GlobalUnlock ( memory_handle );
			GlobalFree ( memory_handle );
			
		}

		CloseClipboard();

	}

	return StringAutoPtr ( new string ( format_list ) );
	
} // ClipboardFormats


StringAutoPtr ClipboardData ( StringAutoPtr atype )
{
	char * clipboard_data = "";

	UINT format_wanted = RegisterClipboardFormat ( atype->c_str() );

	if ( OpenClipboard ( NULL ) ) {
		
		if ( IsClipboardFormatAvailable ( format_wanted ) ) {
			
			HGLOBAL memory_handle = GetClipboardData ( format_wanted );
			unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( memory_handle );
			UINT clipboard_size = GlobalSize ( memory_handle );

			/*
			 the first 4 bytes on the clipboard is the size of the data on the clipboard
			 skip over the first 4 bytes and return the remaining data
			 */

			clipboard_data = (char *)malloc ( clipboard_size );
			memcpy ( clipboard_data, clipboard_contents + 4, clipboard_size - 4 );
			clipboard_data[ clipboard_size - 4 ] = '\0';

			GlobalUnlock ( memory_handle );
		}
		
		CloseClipboard();
	
	}

	StringAutoPtr reply ( new string ( clipboard_data ) );
	if ( clipboard_data ) free ( clipboard_data );

	return reply;

} // ClipboardData


bool SetClipboardData ( StringAutoPtr data, StringAutoPtr atype )
{
	bool ok = FALSE;
	
	if ( OpenClipboard ( NULL ) ) {
		
		EmptyClipboard();

		/* 
		 the first 4 bytes on the clipboard is the size of the data on the clipboard
		 prefix the data to place on the clipboard with the size of the data
		 */

		UINT32 data_size = data->size();
		UINT32 clipboard_size = data_size + 4;
		HGLOBAL memory_handle = GlobalAlloc ( GMEM_SHARE | GMEM_MOVEABLE, clipboard_size );

		unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( memory_handle );

		memcpy ( clipboard_contents, &data_size, 4 );
		memcpy ( clipboard_contents + 4, data->c_str(), clipboard_size );

		GlobalUnlock ( memory_handle );

		// put the data on clipboard ... the clipboard now owns memory_handle
		UINT format = RegisterClipboardFormat ( atype->c_str() );
		
		if ( SetClipboardData ( format, memory_handle ) ) {
			ok = TRUE;
		}

		CloseClipboard();
	}

	return ok;

}	//	Set_ClipboardData


// file dialogs

StringAutoPtr SelectFile ( StringAutoPtr prompt )
{
	CWnd * parent_window = CWnd::FromHandle ( GetActiveWindow() );
	CFileDialog file_dialog ( TRUE, NULL, NULL, OFN_HIDEREADONLY, "All Files (*.*)|*.*||", parent_window, 0 );
	file_dialog.m_ofn.lpstrTitle = prompt->c_str();

	// if the user hasn't cancelled the dialog get the path to the file

	char path[MAX_PATH] = "";
	
	if ( file_dialog.DoModal() == IDOK ) {
		CString file_path = file_dialog.GetPathName();
		strcpy_s ( path, (LPCTSTR)file_path );
	}

	return StringAutoPtr ( new string ( path ) );

}	//	SelectFile


StringAutoPtr SelectFolder ( StringAutoPtr prompt )
{
	BROWSEINFO browse_info = { 0 };
	browse_info.hwndOwner = GetActiveWindow();
	browse_info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_NONEWFOLDERBUTTON;
    browse_info.lpszTitle = prompt->c_str();

    LPITEMIDLIST item_list = SHBrowseForFolder ( &browse_info );
	
	// if the user hasn't cancelled the dialog get the path to the folder

    TCHAR path[MAX_PATH] = "";

	if ( item_list != 0 ) {
		SHGetPathFromIDList ( item_list, path );
	}

	return StringAutoPtr ( new string ( path ) );

}	//	SelectFolder


// (customized) alert dialogs

int DisplayDialog ( StringAutoPtr title, StringAutoPtr message, StringAutoPtr button1, StringAutoPtr button2, StringAutoPtr button3 )
{
	// set the names to be used for each button
	g_button1_name.swap ( *button1 );
	g_button2_name.swap ( *button2 );
	g_button3_name.swap ( *button3 );

	// the type of dialog displayed varies depends on the nunmber of buttons required
	UINT type = MB_OK;

	if ( g_button2_name != "" ) {
		type = MB_OKCANCEL;
		if ( g_button3_name != "" ) {
			type = MB_YESNOCANCEL;
		}
	}

	// set the callback so that the custom button names are installed
	g_window_hook = SetWindowsHookEx ( WH_CBT, &DialogCallback, 0, GetCurrentThreadId() );

	/*
	 get the user's choice and translate that into a response that matches the equivalent
	 choice on OS X
	 */

	int button_clicked = MessageBox ( GetActiveWindow(), (char *)message->c_str(), (char *)title->c_str(), type );

	unsigned long response = 0;

	switch ( button_clicked ) {
			
		case IDOK:
		case IDYES:
			response = kBE_OKButton;
			break;
		case IDCANCEL:
			response = kBE_CancelButton;
			break;
		case IDNO:
			response = kBE_AlternateButton;
			break;
	}

	return response;

} // DisplayDialog


// callback to override the button text on standard alert dialogs

LRESULT CALLBACK DialogCallback ( int nCode, WPARAM wParam, LPARAM lParam )
{
	UINT result = 0;

	if ( nCode == HCBT_ACTIVATE ) {
 
		HWND dialog = (HWND)wParam;

		// set the button text for each button
		// the first button can be IDOK or IDYES

		if ( GetDlgItem ( dialog, IDOK ) != NULL ) {
			result = SetDlgItemText ( dialog, IDOK, g_button1_name.c_str() );
		} else if ( GetDlgItem ( dialog, IDYES ) != NULL ) {
			result = SetDlgItemText ( dialog, IDYES, g_button1_name.c_str() );
		} 
		
		if ( GetDlgItem ( dialog, IDCANCEL ) != NULL ) {
			result = SetDlgItemText( dialog, IDCANCEL, g_button2_name.c_str() );
		} 
		
		if ( GetDlgItem ( dialog, IDNO ) != NULL ) {
			result = SetDlgItemText ( dialog, IDNO, g_button3_name.c_str() );
		}
 
		UnhookWindowsHookEx ( g_window_hook );
	} else {
		// there may be other installed hooks
		CallNextHookEx ( g_window_hook, nCode, wParam, lParam );
	}
	
	return result;

} // CBTProc
