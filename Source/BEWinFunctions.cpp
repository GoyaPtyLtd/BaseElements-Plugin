/*
 BEWinFunctions.cpp
 BaseElements Plug-in
	
 Copyright 2010-2011 Goya. All rights reserved.
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

wstring g_button1_name;
wstring g_button2_name;
wstring g_button3_name;

// clipbaord functions

WStringAutoPtr ClipboardFormats ( void )
{
	wstring format_list = L"";

	if ( OpenClipboard ( NULL ) ) {
		
		UINT formats = CountClipboardFormats();
		UINT next_format = 0;
		
		for ( UINT i = 0 ; i < formats ; i++ ) {
			
			next_format = EnumClipboardFormats ( next_format );

			const int max_count = 4096;
			wchar_t format_name[max_count];
			int name_length = GetClipboardFormatName ( next_format, format_name, max_count );
			
			if ( name_length > 0 ) {
				format_list += format_name;
				format_list += L"\r";
			}
			
		}

		CloseClipboard();

	}

	return WStringAutoPtr ( new wstring ( format_list ) );
	
} // ClipboardFormats


StringAutoPtr ClipboardData ( WStringAutoPtr atype )
{
	char * clipboard_data;

	if ( OpenClipboard ( NULL ) ) {
		
		UINT format_wanted = RegisterClipboardFormat ( atype->c_str() );

		if ( IsClipboardFormatAvailable ( format_wanted ) ) {
			
			HGLOBAL memory_handle = GetClipboardData ( format_wanted );
			unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( memory_handle );
			UINT clipboard_size = GlobalSize ( memory_handle );

			clipboard_data = new char [ clipboard_size + 1 ]();
			memcpy ( clipboard_data, clipboard_contents, clipboard_size );

			GlobalUnlock ( memory_handle );

		}
		
		CloseClipboard();
	
	} 
	
	if ( !clipboard_data ) {
		clipboard_data = "";
	}

	StringAutoPtr reply ( new string ( clipboard_data ) );

	delete[] clipboard_data;

	return reply;

} // ClipboardData


bool SetClipboardData ( StringAutoPtr data, WStringAutoPtr atype )
{
	bool ok = FALSE;
	
	if ( OpenClipboard ( NULL ) ) {
		
		EmptyClipboard();

		UINT32 data_size = data->size();
		UINT32 clipboard_size = data_size;
		HGLOBAL memory_handle = GlobalAlloc ( GMEM_SHARE | GMEM_MOVEABLE, clipboard_size );

		unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( memory_handle );
		memcpy ( clipboard_contents, data->c_str(), clipboard_size );

		GlobalUnlock ( memory_handle );

		UINT format = RegisterClipboardFormat ( atype->c_str() );
		
		if ( SetClipboardData ( format, clipboard_contents ) ) {
			ok = TRUE;
		}

		CloseClipboard();

	}

	return ok;

}	//	Set_ClipboardData


// file dialogs

WStringAutoPtr SelectFile ( WStringAutoPtr prompt )
{
	CWnd * parent_window = CWnd::FromHandle ( GetActiveWindow() );
	CString filter =  "All Files (*.*)|*.*||";
	CFileDialog file_dialog ( TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, parent_window, 0 );
	file_dialog.m_ofn.lpstrTitle = prompt->c_str();

	// if the user hasn't cancelled the dialog get the path to the file

	wchar_t path[MAX_PATH] = L"";
	
	if ( file_dialog.DoModal() == IDOK ) {
		CString file_path = file_dialog.GetPathName();
		wcscpy_s ( path, (LPCTSTR)file_path );
	}

	return WStringAutoPtr ( new wstring ( path ) );

}	//	SelectFile


WStringAutoPtr SelectFolder ( WStringAutoPtr prompt )
{
	BROWSEINFO browse_info = { 0 };
	browse_info.hwndOwner = GetActiveWindow();
	browse_info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_NONEWFOLDERBUTTON;
    browse_info.lpszTitle = prompt->c_str();

    LPITEMIDLIST item_list = SHBrowseForFolder ( &browse_info );
	
	// if the user hasn't cancelled the dialog get the path to the folder

    wchar_t path[MAX_PATH] = L"";

	if ( item_list != 0 ) {
		SHGetPathFromIDList ( item_list, path );
	}

	return WStringAutoPtr ( new wstring ( path ) );

}	//	SelectFolder


// (customized) alert dialogs

int DisplayDialog ( WStringAutoPtr title, WStringAutoPtr message, WStringAutoPtr button1, WStringAutoPtr button2, WStringAutoPtr button3 )
{
	// set the names to be used for each button
	g_button1_name.swap ( *button1 );
	g_button2_name.swap ( *button2 );
	g_button3_name.swap ( *button3 );

	// the type of dialog displayed varies depends on the nunmber of buttons required
	UINT type = MB_OK;

	if ( g_button2_name != L"" ) {
		type = MB_OKCANCEL;
		if ( g_button3_name != L"" ) {
			type = MB_YESNOCANCEL;
		}
	}

	// set the callback so that the custom button names are installed
	g_window_hook = SetWindowsHookEx ( WH_CBT, &DialogCallback, 0, GetCurrentThreadId() );

	/*
	 get the user's choice and translate that into a response that matches the equivalent
	 choice on OS X
	 */

	int button_clicked = MessageBox ( GetActiveWindow(), message->c_str(), title->c_str(), type );

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



bool OpenURL ( WStringAutoPtr url )
{

	HINSTANCE result = ShellExecute ( NULL, (LPCWSTR)L"open", url->c_str(), NULL, NULL, SW_SHOWNORMAL );

	// see http://msdn.microsoft.com/en-us/library/bb762153(VS.85).aspx

	return ( result > (HINSTANCE)32 );
}

