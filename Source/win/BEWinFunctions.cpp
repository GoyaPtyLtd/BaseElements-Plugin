/*
 BEWinFunctions.cpp
 BaseElements Plug-in
	
 Copyright 2010-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#include <ShlObj.h>

#include <fcntl.h>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include <Poco/Util/WinRegistryKey.h>

#include "BEWinFunctions.h"
#include "BEWinIFileDialog.h"


using namespace std;


const bool IsUnicodeFormat ( const UINT format_id );
const wstring ClipboardFormatNameForID ( const UINT format_id );
const UINT ClipboardFormatIDForName ( const wstring& format_name );
const bool SafeOpenClipboard ( void );
const bool IsFileMakerClipboardType ( const wstring& atype );
const UINT32 ClipboardOffset ( const wstring& atype );


// functions & globals for the dialog callback

static int CALLBACK SelectFolderCallback ( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData );
LRESULT CALLBACK DialogCallback ( int nCode, WPARAM wParam, LPARAM lParam );

thread_local HHOOK g_window_hook;

thread_local wstring g_button1_name;
thread_local wstring g_button2_name;
thread_local wstring g_button3_name;

// globals for the progress dialog

#ifndef PROGDLG_MARQUEEPROGRESS
#define PROGDLG_MARQUEEPROGRESS  0x00000020
#endif

#ifndef PROGDLG_NOCANCEL
#define PROGDLG_NOCANCEL         0x00000040
#endif

thread_local IProgressDialog * progress_dialog;
thread_local DWORD progress_dialog_maximum;

// undocumented/private(?) system clipboard formats
thread_local UINT BE_CF_FileGroupDescriptorW;
thread_local UINT BE_CF_FileNameW;
thread_local UINT BE_CF_FileNameMapW;


void InitialiseForPlatform ( void )
{
	_set_fmode ( O_RDONLY | O_BINARY ); // open files as binary by default

	BE_CF_FileGroupDescriptorW = RegisterClipboardFormat ( CFSTR_FILEDESCRIPTORW );
	BE_CF_FileNameW = RegisterClipboardFormat ( CFSTR_FILENAMEW );
	BE_CF_FileNameMapW = RegisterClipboardFormat ( CFSTR_FILEDESCRIPTORW );
}


// dry... fmx::errcode is not large enough to hold all results from GetLastError()

const fmx::errcode GetLastErrorAsFMX ( void )
{
	return (fmx::errcode)GetLastError();
}


// clipbaord functions

const bool IsUnicodeFormat ( const UINT format_id )
{

	bool is_unicode = false;

	if (
		format_id == CF_UNICODETEXT ||
		format_id == BE_CF_FileGroupDescriptorW ||
		format_id == BE_CF_FileNameW ||
		format_id == BE_CF_FileNameMapW
		) {
			is_unicode = true;
	}

	return is_unicode;

}

// default, system formats
// http://msdn.microsoft.com/en-us/library/windows/desktop/ff729168(v=vs.85).aspx


const wstring ClipboardFormatNameForID ( const UINT format_id )
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



const UINT ClipboardFormatIDForName ( const wstring& name ) {
	
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
	} else if ( format_name == CFSTR_FILEDESCRIPTORW ) {
		format_id = BE_CF_FileGroupDescriptorW;
	} else if ( format_name == CFSTR_FILENAMEW ) {
		format_id = BE_CF_FileNameW;
	} else if ( format_name == CFSTR_FILEDESCRIPTORW ) {
		format_id = BE_CF_FileNameMapW;
	} else {
		format_id = RegisterClipboardFormat ( name.c_str() );
	}

	return format_id;

}
	
// transient access is denied errors (#5) can occur after setting the clipboard

const bool SafeOpenClipboard ( void )
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

	
const wstring ClipboardFormats ( void )
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

	return wstring ( format_list );
	
} // ClipboardFormats


/* 
 FileMaker clipboard types for Windows are of the form Mac-XMxx
 */

const bool IsFileMakerClipboardType ( const wstring& atype )
{
	return ( atype.find ( L"Mac-XM" ) == 0 );
}


/* 
 for FileMaker types the first 4 bytes on the clipboard is the size of the data on the clipboard
 */

const UINT32 ClipboardOffset ( const wstring& atype )
{

	if ( IsFileMakerClipboardType ( atype ) ) {
		return 4; // char not wchar_t
	} else {
		return 0;
	}

}


const string UTF8ClipboardData ( const wstring& atype )
{
	char * clipboard_data = NULL;

	UINT format_wanted = ClipboardFormatIDForName ( atype );
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
	UINT32 offset = ClipboardOffset ( atype );
	const string reply ( clipboard_data + offset );

	delete[] clipboard_data;

	return reply;

} // UTF8ClipboardData


const string WideClipboardData ( const wstring& atype )
{
	wchar_t * clipboard_data = NULL;

	UINT format_wanted = ClipboardFormatIDForName ( atype );

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
	const string reply ( utf8 );

	return reply;

} // WideClipboardData


const string ClipboardText ( const wstring& atype )
{
	string reply;

	if ( SafeOpenClipboard() ) {
		
		UINT format = ClipboardFormatIDForName ( atype );

		if ( IsClipboardFormatAvailable ( format ) ) {

			if ( IsUnicodeFormat ( format ) ) {
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

} // ClipboardText



HGLOBAL DataForClipboardAsUTF8 ( const string& data, const wstring& atype )
{
	SIZE_T data_size = data.size();

	/* 
	 prefix the data to place on the clipboard with the size of the data
	 */
	SIZE_T offset = ClipboardOffset ( atype );
	SIZE_T clipboard_size = data_size + offset;

	HGLOBAL clipboard_memory = GlobalAlloc ( GMEM_MOVEABLE, clipboard_size );
	unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( clipboard_memory );

	if ( IsFileMakerClipboardType ( atype ) == TRUE ) {
		memcpy_s ( clipboard_contents, offset, &data_size, offset );
	}

	memcpy_s ( clipboard_contents + offset, clipboard_size, data.c_str(), clipboard_size );

	GlobalUnlock ( clipboard_memory );

	return clipboard_memory;

}	//	DataForClipboardAsUTF8



void * DataForClipboardAsWide ( const string& data )
{
	wstring data_as_wide = utf8toutf16 ( data );

	SIZE_T clipboard_size = data_as_wide.size() + 1;
	SIZE_T memory_size = clipboard_size * sizeof ( wchar_t );

	HGLOBAL clipboard_memory = GlobalAlloc ( GMEM_MOVEABLE, memory_size );
	wchar_t * clipboard_contents = (wchar_t *)GlobalLock ( clipboard_memory );
	wmemcpy_s ( clipboard_contents, clipboard_size, data_as_wide.c_str(), clipboard_size );

	GlobalUnlock ( clipboard_memory );

	return clipboard_memory;

}	//	DataForClipboardAsWide



const bool SetClipboardText ( const string& data, const wstring& atype )
{
	bool ok = FALSE;
	
	if ( OpenClipboard ( GetActiveWindow() ) ) {
		
		if ( EmptyClipboard() ) {

			UINT format = ClipboardFormatIDForName ( atype );

			HGLOBAL clipboard_memory;

			if ( IsUnicodeFormat ( format ) ) {
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

}	//	Set_ClipboardText


const std::vector<unsigned char> ClipboardFile ( const std::wstring& atype )
{
	std::vector<unsigned char> reply;

	if ( SafeOpenClipboard() ) {

		UINT format = ClipboardFormatIDForName ( atype );

		if ( IsClipboardFormatAvailable ( format ) ) {

			const UINT format_wanted = ClipboardFormatIDForName ( atype );
			const HGLOBAL clipboard_memory = GetClipboardData ( format_wanted );

			const unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( clipboard_memory );
			const SIZE_T clipboard_size = GlobalSize ( clipboard_memory );
//			if ( clipboard_size > 0 ) { // keep the msvc debug runtime happy
				reply.assign ( clipboard_contents, clipboard_contents + clipboard_size );
//			}
			GlobalUnlock ( clipboard_memory );

		}

		if (CloseClipboard()) {
			g_last_error = GetLastErrorAsFMX();
		}

	}

	return reply;

} // ClipboardFile


const bool SetClipboardFile ( const std::vector<unsigned char>& data, const std::wstring& atype )
{
	bool ok = FALSE;

	if ( OpenClipboard ( GetActiveWindow() ) ) {

		if ( EmptyClipboard() ) {

			const UINT format = ClipboardFormatIDForName ( atype );
			const HGLOBAL clipboard_memory = GlobalAlloc ( GMEM_MOVEABLE, data.size() );

			unsigned char * clipboard_contents = (unsigned char *)GlobalLock ( clipboard_memory );
			memcpy_s (  clipboard_contents, data.size(), data.data(), data.size() );
			GlobalUnlock ( clipboard_memory );

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

} // SetClipboardFile



// file dialogs


const wstring SelectFile ( const wstring& prompt, const wstring& in_folder )
{
	BEWinIFileOpenDialogAutoPtr file_dialog ( new BEWinIFileOpenDialog ( prompt, in_folder ) );
	const wstring selected_files ( file_dialog->Show ( ) );

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


const wstring SelectFolder ( const wstring& prompt, const wstring& in_folder )
{
	BROWSEINFO browse_info = { 0 };
	browse_info.hwndOwner = GetActiveWindow();
	browse_info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    browse_info.lpszTitle = prompt.c_str();
	if ( !in_folder.empty() ) {
		browse_info.lpfn = SelectFolderCallback;
		browse_info.lParam = (LPARAM)in_folder.c_str();
	}

    LPITEMIDLIST item_list = SHBrowseForFolder ( &browse_info );
	
	// if the user hasn't cancelled the dialog get the path to the folder

    wchar_t path[PATH_MAX] = L"";

	if ( item_list != 0 ) {
		SHGetPathFromIDList ( item_list, path );
	}

	return wstring ( path );

}	//	SelectFolder


const wstring SaveFileDialog ( const wstring& prompt, const wstring& file_name, const wstring& in_folder )
{
	BEWinIFileSaveDialogAutoPtr file_dialog ( new BEWinIFileSaveDialog ( prompt, file_name, in_folder ) );
	const wstring save_file_as ( file_dialog->Show ( ) );

	return save_file_as;

} // SaveFileDialog


// (customized) alert dialogs

const int DisplayDialog ( const wstring& title, const wstring& message, const wstring& button1, const wstring& button2, const wstring& button3 )
{
	// set the names to be used for each button
	g_button1_name = button1;
	g_button2_name = button2;
	g_button3_name = button3;

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

	int button_clicked = MessageBox ( GetForegroundWindow(), message.c_str(), title.c_str(), type );

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


const fmx::errcode DisplayProgressDialog ( const wstring& title, const wstring& description, const long maximum, const bool can_cancel )
{
	HRESULT result = S_OK;

	if ( progress_dialog ) {
		result = kFileOrObjectIsInUse;
	} else {

		progress_dialog_maximum = maximum;

		HWND parent_window = GetForegroundWindow();
		result = CoCreateInstance ( CLSID_ProgressDialog, NULL, CLSCTX_INPROC_SERVER, IID_IProgressDialog, (void**)&progress_dialog );

		if ( result == S_OK ) {
			result = progress_dialog->SetTitle ( title.c_str () );
		}

		if ( result == S_OK ) {
			result = progress_dialog->SetLine ( 2, description.c_str (), false, NULL );
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

	}

	return (fmx::errcode)result;

} // DisplayProgressDialog


const fmx::errcode UpdateProgressDialog ( const unsigned long value, const wstring& description )
{
	fmx::errcode error = kNoError;

	if ( progress_dialog ) {

		BOOL user_cancelled = progress_dialog->HasUserCancelled();

		if ( (user_cancelled != 0) || (value > progress_dialog_maximum) ) {

			progress_dialog->StopProgressDialog();
			progress_dialog->Release();
		    progress_dialog = NULL;

			error = user_cancelled ? kUserCancelledError : error;

		} else {

			HRESULT result;

			if ( !description.empty () ) {
				result = progress_dialog->SetLine ( 2, description.c_str (), false, NULL );
			}

			if ( result == S_OK ) {
				result = progress_dialog->SetProgress ( value, progress_dialog_maximum );
				error = result == S_FALSE ? kNoError : (fmx::errcode)result;
			}

		}

	} else {
		error = kWindowIsMissingError;
	}

	return error;
}



#pragma mark -
#pragma mark User Preferences
#pragma mark -


const bool SetPreference ( const string& key, const string& value, const string& domain )
{

	Poco::Util::WinRegistryKey registry_key ( HKEY_CURRENT_USER, domain );
	registry_key.setString ( key, value );

	return true;

} // SetPreference


const string GetPreference ( const string& key, const string& domain )
{
	Poco::Util::WinRegistryKey registry_key ( HKEY_CURRENT_USER, domain );

	std::string value;

	if ( registry_key.exists() ) {

		try {
			value = registry_key.getString(key);
		}
		catch ( Poco::NotFoundException& /* e */ ) {
			; // do nothing we happy to return an empty string
		}

	}

	return value;

} // GetPreference


void DeletePreference ( const string& key, const string& domain )
{
	Poco::Util::WinRegistryKey registry_key ( HKEY_CURRENT_USER, domain );

	if ( registry_key.exists() ) {

		try {
			registry_key.deleteValue ( key );
		}
		catch ( Poco::NotFoundException& /* e */ ) {
			; // don't worry, be happy
		}

	}

} // DeletePreference



#pragma mark -
#pragma mark Other
#pragma mark -


const bool OpenURL ( const wstring& url )
{

	HINSTANCE result = ShellExecute ( NULL, (LPCWSTR)L"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );

	// see http://msdn.microsoft.com/en-us/library/bb762153(VS.85).aspx

	return ( result > (HINSTANCE)32 );
}


const bool OpenFile ( const wstring& path )
{
	return OpenURL ( path );
}


const std::wstring get_machine_name ()
{
	const int BUFFER_SIZE = MAX_COMPUTERNAME_LENGTH + 1;
	wchar_t buffer[BUFFER_SIZE] = L"";
	DWORD size = BUFFER_SIZE;

	GetComputerNameEx ( ComputerNamePhysicalNetBIOS, buffer, &size );

	return buffer;
}


const std::string get_system_drive ( )
{
	char system_directory[PATH_MAX];
	const UINT length = GetSystemDirectoryA ( (LPSTR)system_directory, PATH_MAX );
	auto system_path = boost::filesystem::path ( system_directory );
	auto system_drive = system_path.root_name();

	return "/" + system_drive.string() + "/";
}


#pragma mark -
#pragma mark Utilities
#pragma mark -

const wstring utf8toutf16 ( const string& instr )
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


const string utf16ToUTF8 ( const wstring& s )
{
    const int size = ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL );

    vector<char> buf( size );
    ::WideCharToMultiByte( CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL );

    return string( &buf[0] );
}


const unsigned long Sub_LoadString ( unsigned long stringID, FMX_Unichar* intoHere, long resultsize )
{
	unsigned long returnResult = 0;

	LoadStringW((HINSTANCE)(gFMX_ExternCallPtr->instanceID), (unsigned int)stringID, (LPWSTR)intoHere, (fmx::uint32)resultsize);

	if (kFMXT_AppConfigStr == stringID || PLUGIN_DESCRIPTION_STRING_ID == stringID) {
		std::wstring plugin_description_string = (LPWSTR)intoHere;
		plugin_description_string.replace(plugin_description_string.find(L"%@"), 2, WSTRING(VERSION_STRING));
		boost::replace_all(plugin_description_string, L"\n", L"\r\n");
		plugin_description_string.copy((WCHAR*)intoHere, plugin_description_string.length(), 0);
		intoHere[plugin_description_string.length()] = '\0';
	}

	return returnResult;

} // Sub_LoadString

