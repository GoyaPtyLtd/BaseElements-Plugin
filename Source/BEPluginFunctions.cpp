/*
 BEPluginFunctions.cpp
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEPluginGlobalDefines.h"
#include "BEPluginFunctions.h"
#include "BEXSLT.h"
#include "BEWStringVector.h"
#include "BECurl.h"
#include "BEMessageDigest.h"
#include "BEFileSystem.h"
#include "BEShell.h"


#if defined(FMX_WIN_TARGET)

	#include "afxdlgs.h"
	#include <locale.h>

	#include "resource.h"
	#include "BEWinFunctions.h"

#endif

#if defined(FMX_MAC_TARGET)

	#include "BEMacFunctions.h"

#endif

#include "FMWrapper/FMXFixPt.h"
#include "FMWrapper/FMXData.h"
#include "FMWrapper/FMXBinaryData.h"
#include "FMWrapper/FMXCalcEngine.h"

#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/thread.hpp"


#include <iostream>


using namespace std;
using namespace boost::filesystem;


#pragma mark -
#pragma mark Globals
#pragma mark -

errcode g_last_error;


#pragma mark -
#pragma mark Version
#pragma mark -

FMX_PROC(errcode) BE_Version ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* data_vect */, Data& results)
{
	return TextConstantFunction ( VERSION_NUMBER_STRING, results );	
}


FMX_PROC(errcode) BE_VersionAutoUpdate ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* data_vect */, Data& results)
{
	return TextConstantFunction ( AUTO_UPDATE_VERSION, results );		
}


#pragma mark -
#pragma mark Errors
#pragma mark -


/*
 BE_GetURL & BE_ExecuteShellCommand are the only functions that populates g_last_error, calling it after any other function 
 will give an undefined result.
 
 g_last_error is cleared after calling this function... the caller should store it if necessary
 */

FMX_PROC(errcode) BE_GetLastError ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* data_vect */, Data& results)
{
	errcode error_result = kNoError;
	
	SetNumericResult ( g_last_error, results );
	g_last_error = kNoError;

	return error_result;
	
}



#pragma mark -
#pragma mark Clipboard
#pragma mark -

FMX_PROC(errcode) BE_ClipboardFormats ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* data_vect */, Data& results)
{
	return TextConstantFunction ( ClipboardFormats(), results );	
}


FMX_PROC(errcode) BE_ClipboardData ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {
		
		WStringAutoPtr atype = ParameterAsWideString ( data_vect, 0 );
		StringAutoPtr clipboard_contents = ClipboardData ( atype );
		SetUTF8Result ( clipboard_contents, results );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_ClipboardData


FMX_PROC(errcode) BE_SetClipboardData ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {

		StringAutoPtr to_copy = ParameterAsUTF8String ( data_vect, 0 );
		WStringAutoPtr atype = ParameterAsWideString ( data_vect, 1 );
		bool success = SetClipboardData ( to_copy, atype );
		SetNumericResult ( success, results );

	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_SetClipboardData


#pragma mark -
#pragma mark Files & Folders
#pragma mark -


#pragma NOTE (consider refatoring some of the detail from the file functions into BEFileSystem)


FMX_PROC(errcode) BE_CreateFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results )
{
	errcode error_result = kNoError;	
	errcode filesystem_result = kNoError;
	
	try {

		WStringAutoPtr folder = ParameterAsWideString ( data_vect, 0 );
		path directory_path = *folder;
		
		try {
			create_directory ( directory_path );
		} catch ( filesystem_error e ) {
			filesystem_result = e.code().value();
		}
		
		SetNumericResult ( filesystem_result, results );
				
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_CreateFolder


FMX_PROC(errcode) BE_DeleteFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	errcode filesystem_result = kNoError;
	
	try {
		
		WStringAutoPtr file = ParameterAsWideString ( data_vect, 0 );
		path path = *file;
		
		try {
			remove_all ( path ); // if path is a directory then path and all it's contents are deleted
		} catch ( filesystem_error e ) {
			filesystem_result = e.code().value();
		}
		
		SetNumericResult ( filesystem_result, results );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_DeleteFile


FMX_PROC(errcode) BE_FileExists ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {
		
		WStringAutoPtr file = ParameterAsWideString ( data_vect, 0 );
		path path = *file;

		bool file_exists = exists ( path );
		SetNumericResult ( file_exists, results );
		
	} catch ( filesystem_error e ) {
		error_result = e.code().value();
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_FileExists


FMX_PROC(errcode) BE_ReadTextFromFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {

		WStringAutoPtr file = ParameterAsWideString ( data_vect, 0 );
		StringAutoPtr contents = ReadFileAsUTF8 ( file );
		SetUTF8Result ( contents, results );

	} catch ( filesystem_error e ) {
		error_result = e.code().value();
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_ReadTextFromFile


FMX_PROC(errcode) BE_WriteTextToFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
		
	try {
		
		WStringAutoPtr file = ParameterAsWideString ( data_vect, 0 );
		path path = *file;
		
		// should the text be appended to the file or replace any existing contents
		
		ios_base::openmode mode = ios_base::trunc;
		if ( data_vect.Size() == 3 ) {
			bool append = data_vect.AtAsBoolean ( 2 );
			if ( append ) {
				mode = ios_base::app;
			}
		}
		
		StringAutoPtr text_to_write = ParameterAsUTF8String ( data_vect, 1 );
		
		try {
			
			boost::filesystem::path filesystem_path ( path );
			boost::filesystem::ofstream output_file ( filesystem_path, ios_base::out | mode );

			output_file.exceptions ( boost::filesystem::ofstream::badbit | boost::filesystem::ofstream::failbit );			
			
			output_file << *text_to_write;
			output_file.close();

		} catch ( filesystem_error e ) {
			error_result = e.code().value();
		} catch ( exception e ) {
			error_result = errno; // unable to write to the file
		}
		
		SetNumericResult ( error_result, results );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_WriteTextToFile


/*
 filemaker can create DDRs that contains utf-16 characters that are not
 valid in an XML document. reads the DDR and writes out a new one, skipping
 any invalid characters, and replaces the old file
 */

FMX_PROC(errcode) BE_StripInvalidUTF16CharactersFromXMLFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {
		
		WStringAutoPtr file = ParameterAsWideString ( data_vect, 0 );
		path source = *file;

		wstring output_path = *file + TEMPORARY_FILE_SUFFIX;
		path destination = output_path;
		boost::uintmax_t length = file_size ( source ); // throws if the source does not exist
		
		boost::filesystem::ifstream input_file ( source, ios_base::in | ios_base::binary | ios_base::ate );
		input_file.seekg ( 0, ios::beg );
		boost::filesystem::ofstream output_file ( destination, ios_base::out | ios_base::binary | ios_base::ate );

		const size_t size = 2; // read (and write) 2 bytes at a time
		boost::uintmax_t skipped = 0;
		bool big_endian = true;

		for ( boost::uintmax_t i = 0; i < length; i += size ) {

			char codepoint[size];
			input_file.read ( codepoint, size );
			
			// check the bom, if present, to determin if the utf-16 if big or little endian
			if ( (i == 0) && ((unsigned char)codepoint[0] == 0xff && (unsigned char)codepoint[1] == 0xfe ) ) {
				big_endian = false;
			}
			
			// swap the byte order for big-endian files
			unichar * utf16 = (unichar *)codepoint;
			char byte_swapped[size];
			if ( big_endian ) {
				byte_swapped[0] = codepoint[1];
				byte_swapped[1] = codepoint[0];
				utf16 = (unichar *)byte_swapped;
			}
			
			// only check codepoints in the bmp (so no 4-byte codepoints)
			if ( (*utf16 == 0x9) ||
				(*utf16 == 0xA) ||
				(*utf16 == 0xD) ||
				((*utf16 >= 0x20) && (*utf16 <= 0xD7FF)) ||
				((*utf16 >= 0xE000) && (*utf16 <= 0xFFFE)) ) {
				
				output_file.write ( codepoint, size );

			} else {
				skipped += size;
			}
		}

		output_file.close();
		input_file.close ();
		
		/*
		 only replace the file if that we've skipped some characters and
		 the output file is the right size
		 */
		
		if ( (skipped > 0) && (length == (file_size ( destination ) + skipped)) ) {
			remove_all ( source );
			rename ( destination, source );
		} else {
			remove_all ( destination );
			if ( skipped > 0 ) {
				// if characters were skipped and the file size is wrong report an error
				error_result = kFileSystemError;
			}
		}

		SetNumericResult ( error_result == kNoError, results );
		
	} catch ( filesystem_error e ) {
		error_result = e.code().value();
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_StripInvalidUTF16CharactersFromXMLFile


FMX_PROC(errcode) BE_MoveFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	errcode filesystem_result = kNoError;
	
	try {
		
		WStringAutoPtr from = ParameterAsWideString ( data_vect, 0 );
		WStringAutoPtr to = ParameterAsWideString ( data_vect, 1 );

		try {
			
			path from_path = *from;
			path to_path = *to;
			
			rename ( from_path, to_path );			
		} catch ( filesystem_error e ) {
			filesystem_result = e.code().value();
		}
		
		SetNumericResult ( filesystem_result, results );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_MoveFile


FMX_PROC(errcode) BE_CopyFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	g_last_error = kNoError;
	errcode filesystem_result = kNoError;
	
	try {
		
		WStringAutoPtr from = ParameterAsWideString ( data_vect, 0 );
		WStringAutoPtr to = ParameterAsWideString ( data_vect, 1 );
		
		try {
			
			path from_path = *from;
			path to_path = *to;
			
			recursive_directory_copy ( from_path, to_path );
			
		} catch ( filesystem_error e ) {
			filesystem_result = e.code().value();
		}
		
		SetNumericResult ( filesystem_result, results );
		
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}
	
	return g_last_error;
	
} // BE_CopyFile



FMX_PROC(errcode) BE_ListFilesInFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	g_last_error = kNoError;
	
	try {
		
		TextAutoPtr list_of_files;
		TextAutoPtr end_of_line;
		end_of_line->Assign ( "\r" );

		WStringAutoPtr directory = ParameterAsWideString ( data_vect, 0 );

		try {

			path directory_path = *directory;
			
			if ( exists ( directory_path ) ) {
				
				directory_iterator end_itr; // default construction yields past-the-end
				directory_iterator itr ( directory_path );

				long want = ParameterAsLong ( data_vect, 1, kBE_FileType_File );
				
				while ( itr != end_itr ) {
					
					bool is_folder = is_directory ( itr->status() );
				
					if ( 
							(!is_folder && (want == kBE_FileType_File)) ||
							(is_folder && (want == kBE_FileType_Folder)) ||
							(want == kBE_FileType_ALL)
						) {
						TextAutoPtr file_name;
						file_name->AssignWide ( itr->path().filename().wstring().c_str() );
						list_of_files->AppendText ( *file_name );
						list_of_files->AppendText ( *end_of_line );
					}
					
					++itr;
					
				}

			}

		} catch ( filesystem_error e ) {
			g_last_error = e.code().value();
		}

		fmx::LocaleAutoPtr default_locale;
		results.SetAsText ( *list_of_files, *default_locale );
		
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}
	
	return g_last_error;
	
} // BE_ListFilesInFolder



#pragma mark -
#pragma mark Dialogs
#pragma mark -

FMX_PROC(errcode) BE_SelectFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {

		WStringAutoPtr prompt = ParameterAsWideString ( data_vect, 0 );
		WStringAutoPtr file = SelectFile ( prompt );
		SetWideResult ( file, results );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_SelectFile


FMX_PROC(errcode) BE_SelectFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {

		WStringAutoPtr prompt = ParameterAsWideString ( data_vect, 0 );
		WStringAutoPtr folder = SelectFolder ( prompt );
		SetWideResult ( folder, results );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}

	return error_result;
	
} // BE_SelectFolder


FMX_PROC(errcode) BE_DisplayDialog ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
		
	try {

		WStringAutoPtr title = ParameterAsWideString ( data_vect, 0 );
		WStringAutoPtr message = ParameterAsWideString ( data_vect, 1 );
		WStringAutoPtr ok_button = ParameterAsWideString ( data_vect, 2 );
		WStringAutoPtr cancel_button = ParameterAsWideString ( data_vect, 3 );
		WStringAutoPtr alternate_button = ParameterAsWideString ( data_vect, 4 );
	
		int response = DisplayDialog ( title, message, ok_button, cancel_button, alternate_button );
		SetNumericResult ( response, results );

	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}

	return error_result;
	
} // BE_DisplayDialog


#pragma mark -
#pragma mark XSLT
#pragma mark -

FMX_PROC(errcode) BE_ApplyXSLT ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {

		StringAutoPtr xml_path = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr xslt = ParameterAsUTF8String ( data_vect, 1 );
		StringAutoPtr csv_path = ParameterAsUTF8String ( data_vect, 2 );

		results.SetAsText( *ApplyXSLT ( xml_path, xslt, csv_path ), data_vect.At(0).GetLocale() );
	
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}

	return error_result;
	
} // BE_ApplyXSLT


FMX_PROC(errcode) BE_ApplyXSLTInMemory ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {
		
		StringAutoPtr xml = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr xslt = ParameterAsUTF8String ( data_vect, 1 );
		
		results.SetAsText( *ApplyXSLTInMemory ( xml, xslt ), data_vect.At(0).GetLocale() );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_ApplyXSLTInMemory


FMX_PROC(errcode) BE_XPath ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results )
{
	errcode error_result = kNoError;
	
	try {
		int numParams = data_vect.Size();
		StringAutoPtr xml = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr xpath = ParameterAsUTF8String ( data_vect, 1 );
		StringAutoPtr nsList(new string);
		if (numParams > 2)
			nsList = ParameterAsUTF8String ( data_vect, 2 );
		
		results.SetAsText( *ApplyXPath ( xml, xpath, nsList ), data_vect.At(0).GetLocale() );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_XPath


FMX_PROC(errcode) BE_XPathAll ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results )
{
	errcode error_result = kNoError;
	TextAutoPtr txt;
	
	try {
		int numParams = data_vect.Size();
		StringAutoPtr xml = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr xpath = ParameterAsUTF8String ( data_vect, 1 );
		StringAutoPtr nsList( new string);
		if (numParams > 2)
			nsList = ParameterAsUTF8String ( data_vect, 2 );
		
		
		results.SetAsText(*ApplyXPathAll (xml, xpath, nsList), data_vect.At(0).GetLocale() );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_XPathAll



#pragma mark -
#pragma mark User Preferences
#pragma mark -


FMX_PROC(errcode) BE_SetPreference ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results)
{
	g_last_error = kNoError;
	
	try {
		
		WStringAutoPtr key = ParameterAsWideString ( parameters, 0 );
		WStringAutoPtr value = ParameterAsWideString ( parameters, 1 );
		WStringAutoPtr domain = ParameterAsWideString ( parameters, 2 );
		
		if ( domain->empty() ) {
			domain->assign ( USER_PREFERENCES_DOMAIN );
		}

		SetNumericResult ( SetPreference ( key, value, domain ), results );
		
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}
	
	return g_last_error;
	
} // BE_SetPreference



FMX_PROC(errcode) BE_GetPreference ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results)
{
	g_last_error = kNoError;
	
	try {
		
		WStringAutoPtr key = ParameterAsWideString ( parameters, 0 );
		WStringAutoPtr domain = ParameterAsWideString ( parameters, 1 );
		
		if ( domain->empty() ) {
			domain->assign ( USER_PREFERENCES_DOMAIN );
		}
		
		SetWideResult ( GetPreference ( key, domain ), results );
		
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}
	
	return g_last_error;
	
} // BE_GetPreference






#pragma mark -
#pragma mark Other / Ungrouped
#pragma mark -


/* 
 
 invoked for multiple plug-in functions... funcId is used to determine which one
 
 constants should be defined in BEPluginGlobalDefines.h
 
 each set of constants should have it's own range [ 1000 then 2000 then 3000 etc. ]
 with an offset of x000
 
*/

FMX_PROC(errcode) BE_NumericConstants ( short funcId, const ExprEnv& /* environment */, const DataVect& /* data_vect */, Data& results)
{
	g_last_error = kNoError;
	
	try {
		
		SetNumericResult ( funcId % kBE_NumericConstantOffset, results );
		
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}
	
	return g_last_error;
	
} // BE_NumericConstants


/*
 BE_ExtractScriptVariables implements are somewhat imperfect heuristic for finding
 script variables within chunks of filemaker calculation
 
 try to stip out unwanted text such as strings and comments and then, when a $ is
 found, attempt to guess the where the variable name ends
 */

FMX_PROC(errcode) BE_ExtractScriptVariables ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {
		
		BEWStringVector variables;
		WStringAutoPtr calculation = ParameterAsWideString ( data_vect, 0 );

		wstring search_for = L"$/\""; // variables, comments and strings (including escaped strings)
		size_t found = calculation->find_first_of ( search_for );

		while ( found != wstring::npos )
		{
			size_t end = 0;
			size_t search_from = found + 1;
									
			switch ( calculation->at ( found ) ) {
				case L'$': // variables
				{
					/*
					 find the end of the variable
						+ - * / ^ & = ≠ < > ≤ ≥ ( , ; ) [ ] " :: $ }
					 unicode escapes are required on Windows
					 */
					
					end = calculation->find_first_of ( L" ;+-=*/&^<>\t\r[]()\u2260\u2264\u2265,", search_from );
					if ( end == wstring::npos ) {
						end = calculation->size();
					}

					// add the variable to the list
					wstring wanted = calculation->substr ( found, end - found );
					variables.PushBack ( wanted );
					search_from = end + 1;
				}
				break;
					
				case L'/': // comments
					switch ( calculation->at ( search_from ) ) {
						case L'/':
							end = calculation->find ( L"\r", search_from );
							search_from = end + 1;
							break;
							
						case L'*':
							end = calculation->find ( L"*/", search_from );
							search_from = end + 2;
							break;
							
						default:
							break;
					}
					break;
					
				case L'\"': // escaped strings
					end = calculation->find ( L"\"", search_from );
					while ( (end != string::npos) && (calculation->at ( end - 1 ) == L'\\') ) {
						end = calculation->find ( L"\"", end + 1 );
					}
					search_from = end + 1;
					break;
					
//				default:
			}
			
			// this is not on an eternal quest
			if ( (end != string::npos) && (search_from < calculation->size()) ) { 
				found = calculation->find_first_of ( search_for, search_from );
			} else {
				found = string::npos;
			}
		}
		
		results.SetAsText( *(variables.AsValueList()), data_vect.At(0).GetLocale() );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_ExtractScriptVariables



FMX_PROC(errcode) BE_ExecuteShellCommand ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	g_last_error = kNoError;
	
	try {
		
		StringAutoPtr command = ParameterAsUTF8String ( data_vect, 0 );
		bool waitForResponse = ParameterAsBoolean ( data_vect, 1 );

		StringAutoPtr response ( new string ( ) );

		if ( waitForResponse ) {
			g_last_error = ExecuteShellCommand ( *command, *response );
		} else {
			boost::thread dontWaitForThis ( ExecuteShellCommand, *command, *response );
		}

		SetUTF8Result ( response, results );
		
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}
	
	return g_last_error;
	
} // BE_ExecuteShellCommand


/*
 a wrapper for the FileMaker SQL calls FileMaker_Tables and FileMaker_Fields
 
 under FileMaker 11 the functions return
 
 FileMaker_Tables - returns a list of TOs with associated information
	table occurance name
	table occurance id
	table name
	file name
	schema modification count 

 FileMaker_Fields - returns a list of fields...
	table occurance name
	name
	type
	id
	class
	repitions
	modification count 

 Note: 
	For FileMaker versions 8.5~10 a subset of this information is returned. 
	The functions do not exist in versions 7 & 8 and an error is returned.
 
 */

FMX_PROC(errcode) BE_FileMaker_TablesOrFields ( short funcId, const ExprEnv& environment, const DataVect& parameters, Data& reply )
{	
	fmx::errcode error_result = kNoError;
	
	fmx::TextAutoPtr expression;

	try {

		if ( funcId == kBE_FileMaker_Tables ) {
			expression->Assign ( "SELECT * FROM FileMaker_Tables" );
		} else {
			expression->Assign ( "SELECT * FROM FileMaker_Fields" );
		}
		
		// the original api best suits the purpose
		error_result = environment.ExecuteSQL ( *expression, reply, '\t', '\n' );
		
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}	
	
	return error_result;
	
} // BE_FileMaker_TablesOrFields


// open the supplied url in the user's default browser

FMX_PROC(errcode) BE_OpenURL ( short funcId, const ExprEnv& environment, const DataVect& parameters, Data& reply )
{	
	fmx::errcode error_result = kNoError;
	
	fmx::TextAutoPtr expression;
	
	try {
		
		WStringAutoPtr url = ParameterAsWideString ( parameters, 0 );
		bool succeeded = OpenURL ( url );

		SetNumericResult ( succeeded, reply );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}	
	
	return error_result;
	
} // BE_OpenURL



FMX_PROC(errcode) BE_ExecuteScript ( short /* funcId */, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& reply)
{
	fmx::errcode error_result = 0;
	
	try {
		
		fmx::TextAutoPtr script_name;
		script_name->SetText ( dataVect.AtAsText ( 0 ) );

		
		// use the current file when a file name is not provided
		
		fmx::TextAutoPtr file_name;
		fmx::DataAutoPtr parameter;

		fmx::ulong number_of_paramters = dataVect.Size();
		
		if ( number_of_paramters >= 2 ) {
			file_name->SetText ( dataVect.AtAsText ( 1 ) );
		} else {
			fmx::TextAutoPtr command;
			command->Assign ( "Get ( FileName )" );

			fmx::DataAutoPtr name;
			environment.Evaluate ( *command, *name );
			file_name->SetText ( name->GetAsText() );
		}

		// get the parameter, if present
		
		if ( number_of_paramters == 3 ) {
			fmx::LocaleAutoPtr default_locale;
			parameter->SetAsText ( dataVect.AtAsText ( 2 ), *default_locale );
		}
		
		error_result = FMX_StartScript ( &(*file_name), &(*script_name), kFMXT_Pause, &(*parameter) );
		
		SetNumericResult ( error_result, reply );

	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}	
	
	return error_result;
	
} // BE_ExecuteScript


FMX_PROC(errcode) BE_FileMakerSQL ( short /* funcId */, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results )
{	
	g_last_error = kNoError;
	
	TextAutoPtr expression;
	
	try {
		
		expression->SetText ( dataVect.AtAsText ( 0 ) );
		
		// use tab and return when the separators are not provided
		
		TextAutoPtr columnSeparator;
		
		ulong number_of_paramters = dataVect.Size();
		
		if ( number_of_paramters >= 2 ) {
			columnSeparator->SetText ( dataVect.AtAsText(1) );
		} else {
			columnSeparator->Assign ( "\t" );
		}
		
		
		TextAutoPtr rowSeparator;
		
		if ( number_of_paramters >= 3 ) {
			rowSeparator->SetText ( dataVect.AtAsText(2) );
		} else {
			rowSeparator->Assign ( "\r" );
		}
		
		const TextAutoPtr filename;
		
		if ( number_of_paramters == 4 ) {
			filename->SetText ( dataVect.AtAsText(3) );
		} else {
			filename->Assign ( "" ); // the current file
		}
		
		const DataVectAutoPtr parameters;
		RowVectAutoPtr sqlResult;
		
		g_last_error = environment.ExecuteFileSQL ( *expression, *filename, *parameters, *sqlResult );
		
        TextAutoPtr	textResult;
		
		ulong numberOfRows = sqlResult->Size();
		ulong lastRow = numberOfRows - 1;
		
        for ( ulong row = 0; row < numberOfRows; ++row ) { 
			
			const DataVect &thisRow = sqlResult->At( row ); 
			ulong numberOfColumns = thisRow.Size();
			ulong lastColumn = numberOfColumns - 1;
			
			for ( ulong column = 0; column < numberOfColumns; ++column ) {
				// should escape this ??? the old API doesn't so leave it to the FM developers
				textResult->AppendText ( thisRow.At( column ).GetAsText() );
				// skip last time
				if ( column != lastColumn ) {
					textResult->AppendText( *columnSeparator );
				}
			}
			// skip last time
			if ( row != lastRow ) {
				textResult->AppendText ( *rowSeparator );
			}
        } 
		
		results.SetAsText( *textResult, dataVect.At( 0 ).GetLocale() );		
		
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}	
	
	return g_last_error;
	
} // BE_FileMakerSQL




FMX_PROC(errcode) BE_GetURL ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results )
{	
	g_last_error = kNoError;
		
	try {
		
		StringAutoPtr url = ParameterAsUTF8String ( dataVect, 0 );
		StringAutoPtr filename ( new string ( "" ) );
		StringAutoPtr username = ParameterAsUTF8String ( dataVect, 1 );
		StringAutoPtr password = ParameterAsUTF8String ( dataVect, 2 );
		
		vector<char> data = GetURL ( url, filename, username, password );

		// if a file name is supplied then send back a file

		if ( dataVect.Size() > 1 ) {

			TextAutoPtr name;
			name->SetText ( dataVect.AtAsText(1) );

			BinaryDataAutoPtr resultBinary; 
			QuadCharAutoPtr data_type ( 'F', 'I', 'L', 'E' ); 
			resultBinary->AddFNAMData ( *name ); 
			resultBinary->Add ( *data_type, (unsigned long) data.size(), (void *)&data[0] ); 
			results.SetBinaryData ( *resultBinary, true ); 

		} else { // otherwise try sending back text
			
			data.push_back ( '\0' );
			StringAutoPtr data_string ( new string ( &data[0] ) );
			SetUTF8Result ( data_string, results );

		}		
		
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}
	
	return g_last_error;
	
} // BE_GetURL



FMX_PROC(errcode) BE_SaveURLToFile ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results )
{	
	g_last_error = kNoError;
	
	try {
		
		StringAutoPtr url = ParameterAsUTF8String ( dataVect, 0 );
		StringAutoPtr filename = ParameterAsUTF8String ( dataVect, 1 );
		StringAutoPtr username = ParameterAsUTF8String ( dataVect, 2 );
		StringAutoPtr password = ParameterAsUTF8String ( dataVect, 3 );
		
		vector<char> data = GetURL ( url, filename, username, password );
				
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}
	
	return g_last_error;
	
} // BE_SaveURLToFile



FMX_PROC(errcode) BE_MessageDigest ( short funcId, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{	
	g_last_error = kNoError;
		
	try {
		
		StringAutoPtr message = ParameterAsUTF8String ( parameters, 0 );
		unsigned long type = ParameterAsLong( parameters, 1, kBE_MessageDigestType_SHA256 );

		StringAutoPtr digest;

		if ( type == kBE_MessageDigestType_MD5 ) {
			digest = MD5 ( message );
		} else { // the default is SHA256
			digest = SHA256 ( message );
		}
		
		SetUTF8Result ( digest, results );
		
		
	} catch ( bad_alloc e ) {
		g_last_error = kLowMemoryError;
	} catch ( exception e ) {
		g_last_error = kErrorUnknown;
	}	
	
	return g_last_error;
	
} // BE_FileMaker_TablesOrFields


