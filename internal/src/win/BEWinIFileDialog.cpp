/*
 BEWinIFileDialog.cpp
 BaseElements Plug-In
 
 Copyright 2014-2022 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEWinIFileDialog.h"
#include "BEValueList.h"

#include <iterator>
#include <string>
#include <sstream>


using namespace std;
using namespace fmx;


// necessary since c++ insist on using file_dialog from the parent class

template <typename T>
HRESULT _SetDefaultFolder ( T& file_dialog, const wstring in_folder )
{
	HRESULT hr = S_OK;

	// go to the recently used folder if we don't specify one
	if ( in_folder.empty() ) {
		hr = file_dialog->SetDefaultFolder ( NULL );
	} else {

		IShellItem * use_folder;
		hr = SHCreateItemFromParsingName ( in_folder.c_str(), NULL, IID_PPV_ARGS ( &use_folder) );
		if ( SUCCEEDED ( hr ) ) {
			hr = file_dialog->SetFolder ( use_folder );
		}

	}

	return hr;

} // SetDefaultFolder



#pragma mark -
#pragma mark BEWinIFileOpenDialog
#pragma mark -


BEWinIFileOpenDialog::BEWinIFileOpenDialog ( const std::wstring& dialog_prompt, const std::wstring& start_in_folder )
{
	prompt = dialog_prompt;
	in_folder = start_in_folder;

	dialog_type = CLSID_FileOpenDialog;
	dialog_flags = FOS_FORCEFILESYSTEM | FOS_ALLOWMULTISELECT | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOREADONLYRETURN;
}


// NOTE: this function is all but a copy/paste from BEWinIFileSaveDialog - if changing here check there aslo

const std::wstring BEWinIFileOpenDialog::Show ( )
{
	hr = S_OK;
	std::wstring selected_files;

	file_dialog = NULL;
	hr = CoCreateInstance ( dialog_type, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS ( &file_dialog ) );
    if ( SUCCEEDED ( hr ) ) {

		// Set the options on the dialog.
		DWORD existing_flags;
		hr = file_dialog->GetOptions ( &existing_flags ); // Before setting, always get the options first in order not to override existing options.
		if ( SUCCEEDED ( hr ) ) {

			hr = file_dialog->SetOptions ( existing_flags | dialog_flags );
			if ( SUCCEEDED ( hr ) ) {

				// go to the recently used folder if we don't specify one
				SetDefaultFolder ( );

				if ( SUCCEEDED ( hr ) ) {

					hr = file_dialog->SetTitle ( prompt.c_str() );
                    if ( SUCCEEDED ( hr ) ) {

						// Show the dialog
						hr = file_dialog->Show ( GetForegroundWindow() );
						if ( SUCCEEDED ( hr ) ) {
							selected_files = GetSelection ( );
						} else {
							CheckIfUserCancelled ( );
						}
					}
				}
			}
		}
	}

	file_dialog->Release();

	g_last_error = (fmx::errcode)hr;

	return selected_files;

}	//	SelectFile


void BEWinIFileOpenDialog::SetDefaultFolder ( )
{
	hr = _SetDefaultFolder ( file_dialog, in_folder );
}


const std::wstring BEWinIFileOpenDialog::GetSelection ( )
{
	hr = S_OK;

	unique_ptr< BEValueList<wstring> > selected_files ( new BEValueList<wstring>( ) );

	IShellItemArray * file_names;
	hr = file_dialog->GetResults ( &file_names );
	if ( SUCCEEDED ( hr ) ) {

		DWORD number_of_files_selected;
		hr = file_names->GetCount ( &number_of_files_selected );
		if ( SUCCEEDED ( hr ) ) {

			for ( DWORD j = 0; j < number_of_files_selected; j++ ) {

				IShellItem * next_file_name_item;
				hr = file_names->GetItemAt ( j, &next_file_name_item );
				if ( SUCCEEDED ( hr ) ) {

					LPOLESTR next_file_name = NULL;
					hr = next_file_name_item->GetDisplayName ( SIGDN_FILESYSPATH, &next_file_name ); // Get its file system path.
					if ( SUCCEEDED ( hr ) ) {
						selected_files->append ( next_file_name );
					}
				}
				next_file_name_item->Release();
			} // for
		}
		file_names->Release();
	}

	const std::wstring out ( selected_files->get_as_filemaker_string() );

	return out;

} // GetSelection
	

void BEWinIFileOpenDialog::CheckIfUserCancelled ( )
{
	if ( HRESULT_FROM_WIN32 ( ERROR_CANCELLED ) ) {
		hr = S_OK;
	}
}



#pragma mark -
#pragma mark BEWinIFileSaveDialog
#pragma mark -


BEWinIFileSaveDialog::BEWinIFileSaveDialog ( const std::wstring& dialog_prompt, const std::wstring& default_file_name, const std::wstring& start_in_folder )
{
	prompt = dialog_prompt;
	file_name = default_file_name;
	in_folder = start_in_folder;

	dialog_flags = FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOREADONLYRETURN;

	dialog_type = CLSID_FileSaveDialog;

}


// NOTE: this function is all but a copy/paste from BEWinIFileOpenDialog - if changing here check there aslo

const std::wstring BEWinIFileSaveDialog::Show ( )
{
	hr = S_OK;
	wstring selected_files;

	file_dialog = NULL;
	hr = CoCreateInstance ( dialog_type, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS ( &file_dialog ) );
    if ( SUCCEEDED ( hr ) ) {

		// Set the options on the dialog.
		DWORD existing_flags;
		hr = file_dialog->GetOptions ( &existing_flags ); // Before setting, always get the options first in order not to override existing options.
		if ( SUCCEEDED ( hr ) ) {

			hr = file_dialog->SetOptions ( existing_flags | dialog_flags );
			if ( SUCCEEDED ( hr ) ) {

				// go to the recently used folder if we don't specify one
				SetDefaultFolder ( );

				if ( SUCCEEDED ( hr ) ) {

					hr = file_dialog->SetTitle ( prompt.c_str() );
                    if ( SUCCEEDED ( hr ) ) {

						if ( ! file_name.empty() ) {
							hr = file_dialog->SetFileName ( file_name.c_str() );
						}

						if ( SUCCEEDED ( hr ) ) {

							// Show the dialog
							hr = file_dialog->Show ( GetForegroundWindow() );
							if ( SUCCEEDED ( hr ) ) {
								selected_files = GetSelection ( );
							} else {
								CheckIfUserCancelled ( );
							}
						}
					}
				}
			}
		}
	}

	file_dialog->Release();

	g_last_error = (fmx::errcode)hr;

	return selected_files;

}	//	Show


// NOTE: this function is copy/paste from BEWinIFileOpenDialog - since c++ insits on using the file_dialog in the parent class if we just inherit

void BEWinIFileSaveDialog::SetDefaultFolder ( )
{
	hr = _SetDefaultFolder ( file_dialog, in_folder );
}


const std::wstring BEWinIFileSaveDialog::GetSelection ( )
{
	hr = S_OK;

	IShellItem * new_file_name_item;
	wstring save_file_as;

	hr = file_dialog->GetResult ( &new_file_name_item );
	if ( SUCCEEDED ( hr ) ) {

		LPOLESTR new_file_name = NULL;
		hr = new_file_name_item->GetDisplayName ( SIGDN_FILESYSPATH, &new_file_name ); // Get its file system path
		if ( SUCCEEDED ( hr ) ) {
			save_file_as.assign ( new_file_name );
		}
	}

	new_file_name_item->Release();

	return save_file_as;

} // GetSelection
