/*
 BEPluginFunctions.cpp
 BaseElements Plug-In
 
 Copyright 2010-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEPluginGlobalDefines.h"


#if defined ( FMX_WIN_TARGET )

	#include <locale.h>
	#include "BEWinFunctions.h"

#endif

#if defined ( FMX_MAC_TARGET )

	#include "BEMacFunctions.h"

#endif


#include "BEPluginFunctions.h"

#include "BEXSLT.h"
#include "BEWStringVector.h"
#include "BECurl.h"
#include "BEFileSystem.h"
#include "BEFMS.h"
#include "BEShell.h"
#include "BEZlib.h"
#include "BESQLCommand.h"
#include "BEXMLReader.h"
#include "BETime.h"
#include "BEJSON.h"
#include "BEOAuth.h"
#include "BEValueList.h"
#include "BECurlOption.h"
#include "BEXMLTextReader.h"
#include "BEBase64.h"
#include "Crypto/BEMessageDigest.h"
#include "Crypto/BEOpenSSLAES.h"
#include "Crypto/BEX509.h"
#include "BEPluginException.h"
#include "BEQuadChar.h"
#include "BEXero.h"
#include "BESMTP.h"
#include "BEJavaScript.h"
#include "Images/BEJPEG.h"
#include "BERegularExpression.h"
#include "BESMTPContainerAttachments.h"

#include <numeric> // for inner_product
#include <list>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-pedantic"
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wshadow"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/date_time/c_local_time_adjustor.hpp>

#pragma clang diagnostic pop

#include <Poco/Exception.h>
#include <Poco/RegularExpression.h>
#include <Poco/String.h>

#include <iconv.h>


using namespace std;
using namespace fmx;
using namespace boost::filesystem;


#pragma mark -
#pragma mark Globals
#pragma mark -

#define GZIP_FILE_EXTENSION string ( ".gz" )

errcode g_last_error;
errcode g_last_ddl_error;
string g_text_encoding;
string g_json_error_description;
BEOAuth * g_oauth;
struct host_details g_smtp_host;
BESMTPContainerAttachments g_smtp_attachments;

extern int g_http_response_code;
extern string g_http_response_headers;
extern CustomHeaders g_http_custom_headers;
extern std::stringstream g_curl_trace;
extern struct host_details g_http_proxy;
extern BECurlOptionMap g_curl_options;


#pragma mark -
#pragma mark Version
#pragma mark -

FMX_PROC(errcode) BE_Version ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	return TextConstantFunction ( VERSION_NUMBER_STRING, results );	
}


FMX_PROC(errcode) BE_VersionAutoUpdate ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	return TextConstantFunction ( AUTO_UPDATE_VERSION, results );		
}


#pragma mark -
#pragma mark Errors
#pragma mark -


FMX_PROC(errcode) BE_GetLastError ( short funcId, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = kNoError; // do not use NoError();
	
	try {
		
		if ( funcId == kBE_GetLastError ) {
			SetResult ( g_last_error, results );
		} else if ( funcId == kBE_GetLastDDLError ) {
			SetResult ( g_last_ddl_error, results );
		}

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return error; // MapError ( error );
}



#pragma mark -
#pragma mark Clipboard
#pragma mark -

FMX_PROC(errcode) BE_ClipboardFormats ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	return TextConstantFunction ( ClipboardFormats(), results );	
}


FMX_PROC(errcode) BE_ClipboardData ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		WStringAutoPtr atype = ParameterAsWideString ( parameters, 0 );
		StringAutoPtr clipboard_contents = ClipboardData ( atype );
		SetResult ( clipboard_contents, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ClipboardData


FMX_PROC(errcode) BE_SetClipboardData ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {

		StringAutoPtr to_copy = ParameterAsUTF8String ( parameters, 0 );
		WStringAutoPtr atype = ParameterAsWideString ( parameters, 1 );
		bool success = SetClipboardData ( to_copy, atype );
		SetResult ( success, results );

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SetClipboardData


#pragma mark -
#pragma mark Files & Folders
#pragma mark -


#pragma NOTE (consider refactoring some of the detail from the file functions into BEFileSystem)


FMX_PROC(errcode) BE_CreateFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();	
	
	try {

		path directory_path = ParameterAsPath ( parameters, 0 );
		
		try {
			create_directories ( directory_path );
		} catch ( filesystem_error& e ) {
			g_last_error = e.code().value();
		}
		
		SetResult ( g_last_error, results );
				
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_CreateFolder


FMX_PROC(errcode) BE_DeleteFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		path path = ParameterAsPath ( parameters, 0 );
		
		try {
			remove_all ( path ); // if path is a directory then path and all it's contents are deleted
		} catch ( filesystem_error& e ) {
			g_last_error = e.code().value();
		}
		
		SetResult ( g_last_error, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_DeleteFile


FMX_PROC(errcode) BE_FileExists ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		path path = ParameterAsPath ( parameters, 0 );
		
		bool file_exists = exists ( path );

		SetResult ( file_exists, results );
		
	} catch ( filesystem_error& e ) {
		g_last_error = e.code().value();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_FileExists



FMX_PROC(errcode) BE_FileSize ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		path path = ParameterAsPath ( parameters, 0 );
		
		uintmax_t size = file_size ( path );
		
		SetResult ( size, results );
		
	} catch ( filesystem_error& e ) {
		g_last_error = e.code().value();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_FileSize


#pragma mark BE_File_Modification_Timestamp

FMX_PROC(errcode) BE_File_Modification_Timestamp ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
    errcode error = NoError();
    
    try {
        
        const path path = ParameterAsPath ( parameters );
        
        const std::time_t last_modified = boost::filesystem::last_write_time ( path );
		const fmx::int64 timestamp = std_time_to_timestamp ( last_modified );

		SetResult ( timestamp, results );

	} catch ( filesystem_error& e ) {
        g_last_error = e.code().value();
    } catch ( bad_alloc& /* e */ ) {
        error = kLowMemoryError;
    } catch ( exception& /* e */ ) {
        error = kErrorUnknown;
    }
    
    return MapError ( error );
    
} // BE_File_Modification_Timestamp


FMX_PROC(errcode) BE_ReadTextFromFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		path file = ParameterAsPath ( parameters, 0 );
		StringAutoPtr contents = ReadFileAsUTF8 ( file );

		SetResult ( contents, results );

	} catch ( filesystem_error& e ) {
		g_last_error = e.code().value();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ReadTextFromFile


FMX_PROC(errcode) BE_WriteTextToFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
		
	try {
		
		path path = ParameterAsPath ( parameters );
		
		// should the text be appended to the file or replace any existing contents
		
		ios_base::openmode mode = ios_base::trunc;
		if ( parameters.Size() == 3 ) {
			bool append = parameters.AtAsBoolean ( 2 );
			if ( append ) {
				mode = ios_base::app;
			}
		}
		
		StringAutoPtr text_to_write = ParameterAsUTF8String ( parameters, 1 );
		vector<char> out = ConvertTextEncoding ( (char *)text_to_write->c_str(), text_to_write->size(), g_text_encoding, UTF8 );
		
		error = write_to_file ( path, out, mode );

		SetResult ( error, results );
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_WriteTextToFile


#pragma mark BE_StripInvalidUTF16CharactersFromXMLFile

/*
 filemaker can create DDRs which contain utf-16 characters that are not
 valid in an XML document. reads the DDR and writes out a new one, skipping
 any invalid characters, and replaces the old file
 */

FMX_PROC(errcode) BE_StripInvalidUTF16CharactersFromXMLFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		path source = ParameterAsPath ( parameters, 0 );

		path destination = source;
		destination += TEMPORARY_FILE_SUFFIX;
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
				(*utf16 == 0x1A) || // ascii 26 - causes problems in pcdata
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
				error = kFileSystemError;
			}
		}

		SetResult ( error == kNoError, results );
		
	} catch ( filesystem_error& e ) {
		g_last_error = e.code().value();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_StripInvalidUTF16CharactersFromXMLFile



FMX_PROC(errcode) BE_ExportFieldContents ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& /* results */ )
{
	errcode error = NoError();
	
	try {
		
		vector<char> field_contents = ParameterAsVectorChar ( parameters );
		path destination = ParameterAsPath ( parameters, 1 );
		
		error = write_to_file ( destination, field_contents );

		//		SetResult ( "", results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ExportFieldContents


FMX_PROC(errcode) BE_ImportFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		path from = ParameterAsPath ( parameters, 0 );
		bool compress = ParameterAsBoolean ( parameters, 1, false );
		std::string data_type = FILE_CONTAINER_TYPE;
		if ( compress ) {
			data_type = COMPRESSED_CONTAINER_TYPE;
		}
		
		// slurp up the file contents
		boost::filesystem::ifstream input_file ( from, ios_base::in | ios_base::binary | ios_base::ate );
		input_file.exceptions ( boost::filesystem::ofstream::badbit | boost::filesystem::ofstream::failbit );			
		input_file.seekg ( 0, ios::beg );
		vector<char> file_data ( (std::istreambuf_iterator<char> ( input_file ) ), std::istreambuf_iterator<char>() );
		
		SetResult ( from.filename().string(), file_data, results, data_type );
		
	} catch ( boost::filesystem::ifstream::failure& /* e */ ) {
		error = errno; // cannot read the file
	} catch ( boost::filesystem::filesystem_error& e ) {
		error = e.code().value();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ImportFile


FMX_PROC(errcode) BE_MoveFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		path from = ParameterAsPath ( parameters, 0 );
		path to = ParameterAsPath ( parameters, 1 );

		try {
			rename ( from, to );
		} catch ( filesystem_error& e ) {
			g_last_error = e.code().value();
		}
		
		SetResult ( g_last_error, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_MoveFile


FMX_PROC(errcode) BE_CopyFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		path from = ParameterAsPath ( parameters, 0 );
		path to = ParameterAsPath ( parameters, 1 );
		
		try {
			recursive_directory_copy ( from, to );
		} catch ( filesystem_error& e ) {
			g_last_error = e.code().value();
		}
		
		SetResult ( g_last_error, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_CopyFile



FMX_PROC(errcode) BE_ListFilesInFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		const path directory = ParameterAsPath ( parameters, 0 );
		const long file_type_wanted = ParameterAsLong ( parameters, 1, kBE_FileType_File );
		const bool include_subfolders = ParameterAsBoolean ( parameters, 2, false );
		const bool use_full_path = ParameterAsBoolean ( parameters, 3, false );
		
		try {

			BEValueListWideStringAutoPtr list_of_files ( list_files_in_directory ( directory, file_type_wanted, include_subfolders ) );
			if ( ! use_full_path ) {
				list_of_files->remove_prefix ( directory.wstring() );
			}
			SetResult ( list_of_files->get_as_filemaker_string(), results );
			
		} catch ( BEPlugin_Exception& e ) {
			error = e.code();
		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ListFilesInFolder



#pragma mark -
#pragma mark Dialogs
#pragma mark -

FMX_PROC(errcode) BE_SelectFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		WStringAutoPtr prompt = ParameterAsWideString ( parameters, 0 );
		WStringAutoPtr inFolder = ParameterAsWideString ( parameters, 1 );
		
		WStringAutoPtr files ( SelectFile ( prompt, inFolder ) );
		
		SetResult ( files, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SelectFile


FMX_PROC(errcode) BE_SelectFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		WStringAutoPtr prompt = ParameterAsWideString ( parameters, 0 );
		WStringAutoPtr inFolder = ParameterAsWideString ( parameters, 1 );
		
		WStringAutoPtr folder = SelectFolder ( prompt, inFolder );
		
		SetResult ( folder, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SelectFolder


FMX_PROC(errcode) BE_SaveFileDialog ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		WStringAutoPtr prompt = ParameterAsWideString ( parameters, 0 );
		WStringAutoPtr fileName = ParameterAsWideString ( parameters, 1 );
		WStringAutoPtr inFolder = ParameterAsWideString ( parameters, 2 );
		
		WStringAutoPtr folder = SaveFileDialog ( prompt, fileName, inFolder );
		
		SetResult ( folder, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SelectFolder


FMX_PROC(errcode) BE_DisplayDialog ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
		
	try {

		WStringAutoPtr title = ParameterAsWideString ( parameters, 0 );
		WStringAutoPtr message = ParameterAsWideString ( parameters, 1 );
		WStringAutoPtr ok_button = ParameterAsWideString ( parameters, 2 );
		WStringAutoPtr cancel_button = ParameterAsWideString ( parameters, 3 );
		WStringAutoPtr alternate_button = ParameterAsWideString ( parameters, 4 );
	
		int response = DisplayDialog ( title, message, ok_button, cancel_button, alternate_button );
		SetResult ( response, results );

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );
	
} // BE_DisplayDialog



FMX_PROC(errcode) BE_ProgressDialog ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& /* results */ )
{
	errcode error = NoError();
	
	try {
		
		WStringAutoPtr title = ParameterAsWideString ( parameters, 0 );
		WStringAutoPtr description = ParameterAsWideString ( parameters, 1 );
		long maximum = ParameterAsLong ( parameters, 2, 0 ); // 0 == indeterminite
		
		// allow the user to cancel ?
		bool can_cancel = AllowUserAbort ( environment );
		
		// display the progress bar
		error = DisplayProgressDialog ( title, description, maximum, can_cancel );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error, true);
	
} // BE_ProgressDialog


FMX_PROC(errcode) BE_ProgressDialog_Update ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& /* results */ )
{
	errcode error = NoError();
	
	try {
		
		long value = ParameterAsLong ( parameters, 0, 0 );
		value = value < 0 ? 0 : value;
		
		WStringAutoPtr description = ParameterAsWideString ( parameters, 1 );
		
		error = UpdateProgressDialog ( value, description );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error, true );
	
} // BE_ProgressDialog_Update



#pragma mark -
#pragma mark XML / XSLT
#pragma mark -


#pragma mark BE_ApplyXSLT

FMX_PROC(errcode) BE_ApplyXSLT ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		path xml_path = ParameterAsPath ( parameters );
		StringAutoPtr xslt = ParameterAsUTF8String ( parameters, 1 );
		path csv_path = ParameterAsPath ( parameters, 2 );
		
		SetResult ( *ApplyXSLT ( xml_path, xslt, csv_path ), results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ApplyXSLT


FMX_PROC(errcode) BE_ApplyXSLTInMemory ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr xml = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr xslt = ParameterAsUTF8String ( parameters, 1 );
		
		results.SetAsText( *ApplyXSLTInMemory ( xml, xslt ), parameters.At(0).GetLocale() );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ApplyXSLTInMemory


FMX_PROC(errcode) BE_XPath ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr xml = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr xpath = ParameterAsUTF8String ( parameters, 1 );
		StringAutoPtr ns_list ( new string );

		const unsigned long number_of_parameters = parameters.Size();
		if ( number_of_parameters > 2 ) {
			ns_list = ParameterAsUTF8String ( parameters, 2 );
		}
		
		xmlXPathObjectType xpath_object_type = XPATH_STRING;
		if ( number_of_parameters > 3 ) {
			
			bool as_text = ParameterAsBoolean ( parameters, 3 );
			if ( as_text != true ) {
				xpath_object_type = XPATH_UNDEFINED; // get the result as xml
			}
		
		}
		
		results.SetAsText( *ApplyXPathExpression ( xml, xpath, ns_list, xpath_object_type ), parameters.At(0).GetLocale() );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_XPath


FMX_PROC(errcode) BE_XPathAll ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	TextAutoPtr text;
	
	try {

		StringAutoPtr xml = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr xpath = ParameterAsUTF8String ( parameters, 1 );

		const unsigned long number_of_parameters = parameters.Size();
		StringAutoPtr ns_list ( new string );
		if ( number_of_parameters > 2 ) {
			ns_list = ParameterAsUTF8String ( parameters, 2 );
		}
				
		results.SetAsText ( *ApplyXPathExpression ( xml, xpath, ns_list, XPATH_NODESET ), parameters.At(0).GetLocale() );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_XPathAll



FMX_PROC(errcode) BE_StripXMLNodes ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();
	
	try {
		
		path input_file = ParameterAsPath ( parameters, 0 );
		path output_file = ParameterAsPath ( parameters, 1 );
		StringAutoPtr node_names = ParameterAsUTF8String ( parameters, 2 );
		
		vector<string> node_names_vector;
		boost::tokenizer<> tokeniser ( *node_names );
		for ( boost::tokenizer<>::iterator it = tokeniser.begin() ; it != tokeniser.end() ; ++it ) {
			node_names_vector.push_back ( *it );
		}
		
		error = StripXMLNodes ( input_file, output_file, node_names_vector );
		SetResult ( error, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_StripXMLNodes



FMX_PROC(errcode) BE_XML_Parse ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();
	
	try {
		
		path input_file = ParameterAsPath ( parameters, 0 );
		
		auto_ptr<BEXMLTextReader> reader ( new BEXMLTextReader ( input_file ) );
		string result = reader->parse();
		
		SetResult ( result, results );
		
	} catch ( BEXMLReaderInterface_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_XML_Parse



FMX_PROC(errcode) BE_SplitBEFileNodes ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();
	
	try {
		
		path input_file = ParameterAsPath ( parameters, 0 );
		
		int result = SplitBEXMLFiles ( input_file );
				
		SetResult ( result, results );
		
	} catch ( BEXMLReaderInterface_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SplitBEFileNodes



#pragma mark -
#pragma mark JSON
#pragma mark -


FMX_PROC(errcode) BE_JSONPath ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	g_json_error_description = "";
	
	try {
		
		StringAutoPtr json = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr json_path_expression = ParameterAsUTF8String ( parameters, 1 );
		
		auto_ptr<BEJSON> json_document ( new BEJSON ( json ) );
		json_document->json_path_query ( json_path_expression, results );
		
	} catch ( BEJSON_Exception& e ) {
		error = e.code();
		g_json_error_description = e.description();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_JSONPath



// do not set the global last error

FMX_PROC(errcode) BE_JSON_Error_Description ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = kNoError;
	
	try {

		SetResult ( g_json_error_description, results );

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return error;
	
} // BE_JSON_Error_Description



FMX_PROC(errcode) BE_JSON_ArraySize ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	g_json_error_description = "";
	
	try {
		
		StringAutoPtr json = ParameterAsUTF8String ( parameters, 0 );
		
		auto_ptr<BEJSON> json_document ( new BEJSON ( json ) );
		json_document->array_size ( results );
		
	} catch ( BEJSON_Exception& e ) {
		error = e.code();
		g_json_error_description = e.description();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_JSON_ArraySize



FMX_PROC(errcode) BE_JSON_Encode ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	g_json_error_description = "";
	
	try {
		
		StringAutoPtr key = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr type = ParameterAsUTF8String ( parameters, 2 );
		
		if ( parameters.Size() == 1 ) {

			string out = "\"" + *key + "\":";
			SetResult ( out, results );

		} else {

			auto_ptr<BEJSON> json_document ( new BEJSON ( ) );
			StringAutoPtr json = json_document->encode ( key, parameters.At ( 1 ), type );
			json->erase ( 0, 1 ); // remove {
			json->erase ( json->length() - 1 ); // remove }
			SetResult ( json, results );

		}
		
	} catch ( BEJSON_Exception& e ) {
		error = e.code();
		g_json_error_description = e.description();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_JSON_Encode



#pragma mark -
#pragma mark JavaScript
#pragma mark -

// EXPERIMENTAL !!!
// Use at your own risk

FMX_PROC(fmx::errcode) BE_EvaluateJavaScript ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr javaScript = ParameterAsUTF8String ( parameters, 0 );
		
		StringAutoPtr jsResult = Evaluate_JavaScript ( javaScript );
		
		SetResult ( jsResult, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SetPreference


#pragma mark -
#pragma mark User Preferences
#pragma mark -


FMX_PROC(errcode) BE_SetPreference ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		WStringAutoPtr key = ParameterAsWideString ( parameters, 0 );
		WStringAutoPtr value = ParameterAsWideString ( parameters, 1 );
		WStringAutoPtr domain = ParameterAsWideString ( parameters, 2 );
		
		if ( domain->empty() ) {
			domain->assign ( USER_PREFERENCES_DOMAIN );
		}

		SetResult ( SetPreference ( key, value, domain ), results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SetPreference



FMX_PROC(errcode) BE_GetPreference ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		WStringAutoPtr key = ParameterAsWideString ( parameters, 0 );
		WStringAutoPtr domain = ParameterAsWideString ( parameters, 1 );
		
		if ( domain->empty() ) {
			domain->assign ( USER_PREFERENCES_DOMAIN );
		}
		
		SetResult ( GetPreference ( key, domain ), results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_GetPreference



#pragma mark -
#pragma mark Compression / Encoding
#pragma mark -



FMX_PROC(errcode) BE_Unzip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr archive = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr output_directory = ParameterAsUTF8String ( parameters, 1 );

		error = (fmx::errcode)UnZip ( archive, output_directory );
		SetResult ( error, results );
		
	} catch ( filesystem_error& e ) {
		g_last_error = e.code().value();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Unzip



FMX_PROC(errcode) BE_Zip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		const BEValueList<string> * files  = new BEValueList<string> ( *ParameterAsUTF8String ( parameters, 0 ) );
		StringAutoPtr output_directory = ParameterAsUTF8String ( parameters, 1 );

		error = (fmx::errcode)Zip ( files, output_directory );
		SetResult ( error, results );
		
		delete files;
		
	} catch ( filesystem_error& e ) {
		g_last_error = e.code().value();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Zip



FMX_PROC(errcode) BE_Base64_Decode ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr text = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr filename = ParameterAsUTF8String ( parameters, 1 );
		
		// decode it...
		vector<char> data = Base64_Decode ( text );
		if ( filename->empty() ) {
			SetResult ( data, results );
		} else {
			SetResult ( *filename, data, results );
		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Base64_Decode



FMX_PROC(errcode) BE_Base64_Encode ( short funcId, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {

		vector<char> data = ParameterAsVectorChar ( parameters, 0 );
		StringAutoPtr base64 = Base64_Encode ( data, funcId == kBE_Base64_URL_Encode );
		
		SetResult ( base64, results );
				
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Base64_Encode



FMX_PROC(errcode) BE_SetTextEncoding ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr encoding = ParameterAsUTF8String ( parameters, 0 );
		
		iconv_t conversion = iconv_open ( encoding->c_str(), encoding->c_str() );
		if ( conversion != (iconv_t)-1 ) {
			SetTextEncoding ( *encoding );
			iconv_close ( conversion );
		 } else {
			error = errno;
		 }


		SetResult ( error, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SetTextEncoding


FMX_PROC(errcode) BE_ContainerIsCompressed ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		if ( (parameters.At(0)).GetNativeType() == fmx::Data::kDTBinary ) {
			
			const BinaryDataAutoPtr data_stream ( parameters.AtAsBinaryData ( 0 ) );

// defeat: Returning null reference (within a call to 'operator*')
#ifndef __clang_analyzer__
			bool compressed = StreamIsCompressed ( *data_stream );
#else
			bool compressed = false;
#endif
			
			SetResult ( compressed, results );
			
		} else {
			error = kInvalidFieldType;
		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ContainerIsCompressed


#pragma mark BE_ContainerCompress

FMX_PROC(errcode) BE_ContainerCompress ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		vector<char> to_compress = ParameterAsVectorChar ( parameters );
		StringAutoPtr filename = ParameterAsUTF8String ( parameters, 1 );

		SetResult ( *filename, to_compress, results, COMPRESSED_CONTAINER_TYPE );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ContainerCompress


#pragma mark BE_ContainerUncompress

FMX_PROC(errcode) BE_ContainerUncompress ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		vector<char> gzipped = ParameterAsVectorChar ( parameters );
		StringAutoPtr filename = ParameterAsUTF8String ( parameters, 1 );

		SetResult ( *filename, gzipped, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ContainerUncompress


#pragma mark BE_Gzip

FMX_PROC(errcode) BE_Gzip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		const vector<char> to_compress = ParameterAsVectorChar ( parameters );
		if ( !to_compress.empty() ) {
			
			const vector<char> compressed = CompressContainerStream ( to_compress );
			
			path filename = ParameterAsPath ( parameters, 1 );
			if ( filename.empty() ) {
				filename = *ParameterFileName ( parameters );
				filename += GZIP_FILE_EXTENSION;
			}
			
			SetResult ( filename.string(), compressed, results );

		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Gzip


#pragma mark BE_UnGzip

FMX_PROC(errcode) BE_UnGzip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		const vector<char> gzipped = ParameterAsVectorChar ( parameters );
		if ( !gzipped.empty() ) {
			
			const vector<char> uncompressed = UncompressContainerStream ( gzipped );
			
			path filename = ParameterAsPath ( parameters, 1 );
			if ( filename.empty() ) {
				filename = *ParameterFileName ( parameters );
				if ( filename.extension() == GZIP_FILE_EXTENSION ) {
					filename.replace_extension ( "" );
				}
			}
			
			SetResult ( filename.string(), uncompressed, results );

		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_UnGzip



#pragma mark -
#pragma mark Encryption
#pragma mark -


FMX_PROC(errcode) BE_Encrypt_AES ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr text = ParameterAsUTF8String ( parameters, 1 );
		
		string key;
		vector<char> input_vector;
		GenerateKeyAndInputVector ( *password, key, input_vector );
		
		// escape the delimiter we use below
		replace ( input_vector.begin(), input_vector.end(), FILEMAKER_END_OF_LINE_CHAR, '\n' );
		
		vector<char> output_to_encode ( input_vector.begin(), input_vector.end() );
		output_to_encode.push_back ( FILEMAKER_END_OF_LINE_CHAR );
		
		vector<char> encrypted_data = Encrypt_AES ( key, *text, input_vector );
		output_to_encode.insert ( output_to_encode.end(), encrypted_data.begin(), encrypted_data.end() );
		
		StringAutoPtr base64 = Base64_Encode ( output_to_encode );
		SetResult ( base64, results );
		
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Encrypt_AES



FMX_PROC(errcode) BE_Decrypt_AES ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr text = ParameterAsUTF8String ( parameters, 1 );
		
		string key;
		vector<char> unwanted;
		GenerateKeyAndInputVector ( *password, key, unwanted );
		
		vector<char> decoded = Base64_Decode ( text );
		
		std::vector<char>::iterator it = find ( decoded.begin(), decoded.end(), FILEMAKER_END_OF_LINE_CHAR );
		if ( it != decoded.end() ) {

			const vector<char> input_vector ( decoded.begin(), it );
			decoded.erase ( decoded.begin(), it + 1 ); // remove the input vector from the input
			vector<char> decrypted_data = Decrypt_AES ( key, decoded, input_vector );

			SetResult ( decrypted_data, results );
			
		} else {
			error = kDecryptionInputVectorNotFound;
		}
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Decrypt_AES


#pragma mark -
#pragma mark HTTP / Curl
#pragma mark -


FMX_PROC(errcode) BE_GetURL ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{	
	errcode error = NoError();
	
	try {
		
		StringAutoPtr url = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr filename = ParameterAsUTF8String ( parameters, 1 );
		StringAutoPtr username = ParameterAsUTF8String ( parameters, 2 );
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 3 );
		
		// not saving to file so do not supply the filename here
		BECurl curl ( *url, kBE_HTTP_METHOD_GET, "", *username, *password );
		vector<char> data = curl.perform_action ( );
		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( *filename, data, results );
		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_GetURL



FMX_PROC(errcode) BE_SaveURLToFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& /* results */ )
{	
	errcode error = NoError();
	
	try {
		
		StringAutoPtr url = ParameterAsUTF8String ( parameters, 0 );
		path filename = ParameterAsPath ( parameters, 1 );
		StringAutoPtr username = ParameterAsUTF8String ( parameters, 2 );
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 3 );
		
		BECurl curl ( *url, kBE_HTTP_METHOD_GET, filename, *username, *password );
		vector<char> data = curl.perform_action ( );
		error = g_last_error;
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SaveURLToFile


#pragma mark BE_HTTP_POST_PUT_PATCH

FMX_PROC(errcode) BE_HTTP_POST_PUT_PATCH ( short funcId, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{	
	errcode error = NoError();
	
	try {
		
		StringAutoPtr url = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr username = ParameterAsUTF8String ( parameters, 2 );
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 3 );
				
		vector<char> response;
		
		if ( funcId == kBE_HTTP_POST ) {

			StringAutoPtr post_parameters = ParameterAsUTF8String ( parameters, 1 );
			BECurl curl ( *url, kBE_HTTP_METHOD_POST, "", *username, *password, *post_parameters );
			response =  curl.perform_action ( );
		
		
		} else if ( funcId == kBE_HTTP_PATCH ) {
			
			StringAutoPtr post_parameters = ParameterAsUTF8String ( parameters, 1 );
			BECurl curl ( *url, kBE_HTTP_METHOD_PATCH, "", *username, *password, *post_parameters );
			response =  curl.perform_action ( );
			
			
		} else if ( funcId == kBE_HTTP_PUT_File ) {

			path filename = ParameterAsPath ( parameters, 1 );
			BECurl curl ( *url, kBE_HTTP_METHOD_PUT, filename, *username, *password );
			response = curl.perform_action ( );
			
			
		} else { // kBE_HTTP_PUT_DATA
			
			vector<char> data = ParameterAsVectorChar ( parameters, 1 );
			BECurl curl ( *url, kBE_HTTP_METHOD_PUT, "", *username, *password, "", data );
			response = curl.perform_action ( );
			
		}

		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( response, results );
		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_HTTP_POST_PUT_PATCH



FMX_PROC(errcode) BE_HTTP_DELETE ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{	
	errcode error = NoError();
	
	try {
		
		StringAutoPtr url = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr username = ParameterAsUTF8String ( parameters, 1 );
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 2 );
		
		BECurl curl ( *url, kBE_HTTP_METHOD_DELETE, "", *username, *password );
		vector<char> data = curl.perform_action ( );
		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( data, results );
		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_HTTP_DELETE



FMX_PROC(errcode) BE_HTTP_Response_Code ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{	
	errcode error = NoError();
	
	try {
		
		SetResult ( g_http_response_code, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_HTTP_Response_Code



FMX_PROC(errcode) BE_HTTP_Response_Headers ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{	
	errcode error = NoError();
	
	try {
		
		StringAutoPtr headers ( new string ( g_http_response_headers ) );
		SetResult ( headers, results );

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_HTTP_Response_Headers



FMX_PROC(errcode) BE_HTTP_Set_Custom_Header ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{	
	errcode error = NoError();
	
	try {
		
		StringAutoPtr name = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr value = ParameterAsUTF8String ( parameters, 1 );
		
		if ( value->empty() ) {
			g_http_custom_headers.erase ( *name );
		} else {
			g_http_custom_headers [ *name ] = *value;
		}
		
		SetResult ( g_last_error, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_HTTP_Set_Custom_Header



FMX_PROC(errcode) BE_HTTP_Set_Proxy ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		g_http_proxy.host = *ParameterAsUTF8String ( parameters, 0 );
		g_http_proxy.port = *ParameterAsUTF8String ( parameters, 1 );
		g_http_proxy.username = *ParameterAsUTF8String ( parameters, 2 );
		g_http_proxy.password = *ParameterAsUTF8String ( parameters, 3 );

		SetResult ( g_last_error, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_HTTP_Set_Custom_Header


#pragma mark BE_Curl_Set_Option

FMX_PROC(fmx::errcode) BE_Curl_Set_Option ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();
	
	try {
		
		const StringAutoPtr option = ParameterAsUTF8String ( parameters );
		
		const unsigned long number_of_parameters = parameters.Size();
		switch ( number_of_parameters ) {
			
			case 0:
				g_curl_options.clear();
				break;
				
			case 1:
			{
				BECurlOptionMap::iterator it = g_curl_options.find ( *option );
				if ( it != g_curl_options.end() ) {
					g_curl_options.erase ( it );
				} else {
					error = kNotFoundError;
				}
				break;
			}
				
			case 2:
				boost::shared_ptr<BECurlOption> curl_option ( new BECurlOption ( *option, parameters.At ( 1 ) ) ); // throws if option not known or not handled
				g_curl_options [ *option ] = curl_option;
				break;
				
//			default:
//				;
				
		}

	} catch ( BECurlOption_Exception& e ) { // we don't handle it
		error = e.code();
	} catch ( out_of_range& /* e */ ) { // we don't know about it
		error = kNameIsNotValid;
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Curl_Set_Option


FMX_PROC(fmx::errcode) BE_Curl_Trace ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& /* parameters */, fmx::Data& results )
{
	errcode error = kNoError; // do not set the global last error

	try {

		SetResult ( g_curl_trace.str(), results );

	} catch ( BECurlOption_Exception& e ) { // we don't handle it
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return error;

} // BE_Curl_Trace


#pragma mark BE_FTP_Upload

FMX_PROC(errcode) BE_FTP_Upload ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr url = ParameterAsUTF8String ( parameters );
		vector<char> data = ParameterAsVectorChar ( parameters, 1 );
		StringAutoPtr username = ParameterAsUTF8String ( parameters, 2 );
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 3 );
		
		BECurl curl ( *url, kBE_FTP_METHOD_UPLOAD, "", *username, *password, "", data );
		vector<char> response = curl.perform_action ( );
			
		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( response, results );
		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_FTP_Upload


#pragma mark BE_FTP_Delete

FMX_PROC(errcode) BE_FTP_Delete ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr url = ParameterAsUTF8String ( parameters );
		StringAutoPtr username = ParameterAsUTF8String ( parameters, 1 );
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 2 );
		
		auto_ptr<BECurl> curl ( new BECurl ( *url, kBE_FTP_METHOD_DELETE, "", *username, *password ) );
		vector<char> response = curl->perform_action ( );

		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( response, results );
		}
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_FTP_Delete


#pragma mark -
#pragma mark SMTP
#pragma mark -


#pragma mark BE_SMTP_Server

FMX_PROC(fmx::errcode) BE_SMTP_Server ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr host = ParameterAsUTF8String ( parameters );
		StringAutoPtr port = ParameterAsUTF8String ( parameters, 1 );
		StringAutoPtr username = ParameterAsUTF8String ( parameters, 2 );
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 3 );

		g_smtp_host.host = *host;
		g_smtp_host.port = *port;
		g_smtp_host.username = *username;
		g_smtp_host.password = *password;
		
//		string do_nothing = "";
//		SetResult ( do_nothing, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Email_SMTP_Server


#pragma mark BE_SMTP_Send

FMX_PROC(fmx::errcode) BE_SMTP_Send ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr from = ParameterAsUTF8String ( parameters );
		StringAutoPtr to = ParameterAsUTF8String ( parameters, 1 );
		StringAutoPtr subject = ParameterAsUTF8String ( parameters, 2 );
		StringAutoPtr text = ParameterAsUTF8String ( parameters, 3 );
		
		auto_ptr<BESMTPEmailMessage> message ( new BESMTPEmailMessage ( *from, *to, *subject, *text ) );
		
		StringAutoPtr cc = ParameterAsUTF8String ( parameters, 4 );
		message->set_cc_addresses ( *cc );
		
		StringAutoPtr bcc = ParameterAsUTF8String ( parameters, 5 );
		message->set_bcc_addresses ( *bcc );
		
		StringAutoPtr reply_to = ParameterAsUTF8String ( parameters, 6 );
		message->set_reply_to ( *reply_to );

		StringAutoPtr html = ParameterAsUTF8String ( parameters, 7 );
		message->set_html_alternative ( *html );
		
		const WStringAutoPtr attachments = ParameterAsWideString ( parameters, 8 );
		vector<path> values;
		if ( !attachments->empty() ) {
			boost::split ( values, *attachments, boost::is_any_of ( FILEMAKER_END_OF_LINE ), boost::token_compress_on );
		}
		BEValueList<path> attachment_list = BEValueList<path> ( values );
		BEValueList<path> container_attachments = g_smtp_attachments.get_file_list();
		attachment_list.append ( container_attachments );
		message->set_attachments ( attachment_list );

		auto_ptr<BESMTP> smtp ( new BESMTP ( g_smtp_host.host, g_smtp_host.port, g_smtp_host.username, g_smtp_host.password ) );
		error = smtp->send ( message.get() );
		
//		string do_nothing = "";
//		SetResult ( do_nothing, results );
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	g_smtp_attachments.clear(); // clear out and clean up any "container" attachments
	
	return MapError ( error );
	
} // BE_SMTP_Send


#pragma mark BE_SMTP_AddAttachment

FMX_PROC(fmx::errcode) BE_SMTP_AddAttachment ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();
	
	try {
		
		if ( parameters.Size() == 1 ) {
			
			vector<char> contents = ParameterAsVectorChar ( parameters );
			StringAutoPtr file_name = ParameterFileName ( parameters );

			g_smtp_attachments.add ( *file_name, contents );

		} else { // destroy the temporary files and clear out the list
			g_smtp_attachments.clear();
		}
		
		//		string do_nothing = "";
		//		SetResult ( do_nothing, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_SMTP_AddAttachment



#pragma mark -
#pragma mark FileMaker Server
#pragma mark -


#pragma mark BE_FMS_Command

FMX_PROC(fmx::errcode) BE_FMS_Command ( short function_id, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr username = ParameterAsUTF8String ( parameters );
		StringAutoPtr password = ParameterAsUTF8String ( parameters, 1 );
		const bool show_statistics = ParameterAsBoolean ( parameters, 2, false );

		string reply = "";
		auto_ptr<BEFMS> fms ( new BEFMS ( *username, *password ) );

		switch ( function_id ) {
			
			case kBE_FMS_List_Files:
				reply = fms->list_files ( show_statistics );
				break;
				
			case kBE_FMS_Pause_Files:
				reply = fms->pause_files ( *(ParameterAsUTF8String ( parameters, 2 )) );
				break;
				
			case kBE_FMS_Run_Schedule:
				reply = fms->run_schedule ( ParameterAsLong ( parameters, 2 ) );
				break;
				
			case kBE_FMS_List_Schedules:
				reply = fms->list_schedules();
				break;
				
			case kBE_FMS_List_Clients:
				reply = fms->list_clients ( show_statistics );
				break;
				
			case kBE_FMS_Resume_Files:
				reply = fms->resume_files ( *(ParameterAsUTF8String ( parameters, 2 )) );
				break;
				
			default:
				;
			
		}
		
		SetResult ( reply, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_FMS_Command


#pragma mark -
#pragma mark OAuth
#pragma mark -


FMX_PROC(errcode) BE_OAuth_RequestAccessToken ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr uri = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr consumer_key = ParameterAsUTF8String ( parameters, 1 );
		StringAutoPtr consumer_secret = ParameterAsUTF8String ( parameters, 2 );
		StringAutoPtr request_key = ParameterAsUTF8String ( parameters, 3 );
		StringAutoPtr request_secret = ParameterAsUTF8String ( parameters, 4 );
		
		if ( g_oauth ) {
			delete g_oauth;
			g_oauth = NULL;
		}
		
		string response;
		
		// if the uri is empty then we are only clearing out any set oauth data
		
		if ( !uri->empty() ) {
			
			BEOAuth * oauth = new BEOAuth ( *consumer_key, *consumer_secret );
			error = oauth->oauth_request ( *uri, *request_key, *request_secret );
			
			// argh, nasty
			if ( error == kNoError ) {
				
				const unsigned long number_of_parameters = parameters.Size();
				
				if ( number_of_parameters == 3 ) {
					response = oauth->get_request_key() + FILEMAKER_END_OF_LINE + oauth->get_request_secret();
				} else {
					response = oauth->get_access_key() + FILEMAKER_END_OF_LINE + oauth->get_access_secret();
				}
				
				g_oauth = oauth; // must assign after the authorisation request otherwise BECurl will try and use g_oauth
				
			} else {
				response = oauth->get_last_error();
				if ( !response.empty() ) {
					error = kNoError;
				}
			}
			
		}
		
		SetResult ( response, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_OAuth_RequestAccessToken


#pragma mark -
#pragma mark Xero
#pragma mark -


FMX_PROC(errcode) BE_Xero_SetTokens ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		if ( g_oauth ) {
			delete g_oauth;
			g_oauth = NULL;
		}
		
		StringAutoPtr consumer_key = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr consumer_secret = ParameterAsUTF8String ( parameters, 1 );
		
		// if the consumer_key is empty then we are only clearing out any set oauth data
		
		if ( !consumer_key->empty() ) {
			
			boost::algorithm::replace_all ( *consumer_secret, FILEMAKER_END_OF_LINE, "\r\n" );

			BEXero * xero = new BEXero ( *consumer_key, *consumer_secret );
			g_oauth = xero; // must assign after the authorisation request otherwise BECurl will try and use g_oauth
			
		}
		
		SetResult ( "", results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Xero_SetTokens


FMX_PROC(errcode) BE_Xero_GenerateKeys ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {

		StringAutoPtr organisation = ParameterAsUTF8String ( parameters );
		StringAutoPtr organisational_unit = ParameterAsUTF8String ( parameters, 1 );
		StringAutoPtr country = ParameterAsUTF8String ( parameters, 2 );
		StringAutoPtr state = ParameterAsUTF8String ( parameters, 3 );
		StringAutoPtr suburb = ParameterAsUTF8String ( parameters, 4 );
		StringAutoPtr domain = ParameterAsUTF8String ( parameters, 5 );
		StringAutoPtr email = ParameterAsUTF8String ( parameters, 6 );

		auto_ptr<BEX509> x509 ( new BEX509 );

		x509->add_name_entry ( "O", *organisation );
		x509->add_name_entry ( "OU", *organisational_unit );
		x509->add_name_entry ( "C", *country );
		x509->add_name_entry ( "ST", *state );
		x509->add_name_entry ( "L", *suburb );
		x509->add_name_entry ( "CN", *domain );
		x509->add_name_entry ( "emailAddress", *email );

		SetResult ( xero_generate_key_pair ( x509.get() ), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Xero_GenerateKeys



#pragma mark -
#pragma mark Value Lists
#pragma mark -


FMX_PROC(errcode) BE_Values_Unique ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr value_list = ParameterAsUTF8String ( parameters, 0 );
		bool case_sensitive = ParameterAsBoolean ( parameters, 1 );

		auto_ptr< BEValueList<string> > values ( new BEValueList<string> ( *value_list, case_sensitive ) );
		string unique_values = values->unique();
		
		SetResult ( unique_values, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Values_Unique



FMX_PROC(errcode) BE_Values_FilterOut ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr value_list = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr filter_out = ParameterAsUTF8String ( parameters, 1 );
		bool case_sensitive = ParameterAsBoolean ( parameters, 2 );
		
		auto_ptr< BEValueList<string> > values ( new BEValueList<string> ( *value_list, case_sensitive ) );
		auto_ptr< BEValueList<string> > filter ( new BEValueList<string> ( *filter_out, case_sensitive ) );
		string filtered_values = values->filter_out ( *filter );
				
		SetResult ( filtered_values, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Values_FilterOut


FMX_PROC(errcode) BE_Values_ContainsDuplicates ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		auto value_list = ParameterAsUTF8String ( parameters );
		auto case_sensitive = ParameterAsBoolean ( parameters, 1 );

		auto values ( new BEValueList<string> ( *value_list, case_sensitive ) );
		auto contains_duplicates = values->contains_duplicates();
		
		SetResult ( contains_duplicates, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Values_ContainsDuplicates


FMX_PROC(errcode) BE_Values_Sort ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr value_list = ParameterAsUTF8String ( parameters );
		const bool ascending = ParameterAsBoolean ( parameters, 1, true );
		const long type = ParameterAsLong ( parameters, 2, kBE_DataType_String );
		
		auto_ptr< BEValueList<string> > values ( new BEValueList<string> ( *value_list ) );
		string sorted_values = values->sort ( ascending, type );
		
		SetResult ( sorted_values, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Values_Sort


#pragma mark BE_Values_TimesDuplicated

FMX_PROC(errcode) BE_Values_TimesDuplicated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr value_list = ParameterAsUTF8String ( parameters );
		const long numberOfTimes = ParameterAsLong ( parameters, 1 );
		
		auto_ptr< BEValueList<string> > values ( new BEValueList<string> ( *value_list ) );
		BEValueList<std::string> times_duplicated = values->times_duplicated ( numberOfTimes );
		std::string found = times_duplicated.get_as_filemaker_string();
		SetResult ( found, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Values_Sort



#pragma mark -
#pragma mark Bitwise
#pragma mark -


FMX_PROC(fmx::errcode) BE_XOR ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		const StringAutoPtr text = ParameterAsUTF8String ( parameters );
		const uint8_t xorWith = (uint8_t)ParameterAsLong ( parameters, 1 );

		std::stringstream xord_text;

		for ( std::string::iterator it = text->begin() ; it != text->end() ; ++it ) {
			const uint8_t xord = *it ^ xorWith;
			xord_text << std::setfill ( '0' ) << std::setw ( 2 ) << std::hex << (const unsigned int)xord;
		}

		SetResult ( xord_text.str(), results );

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_XOR



#pragma mark -
#pragma mark Vector Operations
#pragma mark -


#pragma mark BE_Vector_DotProduct

FMX_PROC(fmx::errcode) BE_Vector_DotProduct ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {
		
		vector<double> a = ParameterAsVectorDouble ( parameters );
		const vector<double> b = ParameterAsVectorDouble ( parameters, 1 );
		
		// it's ok for <b> to be longer as only the number of elements that exist in <a> are used but not vica-versa
		while ( a.size() > b.size() ) {
			a.pop_back();
		}
		
		const double dot_product = std::inner_product ( a.begin(), a.end(), b.begin(), 0.0 );

		SetResult ( dot_product, results );

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Vector_DotProduct


#pragma mark BE_Vector_EuclideanDistance

FMX_PROC(fmx::errcode) BE_Vector_EuclideanDistance ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		const vector<double> a = ParameterAsVectorDouble ( parameters );
		const vector<double> b = ParameterAsVectorDouble ( parameters, 1 );

		double euclidean_distance = 0.0;
		auto ait = a.begin();
		auto bit = b.begin();

		while ( ait != a.end() && bit != b.end() ) {

			double dist = ( *ait++ ) - ( *bit++ );
			euclidean_distance += dist * dist;

		}

		euclidean_distance = euclidean_distance > 0.0 ? sqrt ( euclidean_distance ) : 0.0;

		SetResult ( euclidean_distance, results );

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Vector_EuclideanDistance



#pragma mark -
#pragma mark Other / Ungrouped
#pragma mark -


/*
 
 invoked for multiple plug-in functions... funcId is used to determine which one
 
 constants should be defined in BEPluginGlobalDefines.h
 
 each set of constants should have it's own range [ 1000 then 2000 then 3000 etc. ]
 with an offset of x000
 
*/

FMX_PROC(errcode) BE_NumericConstants ( short funcId, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = NoError();
	
	try {
		
		SetResult ( funcId % kBE_NumericConstantOffset, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_NumericConstants


FMX_PROC(errcode) BE_TimeFunctions ( const short funcId, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = NoError();
	
	try {
		
		fmx::int64 reply = 0;
		
		switch ( funcId ) {
			
			case kBE_CurrentTimeMilliseconds:
				reply = ptime_to_milliseconds ( boost::posix_time::microsec_clock::local_time() );
				break;
				
			case kBE_UTCMilliseconds:
				reply = ptime_to_milliseconds ( boost::posix_time::microsec_clock::universal_time() );
				break;
				
			case kBE_TimeZoneOffset:
				typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime> local_adjustor;
				
				const boost::posix_time::ptime utc_time = boost::posix_time::second_clock::universal_time();
				const boost::posix_time::ptime local_time = local_adjustor::utc_to_local ( utc_time );
				const boost::posix_time::time_duration offset = local_time - utc_time;
				reply = offset.total_seconds() / 60; // can has minutes
				break;
			
			// should not be here
			// default:
			//	;
				
		}		
		
		SetResult ( reply, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_TimeFunctions



/*
 BE_ExtractScriptVariables implements are somewhat imperfect heuristic for finding
 script variables within chunks of filemaker calculation
 
 try to stip out unwanted text such as strings and comments and then, when a $ is
 found, attempt to guess the where the variable name ends
 */

FMX_PROC(errcode) BE_ExtractScriptVariables ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		BEWStringVector variables;
		WStringAutoPtr calculation = ParameterAsWideString ( parameters, 0 );

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
		
		results.SetAsText( *(variables.AsValueList()), parameters.At(0).GetLocale() );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_ExtractScriptVariables



FMX_PROC(errcode) BE_ExecuteSystemCommand ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr command = ParameterAsUTF8String ( parameters, 0 );
		long timeout = ParameterAsLong ( parameters, 1, kBE_Never );

		StringAutoPtr response ( new string );
		error = ExecuteSystemCommand ( *command, *response, timeout );
		
		SetResult ( response, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	// return a result and set last error when there is a timeout
	if ( error == kCommandTimeout ) {
		g_last_error = kCommandTimeout;
		error = kNoError;
	} else {
		error = MapError ( error );
	}
	
	return error;
	
} // BE_ExecuteSystemCommand



// open the supplied url in the user's default browser

FMX_PROC(errcode) BE_OpenURL ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{	
	errcode error = NoError();
	
	try {
		
		WStringAutoPtr url = ParameterAsWideString ( parameters, 0 );
		bool succeeded = OpenURL ( url );

		SetResult ( succeeded, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}	
	
	return MapError ( error );
	
} // BE_OpenURL



FMX_PROC(errcode) BE_OpenFile ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		// in this instance the string is a better choice than boost::filesystem::path
		WStringAutoPtr path = ParameterAsWideString ( parameters, 0 );
		
		bool succeeded = OpenFile ( path );
		SetResult ( succeeded, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_OpenFile



FMX_PROC(errcode) BE_ExecuteScript ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		TextAutoPtr script_name;
		script_name->SetText ( parameters.AtAsText ( 0 ) );

		FMX_UInt32 number_of_paramters = parameters.Size();
		
		TextAutoPtr file_name;
		if ( number_of_paramters >= 2 ) {
			file_name->SetText ( parameters.AtAsText ( 1 ) );
		}

		// get the parameter, if present		
		DataAutoPtr parameter;
		if ( number_of_paramters == 3 ) {

			// defeat: Returning null reference (within a call to 'operator*')
			// default constructor for default_locale gives the current locale
#ifndef __clang_analyzer__
			LocaleAutoPtr default_locale;
			parameter->SetAsText ( parameters.AtAsText ( 2 ), *default_locale );
#endif

		}

#ifndef __clang_analyzer__
		error = ExecuteScript ( *script_name, *file_name, *parameter, environment );
#endif

		SetResult ( error, results );

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}	
	
	return MapError ( error );
	
} // BE_ExecuteScript



FMX_PROC(errcode) BE_FileMakerSQL ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		TextAutoPtr expression;
		expression->SetText ( parameters.AtAsText(0) );
		
		FMX_UInt32 number_of_paramters = parameters.Size();
		
		TextAutoPtr filename;
		if ( number_of_paramters == 4 ) {
			filename->SetText ( parameters.AtAsText(3) );
		}
		
		BESQLCommandAutoPtr sql ( new BESQLCommand ( expression, filename ) );
		
		if ( number_of_paramters >= 2 ) {
			TextAutoPtr column_separator;
			column_separator->SetText ( parameters.AtAsText(1) );
			sql->set_column_separator ( column_separator );
		}
		
		if ( number_of_paramters >= 3 ) {
			TextAutoPtr row_separator;
			row_separator->SetText ( parameters.AtAsText(2) );
			sql->set_row_separator ( row_separator );
		}
		
		sql->execute ( environment );
		
		SetResult ( *(sql->get_text_result()), results );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_FileMakerSQL



FMX_PROC(errcode) BE_MessageDigest ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{	
	errcode error = NoError();
		
	try {
		
		StringAutoPtr message = ParameterAsUTF8String ( parameters, 0 );
		const unsigned long algorithm = ParameterAsLong( parameters, 1, kBE_MessageDigestAlgorithm_SHA256 );
		const unsigned long output_type = ParameterAsLong( parameters, 2, kBE_Encoding_Hex );

		string digest = message_digest ( *message, algorithm, output_type );
		
		SetResult ( digest, results );
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}	
	
	return MapError ( error );
	
} // BE_MessageDigest



FMX_PROC(errcode) BE_HMAC ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		StringAutoPtr message = ParameterAsUTF8String ( parameters, 0 );
		StringAutoPtr key = ParameterAsUTF8String ( parameters, 1 );
		const unsigned long algorithm = ParameterAsLong ( parameters, 2, kBE_MessageDigestAlgorithm_SHA1 );
		const unsigned long output_type = ParameterAsLong ( parameters, 3, kBE_Encoding_Hex );
		const unsigned long input_type = ParameterAsLong ( parameters, 4, kBE_Encoding_None );

		string hmac = HMAC ( *message, algorithm, output_type, *key, input_type );

		SetResult ( hmac, results );
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_HMAC

			
FMX_PROC(errcode) BE_JPEG_Recompress ( const short function_id, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		vector<unsigned char> original_jpeg = ParameterAsVectorUnsignedChar ( parameters );

		if ( original_jpeg.size() > 0 ) {

			auto_ptr<BEJPEG> jpeg ( new BEJPEG ( original_jpeg ) );
			const int quality = (const int)ParameterAsLong ( parameters, 1, 75 ); // percent
			jpeg->set_compression_level ( quality );

			if ( function_id == kBE_JPEG_Recompress ) {
				
				const double scale = ParameterAsDouble ( parameters, 2 );
				jpeg->set_scaling ( scale );
				
			} else {
				
				// depreciated
			
				const int width = (const int)ParameterAsLong ( parameters, 2, 0 );
				jpeg->set_width ( width );

				const int height = (const int)ParameterAsLong ( parameters, 3, 0 );
				jpeg->set_height ( height );

			}

			jpeg->recompress();

			const StringAutoPtr image_name = ParameterFileName ( parameters );
			SetResult ( *image_name, *jpeg, results );

		} else {
			; // nothing... just do what fmp does
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_JPEG_Recompress


FMX_PROC(errcode) BE_ConvertContainer ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		const BinaryDataAutoPtr data ( parameters.AtAsBinaryData ( 0 ) );
		if ( data->GetCount() > 0 ) {

			vector<char> container_data = ParameterAsVectorChar ( parameters );
			StringAutoPtr filename = ParameterFileName ( parameters );

			StringAutoPtr type = ParameterAsUTF8String ( parameters, 1, FILE_CONTAINER_TYPE );

			const unsigned long width = ParameterAsLong ( parameters, 2, kErrorUnknown );
			const unsigned long height = ParameterAsLong ( parameters, 3, kErrorUnknown );

			SetResult ( *filename, container_data, *type, width, height, results );

		} else {
			error = kInvalidFieldType;
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ConvertContainer


FMX_PROC(errcode) BE_RegularExpression ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

        const StringAutoPtr text = ParameterAsUTF8String ( parameters );
        const StringAutoPtr expression = ParameterAsUTF8String ( parameters, 1 );
		const StringAutoPtr options = ParameterAsUTF8String ( parameters, 2 );
        const StringAutoPtr replace_with = ParameterAsUTF8String ( parameters, 3 );
        const bool replace = parameters.Size() == 4 ;
        
        BEValueList<std::string> matched = regular_expression ( *text, *expression, *options, *replace_with, replace );
        std::string matched_text = matched.get_as_filemaker_string();
        // add new setresult for value lists
        SetResult ( matched_text, results );
        
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_RegularExpression


FMX_PROC(errcode) BE_Pause ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& /* results */ )
{
	errcode error = NoError();
				
	try {
		
		const long milliseconds = ParameterAsLong ( parameters );
		
//		std::this_thread::sleep_for ( std::chrono::milliseconds ( milliseconds ) ); // c++11
		boost::this_thread::sleep ( boost::posix_time::milliseconds ( milliseconds ) );
		
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
				
	return MapError ( error );
				
} // BE_Pause
			
			
			
