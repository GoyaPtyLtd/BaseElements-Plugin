/*
 BEWinFileDialog.cpp
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEWinCommonFileDialog.h"
#include "BEValueList.h"

#include <ShObjIdl.h>

#include <iterator>
#include <string>
#include <sstream>


using namespace std;
using namespace fmx;



#pragma mark -
#pragma mark BEWinCommonFileOpenDialog
#pragma mark -


BEWinCommonFileOpenDialog::BEWinCommonFileOpenDialog ( WStringAutoPtr dialog_prompt, WStringAutoPtr start_in_folder )
{
    ZeroMemory ( &file_dialog, sizeof ( file_dialog ) );

	file_dialog.lStructSize = sizeof ( file_dialog ); 
	file_dialog.hwndOwner = GetForegroundWindow();
    file_dialog.lpstrFilter = (LPCWSTR)L"All Files (*.*)\0*.*\0";
	file_dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT;

	wcscpy_s ( cfg_prompt, PATH_MAX, dialog_prompt->c_str() );
	file_dialog.lpstrTitle = cfg_prompt;

	wcscpy_s ( cfg_in_folder, PATH_MAX, start_in_folder->c_str() );
	file_dialog.lpstrInitialDir = cfg_in_folder;

	path[0] = '\0';
	file_dialog.lpstrFile = path;

	file_dialog.nMaxFile = PATH_MAX;
}


WStringAutoPtr BEWinCommonFileOpenDialog::Show ( )
{
	file_dialog.lpstrFile = (LPWSTR)path;

	if ( GetOpenFileName ( &file_dialog ) == TRUE ) {
		wcscpy_s ( path, file_dialog.lpstrFile );
	 }

	// return the file paths as a value list

	wstringstream wss ( path );
	istream_iterator<wstring, wchar_t, std::char_traits<wchar_t> > begin ( wss );
	istream_iterator<wstring, wchar_t, std::char_traits<wchar_t> > end;
	vector<wstring> values ( begin, end );

	auto_ptr< BEValueList<wstring> > selected_files ( new BEValueList<wstring> ( ) );

	int number_of_files = values.size();

	// when there's only one we get the full path, when there are more the first item in the array is the diretory
	if ( number_of_files == 1 ) {
		selected_files->append ( values[0] );
	} else {

		for ( int i = 1 ; i < number_of_files ; i++ ) {
			selected_files->append ( values[0] +  values[i] );
		}

	}

	WStringAutoPtr out ( new wstring ( selected_files->get_as_filemaker_string() ) );
	return out;

} // Show



#pragma mark -
#pragma mark BEWinCommonFileSaveDialog
#pragma mark -


BEWinCommonFileSaveDialog::BEWinCommonFileSaveDialog ( WStringAutoPtr dialog_prompt, WStringAutoPtr default_file_name, WStringAutoPtr start_in_folder ) : BEWinCommonFileOpenDialog ( dialog_prompt, start_in_folder )
{
	wcscpy_s ( path, default_file_name->c_str() );
	file_dialog.lpstrFile = (LPWSTR)path;

	file_dialog.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;
}


WStringAutoPtr BEWinCommonFileSaveDialog::Show ( )
{
	WStringAutoPtr save_file_as ( new wstring );

	if ( GetSaveFileName ( &file_dialog ) == TRUE ) {
		save_file_as->assign ( file_dialog.lpstrFile );
	}

	return save_file_as;

} // SaveFileDialog
