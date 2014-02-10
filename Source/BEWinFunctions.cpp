/*
 BEWinFunctions.cpp
 BaseElements Plug-in
	
 Copyright 2010-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#include <Shlwapi.h>
#include <ShlObj.h>
#include <ShObjIdl.h>
#include <ObjBase.h>


#include <sstream>


#include <boost/algorithm/string.hpp>


#include "BEWinFunctions.h"


using namespace std;


wstring ClipboardFormatNameForID ( const UINT format_id );
UINT ClipboardFormatIDForName ( const wstring format_name );
bool SafeOpenClipboard ( void );
bool IsFileMakerClipboardType ( const wstring atype );
UINT32 ClipboardOffset ( const wstring atype );

// functions & globals for the dialog callback

static int CALLBACK SelectFolderCallback ( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData );
LRESULT CALLBACK DialogCallback ( int nCode, WPARAM wParam, LPARAM lParam );

HHOOK g_window_hook;

wstring g_button1_name;
wstring g_button2_name;
wstring g_button3_name;

// globals for the progress dialog

#ifndef PROGDLG_MARQUEEPROGRESS
#define PROGDLG_MARQUEEPROGRESS  0x00000020
#endif

#ifndef PROGDLG_NOCANCEL
#define PROGDLG_NOCANCEL         0x00000040
#endif

IProgressDialog * progress_dialog;
DWORD progress_dialog_maximum;


// dry... fmx::errcode is not large enough to hold all results from GetLastError()

fmx::errcode GetLastErrorAsFMX ( void )
{
	return (fmx::errcode)GetLastError();
}


// clipbaord functions

// default, system formats
// http://msdn.microsoft.com/en-us/library/windows/desktop/ff729168(v=vs.85).aspx


wstring ClipboardFormatNameForID ( const UINT format_id )
{
	wstring format_name;

	switch ( format_id ) {

		case CF_BITMAP:
			format_name = L"CF_BITMAP";
			break;

		case CF_DIB:
			format_name = L"CF_DIB";
			break;

		case CF_DIBV5:
			format_name = L"CF_DIBV5";
			break;

		case CF_DIF:
			format_name = L"CF_DIF";
			break;

		case CF_DSPBITMAP:
			format_name = L"CF_DSPBITMAP";
			break;

		case CF_DSPENHMETAFILE:
			format_name = L"CF_DSPENHMETAFILE";
			break;

		case CF_DSPMETAFILEPICT:
			format_name = L"CF_DSPMETAFILEPICT";
			break;

		case CF_DSPTEXT:
			format_name = L"CF_DSPTEXT";
			break;

		case CF_ENHMETAFILE:
			format_name = L"CF_ENHMETAFILE";
			break;

		case CF_GDIOBJFIRST:
			format_name = L"CF_GDIOBJFIRST";
			break;

		case CF_GDIOBJLAST:
			format_name = L"CF_GDIOBJLAST";
			break;

		case CF_HDROP:
			format_name = L"CF_HDROP";
			break;

		case CF_LOCALE:
			format_name = L"CF_LOCALE";
			break;

		case CF_METAFILEPICT:
			format_name = L"CF_METAFILEPICT";
			break;

		case CF_OEMTEXT:
			format_name = L"CF_OEMTEXT";
			break;

		case CF_OWNERDISPLAY:
			format_name = L"CF_OWNERDISPLAY";
			break;

		case CF_PALETTE:
			format_name = L"CF_PALETTE";
			break;

		case CF_PENDATA:
			format_name = L"CF_PENDATA";
			break;

		case CF_PRIVATEFIRST:
			format_name = L"CF_PRIVATEFIRST";
			break;

		case CF_PRIVATELAST:
			format_name = L"CF_PRIVATELAST";
			break;

		case CF_RIFF:
			format_name = L"CF_RIFF";
			break;

		case CF_SYLK:
			format_name = L"CF_SYLK";
			break;

		case CF_TEXT:
			format_name = L"CF_TEXT";
			break;

		case CF_TIFF:
			format_name = L"CF_TIFF";
			break;

		case CF_UNICODETEXT:
			format_name = L"CF_UNICODETEXT";
			break;

		case CF_WAVE:
			format_name = L"CF_WAVE";
			break;

		default:

			wchar_t format[PATH_MAX];
			int name_length = GetClipboardFormatName ( format_id, format, PATH_MAX );
			format_name = format;

	}

	return format_name;

}



UINT ClipboardFormatIDForName ( const wstring name ) {
	
	UINT format_id = 0;

	wstring format_name = boost::to_upper_copy ( name );

	if ( format_name == L"CF_BITMAP" ) {
		format_id = CF_BITMAP;
	} else if ( format_name == L"CF_DIB" ) {
		format_id = CF_DIB;
	} else if ( format_name == L"CF_DIBV5" ) {
		format_id = CF_DIBV5;
	} else if ( format_name == L"CF_DIF" ) {
		format_id = CF_DIF;
	} else if ( format_name == L"CF_DSPBITMAP" ) {
		format_id = CF_DSPBITMAP;
	} else if ( format_name == L"CF_DSPENHMETAFILE" ) {
		format_id = CF_DSPENHMETAFILE;
	} else if ( format_name == L"CF_DSPMETAFILEPICT" ) {
		format_id = CF_DSPMETAFILEPICT;
	} else if ( format_name == L"CF_DSPTEXT" ) {
		format_id = CF_DSPTEXT;
	} else if ( format_name == L"CF_ENHMETAFILE" ) {
		format_id = CF_ENHMETAFILE;
	} else if ( format_name == L"CF_GDIOBJFIRST" ) {
		format_id = CF_GDIOBJFIRST;
	} else if ( format_name == L"CF_GDIOBJLAST" ) {
		format_id = CF_GDIOBJLAST;
	} else if ( format_name == L"CF_HDROP" ) {
		format_id = CF_HDROP;
	} else if ( format_name == L"CF_LOCALE" ) {
		format_id = CF_LOCALE;
	} else if ( format_name == L"CF_METAFILEPICT" ) {
		format_id = CF_METAFILEPICT;
	} else if ( format_name == L"CF_OEMTEXT" ) {
		format_id = CF_OEMTEXT;
	} else if ( format_name == L"CF_OWNERDISPLAY" ) {
		format_id = CF_OWNERDISPLAY;
	} else if ( format_name == L"CF_PALETTE" ) {
		format_id = CF_PALETTE;
	} else if ( format_name == L"CF_PENDATA" ) {
		format_id = CF_PENDATA;
	} else if ( format_name == L"CF_PRIVATEFIRST" ) {
		format_id = CF_PRIVATEFIRST;
	} else if ( format_name == L"CF_PRIVATELAST" ) {
		format_id = CF_PRIVATELAST;
	} else if ( format_name == L"CF_RIFF" ) {
		format_id = CF_RIFF;
	} else if ( format_name == L"CF_SYLK" ) {
		format_id = CF_SYLK;
	} else if ( format_name == L"CF_TEXT" ) {
		format_id = CF_TEXT;
	} else if ( format_name == L"CF_TIFF" ) {
		format_id = CF_TIFF;
	} else if ( format_name == L"CF_UNICODETEXT" ) {
		format_id = CF_UNICODETEXT;
	} else if ( format_name == L"CF_WAVE" ) {
		format_id = CF_WAVE;
	} else {
		format_id = RegisterClipboardFormat ( name.c_str() );
	}

	return format_id;

}
	
// transient access is denied errors (#5) can occur after setting the clipboard

bool SafeOpenClipboard ( void )
{
	BOOL is_open = OpenClipboard ( NULL );
	int i = 0;
	while ( GetLastError() && i < 1 )  {
		Sleep ( 10 );
		++i;
		is_open = OpenClipboard ( NULL );
	}

	g_last_error = GetLastErrorAsFMX();

	return is_open != 0;

}

	
WStringAutoPtr ClipboardFormats ( void )
{
	wstring format_list = L"";

	if ( SafeOpenClipboard() ) {
		
		UINT formats = CountClipboardFormats();
		UINT next_format = 0;
		
		for ( UINT i = 0 ; i < formats ; i++ ) {
			
			next_format = EnumClipboardFormats ( next_format );

			format_list += ClipboardFormatNameForID ( next_format );
			format_list += L"\r"; // ( FILEMAKER_END_OF_LINE );
			
		}

		if ( CloseClipboard() ) {
			g_last_error = GetLastErrorAsFMX();
		}

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
		return 4; // char not wchar_t
	} else {
		return 0;
	}

}


StringAutoPtr UTF8ClipboardData ( const WStringAutoPtr atype )
{
	char * clipboard_data = NULL;

	UINT format_wanted = ClipboardFormatIDForName ( *atype );
	HGLOBAL clipboard_memory = GetClipboardData ( format_wanted );
	unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( clipboard_memory );
	SIZE_T clipboard_size = GlobalSize ( clipboard_memory );

	clipboard_data = new char [ clipboard_size + 1 ]();
	memcpy_s ( clipboard_data, clipboard_size, clipboard_contents, clipboard_size );

	GlobalUnlock ( clipboard_memory );
	
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

} // UTF8ClipboardData


StringAutoPtr WideClipboardData ( const WStringAutoPtr atype )
{
	wchar_t * clipboard_data = NULL;

	UINT format_wanted = ClipboardFormatIDForName ( *atype );

	HGLOBAL clipboard_memory = GetClipboardData ( format_wanted );
	wchar_t * clipboard_contents = (wchar_t *)GlobalLock ( clipboard_memory );
	SIZE_T clipboard_size = GlobalSize ( clipboard_memory ) / sizeof ( wchar_t );
	clipboard_data = new wchar_t [ clipboard_size + 1 ]();
	wmemcpy_s ( clipboard_data, clipboard_size, clipboard_contents, clipboard_size );

	GlobalUnlock ( clipboard_memory );
	
	if ( !clipboard_data ) {
		clipboard_data = new wchar_t [ 1 ]();
	}

	string utf8 = utf16ToUTF8 ( clipboard_data );
	delete[] clipboard_data;
	StringAutoPtr reply ( new string ( utf8 ) );

	return reply;

} // WideClipboardData


StringAutoPtr ClipboardData ( WStringAutoPtr atype )
{
	StringAutoPtr reply ( new string );

	if ( SafeOpenClipboard() ) {
		
		UINT format = ClipboardFormatIDForName ( *atype );

		if ( IsClipboardFormatAvailable ( format ) ) {

			if ( format == CF_UNICODETEXT ) {
				reply = WideClipboardData ( atype );
			} else {
				reply = UTF8ClipboardData ( atype );
			}

		}
		
		if ( CloseClipboard() ) {
			g_last_error = GetLastErrorAsFMX();
		}
	
	}
	
	return reply;

} // ClipboardData



HGLOBAL DataForClipboardAsUTF8 ( const StringAutoPtr data, const WStringAutoPtr atype )
{
	SIZE_T data_size = data->size();

	/* 
	 prefix the data to place on the clipboard with the size of the data
	 */
	SIZE_T offset = ClipboardOffset ( *atype );
	SIZE_T clipboard_size = data_size + offset;

	HGLOBAL clipboard_memory = GlobalAlloc ( GMEM_MOVEABLE, clipboard_size );
	unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( clipboard_memory );

	if ( IsFileMakerClipboardType ( *atype ) == TRUE ) {
		memcpy_s ( clipboard_contents, offset, &data_size, offset );
	}

	memcpy_s ( clipboard_contents + offset, clipboard_size, data->c_str(), clipboard_size );

	GlobalUnlock ( clipboard_memory );

	return clipboard_memory;

}	//	DataForClipboardAsUTF8



void * DataForClipboardAsWide ( const StringAutoPtr data )
{
	wstring data_as_wide = utf8toutf16 ( *data );

	SIZE_T clipboard_size = data_as_wide.size() + 1;
	SIZE_T memory_size = clipboard_size * sizeof ( wchar_t );

	HGLOBAL clipboard_memory = GlobalAlloc ( GMEM_MOVEABLE, memory_size );
	wchar_t * clipboard_contents = (wchar_t *)GlobalLock ( clipboard_memory );
	wmemcpy_s ( clipboard_contents, clipboard_size, data_as_wide.c_str(), clipboard_size );

	GlobalUnlock ( clipboard_memory );

	return clipboard_memory;

}	//	DataForClipboardAsWide



bool SetClipboardData ( StringAutoPtr data, WStringAutoPtr atype )
{
	bool ok = FALSE;
	
	if ( OpenClipboard ( GetActiveWindow() ) ) {
		
		if ( EmptyClipboard() ) {

			UINT format = ClipboardFormatIDForName ( *atype );

			HGLOBAL clipboard_memory;

			if ( format == CF_UNICODETEXT ) {
				clipboard_memory = DataForClipboardAsWide ( data );
			} else {
				clipboard_memory = DataForClipboardAsUTF8 ( data, atype );
			}

			if ( SetClipboardData ( format, clipboard_memory ) ) {
				ok = TRUE;
			}
		} else {
			g_last_error = GetLastErrorAsFMX();
		}

		if ( CloseClipboard() ) {
			g_last_error = g_last_error ? g_last_error : GetLastErrorAsFMX();
		}

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
	open_file_dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT;

	 // Display the Open dialog box

	wchar_t path[MAX_PATH] = L"";

	if ( GetOpenFileName ( &open_file_dialog ) == TRUE ) {
		wcscpy_s ( path, open_file_dialog.lpstrFile );
	 }

	// return the file paths as a value list

	wstringstream wss ( path );
	istream_iterator<wstring, wchar_t, std::char_traits<wchar_t> > begin ( wss );
	istream_iterator<wstring, wchar_t, std::char_traits<wchar_t> > end;
	vector <wstring> values ( begin, end );

	WStringAutoPtr selected_files = WStringAutoPtr ( new wstring ( L"" ) );

	size_t number_of_files = values.size();

	if ( number_of_files == 1 ) {
		selected_files->append ( values[0] );
	} else {

		for ( size_t i = 1 ; i < number_of_files ; i++ ) {

			selected_files->append ( values[0] );
			selected_files->append ( values[i] );

			if ( i + 1 != number_of_files ) {
				selected_files->append ( L"\r" /*FILEMAKER_END_OF_LINE*/ );
			}

		}

	} // if ( number_of_files == 1 )
	
	return selected_files;

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
#pragma mark Progress Dialog
#pragma mark -


fmx::errcode DisplayProgressDialog ( const WStringAutoPtr title, const WStringAutoPtr description, const long maximum, const bool can_cancel )
{
	fmx::errcode error = kNoError;

	if ( progress_dialog ) {
		return error;
	}

	progress_dialog_maximum = maximum;

	HWND parent_window = GetActiveWindow ( );
	HRESULT result = CoCreateInstance ( CLSID_ProgressDialog, NULL, CLSCTX_INPROC_SERVER, IID_IProgressDialog, (void**)&progress_dialog );

	if ( result == S_OK ) {
		result = progress_dialog->SetTitle ( title->c_str () );
	}

	if ( result == S_OK ) {
		progress_dialog->SetLine ( 2, description->c_str (), false, NULL );
	}

	DWORD flags = PROGDLG_NORMAL;
	if ( !can_cancel ) {
		flags |= PROGDLG_NOCANCEL;
	}

	if ( 0 == maximum ) {
		flags |= PROGDLG_MARQUEEPROGRESS;
	}

	if ( result == S_OK ) {
		result = progress_dialog->StartProgressDialog ( parent_window, NULL, flags, NULL );
	}

	return (fmx::errcode)result;

} // DisplayProgressDialog


fmx::errcode UpdateProgressDialog ( const unsigned long value, const WStringAutoPtr description )
{
	fmx::errcode error = kNoError;

	BOOL user_cancelled = progress_dialog->HasUserCancelled();

	if ( (user_cancelled != 0) || (value > progress_dialog_maximum) ) {

		progress_dialog->StopProgressDialog();
		progress_dialog->Release();
        progress_dialog = NULL;

		error = user_cancelled ? kUserCancelledError : error;

	} else {

		HRESULT result;

		if ( !description->empty () ) {
			result = progress_dialog->SetLine ( 2, description->c_str (), false, NULL );
		}

		if ( result == S_OK ) {
			result = progress_dialog->SetProgress ( value, progress_dialog_maximum );
			error = result == S_FALSE ? kNoError : (fmx::errcode)result;
		}
	}

	return error;
}



#pragma mark -
#pragma mark User Preferences
#pragma mark -


bool SetPreference ( WStringAutoPtr key, WStringAutoPtr value, WStringAutoPtr domain )
{
	HKEY registry_key;
 
	LSTATUS status;

	status = RegCreateKeyEx ( HKEY_CURRENT_USER,
								domain->c_str(),
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_WRITE,
								NULL,
								&registry_key,
								NULL
								);

	if ( status == ERROR_SUCCESS ) {

		status = RegSetValueEx ( registry_key,
								key->c_str(),
								0,
								REG_SZ,
								(PBYTE)value->c_str(),
								(DWORD)(value->size() * sizeof(wchar_t)) + 1
								);
		RegCloseKey ( registry_key );
	}

	g_last_error = (fmx::errcode)status;

	return ( status == ERROR_SUCCESS );

}


WStringAutoPtr GetPreference ( WStringAutoPtr key, WStringAutoPtr domain )
{

	HKEY registry_key;
	DWORD buffer_size = 1024;
	wchar_t * preference_data = new wchar_t[buffer_size]();

	LSTATUS status;

	status = RegOpenKeyEx ( HKEY_CURRENT_USER, 
							domain->c_str(),
							NULL,
							KEY_READ,
							&registry_key
							);
 
	if ( status == ERROR_SUCCESS ) {

		status = RegQueryValueEx ( registry_key,
									key->c_str(),
									NULL,
									NULL,
									(LPBYTE)preference_data,
									&buffer_size
									);

		// if preference_data isn't big enough resize it and try again
		if ( status == ERROR_MORE_DATA ) {

			delete [] preference_data;
			preference_data = new wchar_t[buffer_size + 1]();

			status = RegQueryValueEx ( registry_key,
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

	g_last_error = (fmx::errcode)status;

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
    int bufferlen = MultiByteToWideChar ( CP_UTF8, 0, instr.c_str(), (int)instr.size(), NULL, 0 );

    // Allocate new LPWSTR - must deallocate it later
    LPWSTR widestr = new WCHAR[bufferlen + 1];

    MultiByteToWideChar ( CP_UTF8, 0, instr.c_str(), (int)instr.size(), widestr, bufferlen );

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

