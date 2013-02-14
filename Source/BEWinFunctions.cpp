/*
 BEWinFunctions.cpp
 BaseElements Plug-in
	
 Copyright 2010-2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#include "BEWinFunctions.h"
#include "windows.h"
#include "Shlwapi.h"
#include "ShlObj.h"

#include "BEPluginGlobalDefines.h"


bool IsFileMakerClipboardType ( const wstring atype );
UINT32 ClipboardOffset ( const wstring atype );

// functions & globals for the dialog callback

static int CALLBACK SelectFolderCallback ( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData );
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


/* 
 FileMaker clipboard types for Windows are of the form Mac-XMxx
 */

bool IsFileMakerClipboardType ( const wstring atype )
{
	return ( atype.find ( L"Mac-XM" ) == 0 );
}


/* 
 for FileMaker types the first 4 bytes on the clipboard is the size of the data on the clipboard
 */

UINT32 ClipboardOffset ( const wstring atype )
{

	if ( IsFileMakerClipboardType ( atype ) ) {
		return 4;
	} else {
		return 0;
	}

}


StringAutoPtr ClipboardData ( WStringAutoPtr atype )
{
	char * clipboard_data = NULL;

	if ( OpenClipboard ( NULL ) ) {
		
		UINT format_wanted = RegisterClipboardFormat ( atype->c_str() );

		if ( IsClipboardFormatAvailable ( format_wanted ) ) {
			
			HGLOBAL memory_handle = GetClipboardData ( format_wanted );
			unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( memory_handle );
			SIZE_T clipboard_size = GlobalSize ( memory_handle );

			clipboard_data = new char [ clipboard_size + 1 ]();
			memcpy ( clipboard_data, clipboard_contents, clipboard_size );

			GlobalUnlock ( memory_handle );

		}
		
		CloseClipboard();
	
	} 
	
	if ( !clipboard_data ) {
		clipboard_data = new char [ 1 ]();
	}

	/* 
	 prefix the data to place on the clipboard with the size of the data
	 */
	UINT32 offset = ClipboardOffset ( *atype );
	StringAutoPtr reply ( new string ( clipboard_data + offset ) );

	delete[] clipboard_data;

	return reply;

} // ClipboardData


bool SetClipboardData ( StringAutoPtr data, WStringAutoPtr atype )
{
	bool ok = FALSE;
	
	if ( OpenClipboard ( NULL ) ) {
		
		EmptyClipboard();

		SIZE_T data_size = data->size();

		/* 
		 prefix the data to place on the clipboard with the size of the data
		 */
		SIZE_T offset = ClipboardOffset ( *atype );
		SIZE_T clipboard_size = data_size + offset;
		HGLOBAL memory_handle = GlobalAlloc ( GMEM_SHARE | GMEM_MOVEABLE, clipboard_size );

		unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( memory_handle );
		if ( IsFileMakerClipboardType ( *atype ) == TRUE ) {
			memcpy ( clipboard_contents, &data_size, offset );
		}
		memcpy ( clipboard_contents + offset, data->c_str(), clipboard_size );

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


// SelectFile based on code provided by Dan Smith, https://github.com/dansmith65

WStringAutoPtr SelectFile ( WStringAutoPtr prompt, WStringAutoPtr in_folder )
{
	OPENFILENAME open_file_dialog;   // common dialog box structure

	ZeroMemory ( &open_file_dialog, sizeof(open_file_dialog) );

	open_file_dialog.lStructSize = sizeof(open_file_dialog);
	open_file_dialog.hwndOwner = GetActiveWindow();

	wchar_t szFile[MAX_PATH];        // buffer for file name
	open_file_dialog.lpstrFile = szFile;
	open_file_dialog.lpstrFilter = L"All Files (*.*)\0*.*\0";
	open_file_dialog.lpstrFile[0] = '\0';
	open_file_dialog.nMaxFile = sizeof(szFile);
	open_file_dialog.lpstrTitle = prompt->c_str();
	open_file_dialog.lpstrInitialDir = in_folder->c_str();
	open_file_dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	 // Display the Open dialog box

	wchar_t path[MAX_PATH] = L"";

	if ( GetOpenFileName ( &open_file_dialog ) == TRUE ) {
		wcscpy_s ( path, open_file_dialog.lpstrFile );
	 }

	 return WStringAutoPtr ( new wstring ( path ) );

}	//	SelectFile


static int CALLBACK SelectFolderCallback ( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	// http://msdn.microsoft.com/en-us/library/windows/desktop/bb773205(v=vs.85).aspx

    if ( uMsg == BFFM_INITIALIZED ) {
        if ( NULL != lpData ) {
            SendMessage ( hwnd, BFFM_SETSELECTION, TRUE, lpData );
        }
    }
 
    return 0; // should always return 0
}


WStringAutoPtr SelectFolder ( WStringAutoPtr prompt, WStringAutoPtr in_folder )
{
	BROWSEINFO browse_info = { 0 };
	browse_info.hwndOwner = GetActiveWindow();
	browse_info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    browse_info.lpszTitle = prompt->c_str();
	if ( !in_folder->empty() ) {
		browse_info.lpfn = SelectFolderCallback;
		browse_info.lParam = (LPARAM)in_folder->c_str();
	}

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


#pragma mark -
#pragma mark User Preferences
#pragma mark -


bool SetPreference ( WStringAutoPtr key, WStringAutoPtr value, WStringAutoPtr domain )
{
	HKEY registry_key;
 
	g_last_error = RegCreateKeyEx ( HKEY_CURRENT_USER,
								domain->c_str(),
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_WRITE,
								NULL,
								&registry_key,
								NULL
								);

	if ( g_last_error == ERROR_SUCCESS ) {

		g_last_error = RegSetValueEx ( registry_key,
								key->c_str(),
								0,
								REG_SZ,
								(PBYTE)value->c_str(),
								(DWORD)(value->size() * sizeof(wchar_t)) + 1
								);
		RegCloseKey ( registry_key );
	}

	return ( g_last_error == ERROR_SUCCESS );

}


WStringAutoPtr GetPreference ( WStringAutoPtr key, WStringAutoPtr domain )
{

	HKEY registry_key;
	DWORD buffer_size = 1024;
	wchar_t * preference_data = new wchar_t[buffer_size]();

	g_last_error = RegOpenKeyEx ( HKEY_CURRENT_USER, 
							domain->c_str(),
							NULL,
							KEY_READ,
							&registry_key
							);
 
	if ( g_last_error == ERROR_SUCCESS ) {

		g_last_error = RegQueryValueEx ( registry_key,
									key->c_str(),
									NULL,
									NULL,
									(LPBYTE)preference_data,
									&buffer_size
									);

		// if preference_data isn't big enough resize it and try again
		if ( g_last_error == ERROR_MORE_DATA ) {

			delete [] preference_data;
			preference_data = new wchar_t[buffer_size + 1]();

			g_last_error = RegQueryValueEx ( registry_key,
										key->c_str(),
										NULL,
										NULL,
										(LPBYTE)preference_data,
										&buffer_size
										);
		}
		RegCloseKey ( registry_key );
	}

	WStringAutoPtr value = WStringAutoPtr ( new wstring ( preference_data ) );
  
	delete [] preference_data;

	return value;

}


#pragma mark -
#pragma mark Other
#pragma mark -


bool OpenURL ( WStringAutoPtr url )
{

	HINSTANCE result = ShellExecute ( NULL, (LPCWSTR)L"open", url->c_str(), NULL, NULL, SW_SHOWNORMAL );

	// see http://msdn.microsoft.com/en-us/library/bb762153(VS.85).aspx

	return ( result > (HINSTANCE)32 );
}


bool OpenFile ( WStringAutoPtr path )
{
	return OpenURL ( path );
}



#pragma mark -
#pragma mark Utilities
#pragma mark -

wstring utf8toutf16 ( const string& instr )
{
    // Assumes std::string is encoded using the UTF-8 codepage
    int bufferlen = ::MultiByteToWideChar(CP_UTF8, 0, instr.c_str(), instr.size(), NULL, 0);

    // Allocate new LPWSTR - must deallocate it later
    LPWSTR widestr = new WCHAR[bufferlen + 1];

    ::MultiByteToWideChar(CP_UTF8, 0, instr.c_str(), instr.size(), widestr, bufferlen);

    // Ensure wide string is null terminated
    widestr[bufferlen] = 0;

    // Do something with widestr
	wstring out ( widestr );

    delete[] widestr;

    return out;
}



string utf16ToUTF8 ( const wstring& s )
{
    const int size = ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL );

    vector<char> buf( size );
    ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL );

    return string( &buf[0] );
}

