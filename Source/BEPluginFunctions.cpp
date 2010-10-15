/*
 BEPluginFunctions.cpp
 BaseElements Plug-In
 
 Copyright 2010 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEPluginGlobalDefines.h"
#include "BEPluginFunctions.h"
#include "BEXSLT.h"
#include "BEWStringVector.h"


#if defined(FMX_WIN_TARGET)

	#include "afxdlgs.h"
	#include <locale.h>

	#include "resource.h"
	#include "BEWinFunctions.h"

	#define PATH_MAX MAX_PATH

#endif

#if defined(FMX_MAC_TARGET)

	#include "BEMacFunctions.h"

#endif

#include "FMWrapper/FMXFixPt.h"
#include "FMWrapper/FMXData.h"
#include "FMWrapper/FMXCalcEngine.h"

#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"

#include <iostream>


using namespace std;
using namespace boost::filesystem;


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
		
		StringAutoPtr atype = ParameterAsUTF8String ( data_vect, 0 );
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
		StringAutoPtr atype = ParameterAsUTF8String ( data_vect, 1 );
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

FMX_PROC(errcode) BE_CreateFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results )
{
	errcode error_result = kNoError;	
	errcode filesystem_result = kNoError;
	
	try {

		StringAutoPtr path = ParameterAsUTF8String ( data_vect, 0 );
		
		try {
			create_directory ( *path );
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
		
		StringAutoPtr path = ParameterAsUTF8String ( data_vect, 0 );
		
		try {
			remove_all ( *path ); // if path is a directory then path and all it's contents are deleted
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
	errcode filesystem_result = kNoError;
	
	try {
		
		StringAutoPtr path = ParameterAsUTF8String ( data_vect, 0 );
		bool file_exists = exists ( *path );
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

		StringAutoPtr path = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr contents = ReadFileAsUTF8 ( path );
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
		
		StringAutoPtr path = ParameterAsUTF8String ( data_vect, 0 );
		
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
			
			boost::filesystem::path filesystem_path ( *path );
			boost::filesystem::ofstream output_file ( filesystem_path, ios_base::out | mode );
			
			output_file << *text_to_write;
			output_file.close();
			
		} catch ( filesystem_error e ) {
			error_result = e.code().value();
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
		
		StringAutoPtr path = ParameterAsUTF8String ( data_vect, 0 );
		boost::filesystem::path source = *path;
		string output_path = *path + ".be3.tmp";
		boost::filesystem::path destination = output_path;
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
		
		StringAutoPtr from = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr to = ParameterAsUTF8String ( data_vect, 1 );
		
		try {
			
			boost::filesystem::path from_path = *from;
			boost::filesystem::path to_path = *to;
			
			rename ( from_path, to_path );			
		}
		catch ( filesystem_error e ) {
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
	errcode error_result = kNoError;
	errcode filesystem_result = kNoError;
	
	try {
		
		StringAutoPtr from = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr to = ParameterAsUTF8String ( data_vect, 1 );
		
		try {
			
			boost::filesystem::path from_path = *from;
			boost::filesystem::path to_path = *to;
		
			copy_file ( from_path, to_path );
			
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
	
} // BE_CopyFile


#pragma mark -
#pragma mark Dialogs
#pragma mark -

FMX_PROC(errcode) BE_SelectFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results)
{
	errcode error_result = kNoError;
	
	try {

		StringAutoPtr prompt = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr file = SelectFile ( prompt );
		SetUTF8Result ( file, results );
		
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

		StringAutoPtr prompt = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr folder = SelectFolder ( prompt );
		SetUTF8Result ( folder, results );
		
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

		StringAutoPtr title = ParameterAsUTF8String ( data_vect, 0 );
		StringAutoPtr message = ParameterAsUTF8String ( data_vect, 1 );
		StringAutoPtr ok_button = ParameterAsUTF8String ( data_vect, 2 );
		StringAutoPtr cancel_button = ParameterAsUTF8String ( data_vect, 3 );
		StringAutoPtr alternate_button = ParameterAsUTF8String ( data_vect, 4 );
	
		int response = DisplayDialog ( title, message, ok_button, cancel_button, alternate_button );
		SetNumericResult ( response, results );

	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}

	return error_result;
	
} // BE_DisplayDialog


// invoked for multiple plug-in functions... funcId is used to determine which one

FMX_PROC(errcode) BE_ButtonConstants ( short funcId, const ExprEnv& /* environment */, const DataVect& /* data_vect */, Data& results)
{
	errcode error_result = kNoError;
	
	try {
		
		SetNumericResult ( funcId - kXMpl_ButtonOffset, results );
		
	} catch ( bad_alloc e ) {
		error_result = kLowMemoryError;
	} catch ( exception e ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // BE_ButtonConstants


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


#pragma mark -
#pragma mark FileMaker Calculations
#pragma mark -

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
		WStringAutoPtr calculation = ParameterAsUnicodeString ( data_vect, 0 );

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


