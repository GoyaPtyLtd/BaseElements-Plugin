/*
 BEPluginFunctions.cpp
 BaseElements Plug-In

 Copyright 2010-2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEPluginGlobalDefines.h"


#if defined FMX_MAC_TARGET

	#include "BEMacFunctions.h"

#elif FMX_IOS_TARGET

	#include "BEIOSFunctions.h"

#elif defined FMX_WIN_TARGET

	#include <locale.h>
	#include "BEWinFunctions.h"

#elif defined FMX_LINUX_TARGET

	#include "BELinuxFunctions.h"

#endif


#include "BEPluginFunctions.h"

#include "BEXSLT.h"
#include "BECurl.h"
#include "BEFileSystem.h"
#include "BEFileMakerPlugin.h"
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
#include "Crypto/BEBase64.h"
#include "Crypto/BEMessageDigest.h"
#include "Crypto/BEOpenSSLAES.h"
#include "Crypto/BEOpenSSLRSA.h"
#include "Crypto/BEX509.h"
#include "BEPluginException.h"
#include "BEQuadChar.h"
#include "BEXero.h"
#include "BEXMLSchema.h"
#include "BESMTP.h"
#include "BEJavaScript.h"
#include "Images/BEJPEG.h"
#include "BERegularExpression.h"
#include "BESMTPContainerAttachments.h"

#include <numeric> // for inner_product
#include <list>
#include <thread>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

#include <Poco/Exception.h>
#include <Poco/RegularExpression.h>
#include <Poco/String.h>

#include <iconv.h>

#include <podofo/podofo.h>

using namespace std;
using namespace fmx;
using namespace boost::filesystem;


#pragma mark -
#pragma mark Globals
#pragma mark -

#define GZIP_FILE_EXTENSION string ( ".gz" )


std::map<short, std::string> g_script_steps;
extern BEFileMakerPlugin * g_be_plugin;

thread_local errcode g_last_error;
thread_local errcode g_last_ddl_error;
string g_text_encoding;
string g_json_error_description;
BEOAuth * g_oauth;
struct host_details g_smtp_host;
BESMTPContainerAttachments g_smtp_attachments;
vector<BEValueListStringSharedPtr> arrays;

extern int g_http_response_code;
extern string g_http_response_headers;
extern CustomHeaders g_http_custom_headers;
extern CustomHeaders g_smtp_custom_headers;
extern std::stringstream g_curl_trace;
extern struct host_details g_http_proxy;
extern BECurlOptionMap g_curl_options;


#pragma mark -
#pragma mark Version
#pragma mark -

fmx::errcode BE_Version ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	return TextConstantFunction ( VERSION_NUMBER_STRING, results );
}


fmx::errcode BE_VersionAutoUpdate ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	return TextConstantFunction ( AUTO_UPDATE_VERSION, results );
}


#pragma mark -
#pragma mark Errors
#pragma mark -


fmx::errcode BE_GetLastError ( short funcId, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = kNoError; // do not use NoError();

	try {

		if ( funcId == kBE_GetLastError ) {
			SetResult ( g_last_error, results );
		} else if ( funcId == kBE_GetLastDDLError ) {
			SetResult ( g_last_ddl_error, results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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

fmx::errcode BE_ClipboardFormats ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	return TextConstantFunction ( ClipboardFormats(), results );
}


fmx::errcode BE_ClipboardData ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto atype = ParameterAsWideString ( parameters );
		auto clipboard_contents = ClipboardData ( atype );
		SetResult ( clipboard_contents, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ClipboardData


fmx::errcode BE_SetClipboardData ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		std::string to_copy = ParameterAsUTF8String ( parameters );
		std::wstring atype = ParameterAsWideString ( parameters, 1 );
		bool success = SetClipboardData ( to_copy, atype );
		SetResult ( success, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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


fmx::errcode BE_CreateFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		path directory_path = ParameterAsPath ( parameters );

		try {
			create_directories ( directory_path );
		} catch ( filesystem_error& e ) {
			g_last_error = e.code().value();
		}

		SetResult ( g_last_error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_CreateFolder


fmx::errcode BE_DeleteFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		path path = ParameterAsPath ( parameters );

		try {
			remove_all ( path ); // if path is a directory then path and all it's contents are deleted
		} catch ( filesystem_error& e ) {
			g_last_error = e.code().value();
		}

		SetResult ( g_last_error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_DeleteFile


fmx::errcode BE_FileExists ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		path path = ParameterAsPath ( parameters );

		bool file_exists = exists ( path );

		SetResult ( file_exists, results );

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

} // BE_FileExists



fmx::errcode BE_FileSize ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto path = ParameterAsPath ( parameters );
		auto size = file_or_directory_size ( path );

		SetResult ( size, results );

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

} // BE_FileSize


fmx::errcode BE_File_Modification_Timestamp ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
    errcode error = NoError();

    try {

        const path path = ParameterAsPath ( parameters );

        const std::time_t last_modified = boost::filesystem::last_write_time ( path );
		const fmx::int64 timestamp = std_time_to_timestamp ( last_modified );

		SetResult ( timestamp, results );

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

} // BE_File_Modification_Timestamp


fmx::errcode BE_ReadTextFromFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto file_contents = ParameterPathOrContainerAsUTF8 ( parameters );
		SetResult ( file_contents, results );

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


fmx::errcode BE_WriteTextToFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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
		
		std::string text_to_write = ParameterAsUTF8String ( parameters, 1 );
		vector<char> out = ConvertTextEncoding ( (char *)text_to_write.c_str(), text_to_write.size(), g_text_encoding, UTF8 );
		
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


fmx::errcode BE_WriteTextFileToContainer ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		auto file_name = ParameterFileName ( parameters );
		
		if ( !file_name.empty() ) {
			
			std::string text_to_write;
			
			// should the text be appended to the file or replace any existing contents
			
			auto append = ParameterAsBoolean ( parameters, 2, false );
			if ( append && BinaryDataAvailable ( parameters )) {
				text_to_write = ParameterPathOrContainerAsUTF8 ( parameters );
			}
			
			text_to_write += ParameterAsUTF8String ( parameters, 1 );
			
			auto out = ConvertTextEncoding ( (char *)text_to_write.c_str(), text_to_write.size(), g_text_encoding, UTF8 );
			
			if ( !out.empty() ) {
				SetResult ( file_name, out, results );
			}
			
		} else {
			error = kRequestedDataIsMissingError;
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_WriteTextFileToContainer


/*
 filemaker can create DDRs which contain utf-16 characters that are not
 valid in an XML document. reads the DDR and writes out a new one, skipping
 any invalid characters, and replaces the old file
 */

fmx::errcode BE_StripInvalidUTF16CharactersFromXMLFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	auto overwriting = false;

	try {

		auto source = ParameterAsPath ( parameters );
		auto destination = ParameterAsPath ( parameters, 1, source );
		if ( destination == source ) {
			destination += TEMPORARY_FILE_SUFFIX;
			overwriting = true;
		}
		
		boost::uintmax_t length = file_size ( source ); // throws if the source does not exist

		boost::filesystem::ifstream input_file ( source, ios_base::in | ios_base::binary | ios_base::ate );
		input_file.seekg ( 0, ios::beg );
		boost::filesystem::ofstream output_file ( destination, ios_base::out | ios_base::binary | ios_base::ate );

		const size_t size = 2; // read (and write) 2 bytes at a time
		boost::uintmax_t skipped = 0;
		auto big_endian = true;
		auto write_output = false;

		for ( boost::uintmax_t i = 0; i < length; i += size ) {

			char codepoint[size];
			input_file.read ( codepoint, size );

			// check the bom, if present, to determin if the utf-16 if big or little endian
			if ( (i == 0) && ((unsigned char)codepoint[0] == 0xff && (unsigned char)codepoint[1] == 0xfe ) ) {
				big_endian = false;
			}

			// swap the byte order for big-endian files
			unichar16 * utf16 = (unichar16 *)codepoint;
			char byte_swapped[size];
			if ( big_endian ) {
				byte_swapped[0] = codepoint[1];
				byte_swapped[1] = codepoint[0];
				utf16 = (unichar16 *)byte_swapped;
			}

			// only check for 'control characters'
			if ( (*utf16 == 0x9) || // horizontal tab
				(*utf16 == 0xA) ||	// line feed
				(*utf16 == 0xD) ||	// carriage return
				(*utf16 >= 0x20) ) {

				if ( write_output ) {
					output_file.write ( codepoint, size );
				}
				
			} else {
				
				if ( write_output ) {
					skipped += size;
				} else {
					// reset the loop and start again but writing the output
					write_output = true;
					i = -size;
					input_file.seekg ( 0, ios::beg );
				}
				
			}
		}

		output_file.close();
		input_file.close();

		/*
		 only replace the file if that we've skipped some characters and
		 the output file is the right size
		 */

		if ( (skipped > 0) && (length == (file_size ( destination ) + skipped)) ) {
			
			if ( overwriting ) {
				remove_all ( source );
				rename ( destination, source );
			}

		} else {

			remove_all ( destination );
			if ( skipped > 0 ) {
				// if characters were skipped and the file size is wrong report an error
				error = kFileSystemError;
			}

		}

		SetResult ( skipped, results );

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

} // BE_StripInvalidUTF16CharactersFromXMLFile



fmx::errcode BE_ExportFieldContents ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto field_contents = ParameterAsVectorChar ( parameters );

		path destination;
		
		auto number_of_parameters = parameters.Size();
		if ( number_of_parameters == 2 ) {
			destination = ParameterAsPath ( parameters, 1 );
		} else {
			// write out a temporary file
			destination = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
		}

		error = write_to_file ( destination, field_contents );

		SetResult ( destination.wstring(), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ExportFieldContents


fmx::errcode BE_ImportFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto from = ParameterAsPath ( parameters );
		auto compress = ParameterAsBoolean ( parameters, 1, false );
		std::string data_type = FILE_CONTAINER_TYPE;
		if ( compress ) {
			data_type = COMPRESSED_CONTAINER_TYPE;
		}

		auto file_data = ReadFileAsBinary ( from );

		SetResult ( from.filename().string(), file_data, results, data_type );

	} catch ( boost::filesystem::ifstream::failure& /* e */ ) {
		error = errno; // cannot read the file
	} catch ( boost::filesystem::filesystem_error& e ) {
		error = e.code().value();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ImportFile


fmx::errcode BE_MoveFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		path from = ParameterAsPath ( parameters );
		path to = ParameterAsPath ( parameters, 1 );

		try {
			rename ( from, to );
		} catch ( filesystem_error& e ) {
			g_last_error = e.code().value();
		}

		SetResult ( g_last_error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_MoveFile


fmx::errcode BE_CopyFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		path from = ParameterAsPath ( parameters );
		path to = ParameterAsPath ( parameters, 1 );

		try {
			recursive_directory_copy ( from, to );
		} catch ( filesystem_error& e ) {
			g_last_error = e.code().value();
		}

		SetResult ( g_last_error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_CopyFile



fmx::errcode BE_ListFilesInFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		const path directory = ParameterAsPath ( parameters );
		const long file_type_wanted = ParameterAsLong ( parameters, 1, kBE_FileType_File );
		const bool include_subfolders = ParameterAsBoolean ( parameters, 2, false );
		const bool use_full_path = ParameterAsBoolean ( parameters, 3, false );
		const bool include_hidden = ParameterAsBoolean ( parameters, 4, true );

		try {

			BEValueListWideStringUniquePtr list_of_files ( list_files_in_directory ( directory, file_type_wanted, include_subfolders, include_hidden ) );
			if ( ! use_full_path ) {
				list_of_files->remove_prefix ( directory.wstring() );
			}
			SetResult ( list_of_files->get_as_filemaker_string(), results );

		} catch ( BEPlugin_Exception& e ) {
			error = e.code();
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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

fmx::errcode BE_SelectFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		std::wstring prompt = ParameterAsWideString ( parameters );
		std::wstring inFolder = ParameterAsWideString ( parameters, 1 );

		std::wstring files ( SelectFile ( prompt, inFolder ) );

		SetResult ( files, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_SelectFile


fmx::errcode BE_SelectFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		std::wstring prompt = ParameterAsWideString ( parameters );
		std::wstring inFolder = ParameterAsWideString ( parameters, 1 );

		std::wstring folder = SelectFolder ( prompt, inFolder );

		SetResult ( folder, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_SelectFolder


fmx::errcode BE_SaveFileDialog ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		std::wstring prompt = ParameterAsWideString ( parameters );
		std::wstring fileName = ParameterAsWideString ( parameters, 1 );
		std::wstring inFolder = ParameterAsWideString ( parameters, 2 );

		std::wstring folder = SaveFileDialog ( prompt, fileName, inFolder );

		SetResult ( folder, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_SelectFolder


fmx::errcode BE_DisplayDialog ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		std::wstring title = ParameterAsWideString ( parameters );
		std::wstring message = ParameterAsWideString ( parameters, 1 );
		std::wstring ok_button = ParameterAsWideString ( parameters, 2 );
		std::wstring cancel_button = ParameterAsWideString ( parameters, 3 );
		std::wstring alternate_button = ParameterAsWideString ( parameters, 4 );

		int response = DisplayDialog ( title, message, ok_button, cancel_button, alternate_button );
		SetResult ( response, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_DisplayDialog



fmx::errcode BE_ProgressDialog ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& /* results */ )
{
	errcode error = NoError();

	try {

		std::wstring title = ParameterAsWideString ( parameters );
		std::wstring description = ParameterAsWideString ( parameters, 1 );
		long maximum = ParameterAsLong ( parameters, 2, 0 ); // 0 == indeterminite

		// allow the user to cancel ?
		bool can_cancel = AllowUserAbort ( environment );

		// display the progress bar
		error = DisplayProgressDialog ( title, description, maximum, can_cancel );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error, true);

} // BE_ProgressDialog


fmx::errcode BE_ProgressDialog_Update ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& /* results */ )
{
	errcode error = NoError();

	try {

		long value = ParameterAsLong ( parameters, 0, 0 );
		value = value < 0 ? 0 : value;

		auto description = ParameterAsWideString ( parameters, 1 );

		error = UpdateProgressDialog ( value, description );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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


fmx::errcode BE_ApplyXSLT ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto xml = ParameterPathOrContainerAsUTF8 ( parameters );
		auto xml_path = ParameterAsPath ( parameters );
		auto xslt = ParameterAsUTF8String ( parameters, 1 );
		auto csv_path = ParameterAsPath ( parameters, 2 );
	
		auto csv = ApplyXSLTInMemory ( xml, xslt, csv_path, xml_path );
		
		SetResult ( csv, results );
		

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ApplyXSLT


fmx::errcode BE_ApplyXSLTInMemory ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto xml = ParameterAsUTF8String ( parameters );
		auto xslt = ParameterAsUTF8String ( parameters, 1 );

		auto xslt_result = ApplyXSLTInMemory ( xml, xslt );
		SetResult ( xslt_result, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ApplyXSLTInMemory


fmx::errcode BE_XPath ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto xml = ParameterAsUTF8String ( parameters );
		auto xpath = ParameterAsUTF8String ( parameters, 1 );
		std::string ns_list;

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

		auto xpath_result = ApplyXPathExpression ( xml, xpath, ns_list, xpath_object_type );
		SetResult ( xpath_result, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_XPath


fmx::errcode BE_XPathAll ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	TextUniquePtr text;

	try {

		auto xml = ParameterAsUTF8String ( parameters );
		auto xpath = ParameterAsUTF8String ( parameters, 1 );

		const unsigned long number_of_parameters = parameters.Size();
		std::string ns_list;
		if ( number_of_parameters > 2 ) {
			ns_list = ParameterAsUTF8String ( parameters, 2 );
		}

		auto xpath_result = ApplyXPathExpression ( xml, xpath, ns_list, XPATH_NODESET );
		SetResult ( xpath_result, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_XPathAll



fmx::errcode BE_StripXMLNodes ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();

	try {

		path input_file = ParameterAsPath ( parameters );
		path output_file = ParameterAsPath ( parameters, 1 );
		std::string node_names = ParameterAsUTF8String ( parameters, 2 );

		vector<string> node_names_vector;
		boost::tokenizer<> tokeniser ( node_names );
		for ( boost::tokenizer<>::iterator it = tokeniser.begin() ; it != tokeniser.end() ; ++it ) {
			node_names_vector.push_back ( *it );
		}

		error = StripXMLNodes ( input_file, output_file, node_names_vector );
		SetResult ( error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_StripXMLNodes



fmx::errcode BE_XML_Parse ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();

	try {

		path input_file = ParameterAsPath ( parameters );

		unique_ptr<BEXMLTextReader> reader ( new BEXMLTextReader ( input_file ) );
		string result = reader->parse();

		SetResult ( result, results );

	} catch ( BEXMLReaderInterface_Exception& e ) {
		error = e.code();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_XML_Parse



fmx::errcode BE_SplitBEFileNodes ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();

	try {

		path input_file = ParameterAsPath ( parameters );

		int result = SplitBEXMLFiles ( input_file );

		SetResult ( result, results );

	} catch ( BEXMLReaderInterface_Exception& e ) {
		error = e.code();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_SplitBEFileNodes


fmx::errcode BE_XML_Validate ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();
	
	try {
		
		auto xml = ParameterAsUTF8String ( parameters );
		auto schema = ParameterAsUTF8String ( parameters, 1 );
		
		auto result = validate_xml ( xml, schema );
		
		SetResult ( result, results );
		
	} catch ( BEXMLReaderInterface_Exception& e ) {
		error = e.code();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_XML_Validate



#pragma mark -
#pragma mark JSON
#pragma mark -


fmx::errcode BE_JSONPath_Deprecated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	g_json_error_description.clear();

	try {

		auto json = ParameterAsUTF8String ( parameters );
		auto json_path_expression = ParameterAsUTF8String ( parameters, 1 );

		std::unique_ptr<BEJSON> json_document ( new BEJSON ( json ) );
		json_document->json_path_query ( json_path_expression, results );

	} catch ( BEJSON_Exception& e ) {
		error = e.code();
		g_json_error_description = e.description();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_JSONPath_Deprecated



// do not set the global last error

fmx::errcode BE_JSON_Error_Description_Deprecated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = kNoError;

	try {

		SetResult ( g_json_error_description, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return error;

} // BE_JSON_Error_Description_Deprecated



fmx::errcode BE_JSON_ArraySize_Deprecated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	g_json_error_description.clear();

	try {

		auto json = ParameterAsUTF8String ( parameters );

		std::unique_ptr<BEJSON> json_document ( new BEJSON ( json ) );
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

} // BE_JSON_ArraySize_Deprecated



fmx::errcode BE_JSON_Encode_Deprecated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	g_json_error_description.clear();

	try {

		auto key = ParameterAsUTF8String ( parameters );
		auto type = ParameterAsUTF8String ( parameters, 2 ); // allows the developer to specify null (only)

		if ( parameters.Size() == 1 ) {

			const string out = "\"" + key + "\":";
			SetResult ( out, results );

		} else {

			std::unique_ptr<BEJSON> json_document ( new BEJSON ( ) );
			auto json = json_document->encode ( key, parameters.At ( 1 ), type );
			json.erase ( 0, 1 ); // remove {
			json.erase ( json.length() - 1 ); // remove }
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

} // BE_JSON_Encode_Deprecated



#pragma mark -
#pragma mark JavaScript
#pragma mark -

// EXPERIMENTAL !!!
// Use at your own risk

fmx::errcode BE_EvaluateJavaScript ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto javaScript = ParameterAsUTF8String ( parameters );

		auto jsResult = Evaluate_JavaScript ( javaScript );

		SetResult ( jsResult, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_EvaluateJavaScript


#pragma mark -
#pragma mark Arrays
#pragma mark -


fmx::errcode BE_ArraySetFromValueList ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );
		BEValueListStringSharedPtr array ( new BEValueList<string> ( value_list ) );

		arrays.push_back ( array );
		const size_t size = arrays.size();

		SetResult ( size, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ArraySetFromList


fmx::errcode BE_ArrayGetSize ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto array_id = ParameterAsIndex ( parameters );

		try {

			const size_t array_size = ( arrays.at ( array_id ) )->size();
			SetResult ( array_size, results );

		} catch ( out_of_range& /* e */ ) {
			; // we not returnin' anyting
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ArrayGetSize


fmx::errcode BE_ArrayGetValue ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto array_id = ParameterAsIndex ( parameters );
		auto value_id = ParameterAsIndex ( parameters, 1 );

		string value;

		try {
			value = ( arrays.at ( array_id ) )->at ( value_id );
		} catch ( out_of_range& /* e */ ) {
			; // we already empty string
		}

		SetResult ( value, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ArrayGetValue


fmx::errcode BE_Array_Delete ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();

	try {

		auto array_id = ParameterAsIndex ( parameters );

		try {
			auto unwanted = arrays.at ( array_id ); // so we throw if the index is invalid
			arrays.erase ( arrays.begin () + array_id );
		} catch ( out_of_range& /* e */ ) {
			; // if we don't find it don't error
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Array_Delete


fmx::errcode BE_Array_Find ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();
	
	try {
		
		auto array_id = ParameterAsIndex ( parameters );
		
		try {
			auto wanted = arrays.at ( array_id ); // so we throw if the index is invalid
			auto find_this = ParameterAsUTF8String ( parameters, 1 );
			auto found = wanted->find ( find_this );
			SetResult ( found.get_as_filemaker_string(), results );
		} catch ( out_of_range& /* e */ ) {
			; // if we don't find it don't error
		}
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Array_Find


fmx::errcode BE_Array_Change_Value ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();
	
	try {
		
		auto array_id = ParameterAsIndex ( parameters );
		
		try {
			
			auto wanted = arrays.at ( array_id ); // so we throw if the index is invalid
			auto find_this = ParameterAsIndex ( parameters, 1 );
			auto replace_with = ParameterAsUTF8String ( parameters, 2 );
			auto changed = wanted->change_value ( find_this, replace_with );
			SetResult ( changed, results );
			
		} catch ( out_of_range& /* e */ ) {
			; // if we don't find it don't error
		}
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Array_Change_Value



#pragma mark -
#pragma mark User Preferences
#pragma mark -


fmx::errcode BE_SetPreference ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto key = ParameterAsWideString ( parameters );
		auto value = ParameterAsWideString ( parameters, 1 );
		auto domain = ParameterAsWideString ( parameters, 2 );

		if ( domain.empty() ) {
			domain.assign ( USER_PREFERENCES_DOMAIN );
		}

		SetResult ( SetPreference ( key, value, domain ), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_SetPreference



fmx::errcode BE_GetPreference ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto key = ParameterAsWideString ( parameters );
		auto domain = ParameterAsWideString ( parameters, 1 );

		if ( domain.empty() ) {
			domain.assign ( USER_PREFERENCES_DOMAIN );
		}

		SetResult ( GetPreference ( key, domain ), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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



fmx::errcode BE_Unzip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto archive = ParameterAsUTF8String ( parameters );
		auto output_directory = ParameterAsUTF8String ( parameters, 1 );

		error = (fmx::errcode)UnZip ( archive, output_directory );
		SetResult ( error, results );

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

} // BE_Unzip



fmx::errcode BE_Zip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto files  = new const BEValueList<string> ( ParameterAsUTF8String ( parameters ) );
		auto output_directory = ParameterAsUTF8String ( parameters, 1 );

		error = (fmx::errcode)Zip ( files, output_directory );
		SetResult ( error, results );

		delete files;

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

} // BE_Zip



fmx::errcode BE_Base64_Decode_Deprecated ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto text = ParameterAsUTF8String ( parameters );
		auto filename = ParameterAsUTF8String ( parameters, 1 );

		// decode it...
		vector<char> data = Base64_Decode ( text );
		if ( filename.empty() ) {
			SetResult ( data, results );
		} else {
			SetResult ( filename, data, results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Base64_Decode_Deprecated



fmx::errcode BE_Base64_Encode_Deprecated ( short funcId, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto data = ParameterAsVectorChar ( parameters );
		auto base64 = Base64_Encode ( data, funcId == kBE_Base64_URL_Encode_Deprecated );

		SetResult ( base64, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Base64_Encode_Deprecated



fmx::errcode BE_SetTextEncoding ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto encoding = ParameterAsUTF8String ( parameters );

		iconv_t conversion = iconv_open ( encoding.c_str(), encoding.c_str() );
		if ( conversion != (iconv_t)-1 ) {
			SetTextEncoding ( encoding );
			iconv_close ( conversion );
		 } else {
			error = errno;
		 }


		SetResult ( error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_SetTextEncoding


fmx::errcode BE_ContainerIsCompressed ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		if ( (parameters.At(0)).GetNativeType() == fmx::Data::kDTBinary ) {

			const BinaryDataUniquePtr data_stream ( parameters.AtAsBinaryData ( 0 ) );

			bool compressed = StreamIsCompressed ( *data_stream );

			SetResult ( compressed, results );

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

} // BE_ContainerIsCompressed


fmx::errcode BE_ContainerCompress ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto to_compress = ParameterAsVectorChar ( parameters );
		auto filename = ParameterAsUTF8String ( parameters, 1 );

		SetResult ( filename, to_compress, results, COMPRESSED_CONTAINER_TYPE );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ContainerCompress


fmx::errcode BE_ContainerUncompress ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto gzipped = ParameterAsVectorChar ( parameters );
		auto filename = ParameterAsUTF8String ( parameters, 1 );

		SetResult ( filename, gzipped, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ContainerUncompress


fmx::errcode BE_Gzip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto to_compress = ParameterAsVectorChar ( parameters );
		if ( !to_compress.empty() ) {

			const vector<char> compressed = CompressContainerStream ( to_compress );

			path filename = ParameterAsPath ( parameters, 1 );
			if ( filename.empty() ) {
				filename = ParameterFileName ( parameters );
				filename += GZIP_FILE_EXTENSION;
			}

			SetResult ( filename.string(), compressed, results );

		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Gzip


fmx::errcode BE_UnGzip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		const vector<char> gzipped = ParameterAsVectorChar ( parameters );
		if ( !gzipped.empty() ) {

			const vector<char> uncompressed = UncompressContainerStream ( gzipped );

			path filename = ParameterAsPath ( parameters, 1 );
			if ( filename.empty() ) {
				filename = ParameterFileName ( parameters );
				if ( filename.extension() == GZIP_FILE_EXTENSION ) {
					filename.replace_extension ( "" );
				}
			}

			SetResult ( filename.string(), uncompressed, results );

		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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


fmx::errcode BE_Encrypt_AES_Deprecated ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto password = ParameterAsUTF8String ( parameters );
		auto text = ParameterAsUTF8String ( parameters, 1 );

		string key;
		vector<char> input_vector;
		GenerateKeyAndInputVector ( password, key, input_vector );

		// escape the delimiter we use below
		replace ( input_vector.begin(), input_vector.end(), FILEMAKER_END_OF_LINE_CHAR, '\n' );

		vector<char> output_to_encode ( input_vector.begin(), input_vector.end() );
		output_to_encode.push_back ( FILEMAKER_END_OF_LINE_CHAR );

		vector<char> encrypted_data = Encrypt_AES ( key, text, input_vector );
		output_to_encode.insert ( output_to_encode.end(), encrypted_data.begin(), encrypted_data.end() );

		auto base64 = Base64_Encode ( output_to_encode );
		SetResult ( base64, results );


	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Encrypt_AES_Deprecated



fmx::errcode BE_Decrypt_AES_Deprecated ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto password = ParameterAsUTF8String ( parameters );
		auto text = ParameterAsUTF8String ( parameters, 1 );

		string key;
		vector<char> unwanted;
		GenerateKeyAndInputVector ( password, key, unwanted );

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

} // BE_Decrypt_AES_Deprecated


fmx::errcode BE_SignatureGenerate_RSA ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto data = ParameterAsVectorUnsignedChar ( parameters );
		auto privateKey = ParameterAsUTF8String ( parameters, 1 );
		auto privateKeyPassword = ParameterAsUTF8String ( parameters, 2 );
		auto digestName = ParameterAsUTF8String ( parameters, 3 );
		auto filename = ParameterAsUTF8String ( parameters, 4 );

		// Line separator of RSA key must be LF.
		replace( privateKey.begin(), privateKey.end(), FILEMAKER_END_OF_LINE_CHAR, '\n' );

		vector<char> encrypted_data = SignatureGenerate_RSA ( data, privateKey, digestName, privateKeyPassword );
		if ( filename.empty() ) {
			vector<char> v(encrypted_data.begin(), encrypted_data.end());
			auto base64 = Base64_Encode(v);
			SetResult ( base64, results );
		} else {
			SetResult ( filename, encrypted_data, results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );

} // BE_SignatureGenerate_RSA

fmx::errcode BE_SignatureVerify_RSA ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto data = ParameterAsVectorUnsignedChar ( parameters );
		auto publicKey = ParameterAsUTF8String ( parameters, 1 );
		vector<char> signature;
		if ( BinaryDataAvailable ( parameters, 2 ) ) {
			signature = ParameterAsVectorChar ( parameters, 2 );
		} else {
			signature = Base64_Decode ( ParameterAsUTF8String ( parameters, 2 ) );
		}
		auto digestName = ParameterAsUTF8String ( parameters, 3 );

		// Line separator of RSA key must be LF.
		replace( publicKey.begin(), publicKey.end(), FILEMAKER_END_OF_LINE_CHAR, '\n' );

		bool result = SignatureVerify_RSA ( data, publicKey, signature, digestName );
		SetResult ( result, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_SignatureGenerate_RSA


fmx::errcode BE_CipherEncrypt ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		auto cipher_name = ParameterAsUTF8String ( parameters );
		auto data = HexOrContainer ( parameters, 1 );
		auto key = HexOrContainer ( parameters, 2 );
		auto iv = HexOrContainer ( parameters, 3 );
		auto padding = ParameterAsBoolean ( parameters, 4, true );
		auto filename = ParameterAsUTF8String ( parameters, 5 );

		vector<char> encrypted_data = CipherEncrypt ( cipher_name, data, key, iv, padding );
		
		if ( filename.empty() ) {
			vector<char> encrypted;
			boost::algorithm::hex ( encrypted_data.begin(), encrypted_data.end(), back_inserter ( encrypted ) );
			SetResult ( encrypted, results );
		} else {
			SetResult ( filename, encrypted_data, results );
		}
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Encrypt_Cipher


fmx::errcode BE_CipherDecrypt ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		auto cipher_name = ParameterAsUTF8String ( parameters );
		auto encrypted_data = HexOrContainer ( parameters, 1 );
		auto key = HexOrContainer ( parameters, 2 );
		auto iv = HexOrContainer ( parameters, 3 );
		auto padding = ParameterAsBoolean ( parameters, 4, true );
		auto filename = ParameterAsUTF8String ( parameters, 5 );

		vector<char> decrypted_data = CipherDecrypt ( cipher_name, encrypted_data, key, iv, padding );
		
		if ( filename.empty() ) {
			vector<char> decrypted;
			boost::algorithm::hex ( decrypted_data.begin(), decrypted_data.end(), back_inserter ( decrypted ) );
			SetResult ( decrypted, results );
		} else {
			SetResult ( filename, decrypted_data, results );
		}
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_Decrypt_Cipher



#pragma mark -
#pragma mark HTTP / Curl
#pragma mark -


fmx::errcode BE_HTTP_GET ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto url = ParameterAsUTF8String ( parameters );
		auto filename = ParameterAsUTF8String ( parameters, 1 );
		auto username = ParameterAsUTF8String ( parameters, 2 );
		auto password = ParameterAsUTF8String ( parameters, 3 );

		// not saving to file so do not supply the filename here
		BECurl curl ( url, kBE_HTTP_METHOD_GET, "", username, password );
		vector<char> data = curl.download ( );
		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( filename, data, results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_HTTP_GET


fmx::errcode BE_HTTP_GET_File ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& /* results */ )
{
	errcode error = NoError();

	try {

		auto url = ParameterAsUTF8String ( parameters );
		path filename = ParameterAsPath ( parameters, 1 );
		auto username = ParameterAsUTF8String ( parameters, 2 );
		auto password = ParameterAsUTF8String ( parameters, 3 );

		BECurl curl ( url, kBE_HTTP_METHOD_GET, filename, username, password );
		vector<char> data = curl.download ( );
		error = g_last_error;

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_HTTP_GET_File


fmx::errcode BE_HTTP_POST_PUT_PATCH ( short funcId, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto url = ParameterAsUTF8String ( parameters );
		auto username = ParameterAsUTF8String ( parameters, 2 );
		auto password = ParameterAsUTF8String ( parameters, 3 );

		std::string container_filename;
		
		vector<char> response;

		if ( funcId == kBE_HTTP_POST ) {

			auto post_parameters = ParameterAsUTF8String ( parameters, 1 );
			container_filename = ParameterAsUTF8String ( parameters, 4 );
			BECurl curl ( url, kBE_HTTP_METHOD_POST, "", username, password, post_parameters );
			response =  curl.download ( );


		} else if ( funcId == kBE_HTTP_PATCH ) {

			auto post_parameters = ParameterAsUTF8String ( parameters, 1 );
			BECurl curl ( url, kBE_HTTP_METHOD_PATCH, "", username, password, post_parameters );
			response =  curl.download ( );


		} else if ( funcId == kBE_HTTP_PUT_File ) {

			path filename = ParameterAsPath ( parameters, 1 );
			BECurl curl ( url, kBE_HTTP_METHOD_PUT, filename, username, password );
			response = curl.http_put ( );


		} else { // kBE_HTTP_PUT_DATA

			vector<char> data = ParameterAsVectorChar ( parameters, 1 );
			BECurl curl ( url, kBE_HTTP_METHOD_PUT, "", username, password, "", data );
			response = curl.http_put ( );

		}

		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( container_filename, response, results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_HTTP_POST_PUT_PATCH



fmx::errcode BE_HTTP_DELETE ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto url = ParameterAsUTF8String ( parameters );
		auto username = ParameterAsUTF8String ( parameters, 1 );
		auto password = ParameterAsUTF8String ( parameters, 2 );

		BECurl curl ( url, kBE_HTTP_METHOD_DELETE, "", username, password );
		vector<char> data = curl.http_delete ( );
		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( data, results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_HTTP_DELETE



fmx::errcode BE_HTTP_Response_Code ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = NoError();

	try {

		SetResult ( g_http_response_code, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_HTTP_Response_Code



fmx::errcode BE_HTTP_Response_Headers ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		const unsigned long number_of_parameters = parameters.Size();

		if ( number_of_parameters == 0 ) { // get them all

			auto headers ( g_http_response_headers );
			SetResult ( headers, results );

		} else {

			auto http_headers = boost::replace_all_copy (g_http_response_headers, "\r\n", "\n" );

			BEValueList<string> headers ( http_headers, "\n", false );
			auto look_for = ParameterAsUTF8String ( parameters );
			auto found = headers.starts_with ( look_for + ": " );

			SetResult ( found, results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_HTTP_Response_Headers


fmx::errcode BE_Net_Set_Header ( short funcId, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		if ( funcId == kBE_HTTP_Set_Custom_Header ) {
			set_name_value_pair ( parameters, g_http_custom_headers );
		} else {
			set_name_value_pair ( parameters, g_smtp_custom_headers );
		}

		SetResult ( g_last_error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Net_Set_Net_Header



fmx::errcode BE_HTTP_Set_Proxy ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		g_http_proxy.host = ParameterAsUTF8String ( parameters );
		g_http_proxy.port = ParameterAsUTF8String ( parameters, 1 );
		g_http_proxy.username = ParameterAsUTF8String ( parameters, 2 );
		g_http_proxy.password = ParameterAsUTF8String ( parameters, 3 );

		SetResult ( g_last_error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_HTTP_Set_Custom_Header


fmx::errcode BE_Curl_Set_Option ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();

	try {

		auto option = ParameterAsUTF8String ( parameters );

		const unsigned long number_of_parameters = parameters.Size();
		switch ( number_of_parameters ) {

			case 0:
				g_curl_options.clear();
				break;

			case 1:
			{
				BECurlOptionMap::iterator it = g_curl_options.find ( option );
				if ( it != g_curl_options.end() ) {
					g_curl_options.erase ( it );
				} else {
					error = kNotFoundError;
				}
				break;
			}

			case 2:
				std::shared_ptr<BECurlOption> curl_option ( new BECurlOption ( option, parameters.At ( 1 ) ) ); // throws if option not known or not handled
				g_curl_options [ option ] = curl_option;
				break;

//			default:
//				;

		}

	} catch ( BECurlOption_Exception& e ) { // we don't handle it
		error = e.code();
	} catch ( out_of_range& /* e */ ) { // we don't know about it
		error = kNameIsNotValid;
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Curl_Set_Option


fmx::errcode BE_Curl_Trace ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& /* parameters */, fmx::Data& results )
{
	errcode error = kNoError; // do not set the global last error

	try {

		SetResult ( g_curl_trace.str(), results );

	} catch ( BECurlOption_Exception& e ) { // we don't handle it
		error = e.code();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return error;

} // BE_Curl_Trace


fmx::errcode BE_FTP_Upload ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto url = ParameterAsUTF8String ( parameters );
		auto data = ParameterAsVectorChar ( parameters, 1 );
		auto username = ParameterAsUTF8String ( parameters, 2 );
		auto password = ParameterAsUTF8String ( parameters, 3 );

		BECurl curl ( url, kBE_FTP_METHOD_UPLOAD, "", username, password, "", data );
		vector<char> response = curl.ftp_upload ( );

		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( response, results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_FTP_Upload


fmx::errcode BE_FTP_Delete ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto url = ParameterAsUTF8String ( parameters );
		auto username = ParameterAsUTF8String ( parameters, 1 );
		auto password = ParameterAsUTF8String ( parameters, 2 );

		unique_ptr<BECurl> curl ( new BECurl ( url, kBE_FTP_METHOD_DELETE, "", username, password ) );
		vector<char> response = curl->ftp_delete ( );

		error = g_last_error;
		if ( error == kNoError ) {
			SetResult ( response, results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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


fmx::errcode BE_SMTP_Server ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();

	try {

		auto host = ParameterAsUTF8String ( parameters );
		auto port = ParameterAsUTF8String ( parameters, 1 );
		auto username = ParameterAsUTF8String ( parameters, 2 );
		auto password = ParameterAsUTF8String ( parameters, 3 );

		g_smtp_host.host = host;
		g_smtp_host.port = port;
		g_smtp_host.username = username;
		g_smtp_host.password = password;

//		string do_nothing = "";
//		SetResult ( do_nothing, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Email_SMTP_Server


fmx::errcode BE_SMTP_Send ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();

	try {

		auto from = ParameterAsUTF8String ( parameters );
		auto to = ParameterAsUTF8String ( parameters, 1 );
		auto subject = ParameterAsUTF8String ( parameters, 2 );
		auto text = ParameterAsUTF8String ( parameters, 3 );

		unique_ptr<BESMTPEmailMessage> message ( new BESMTPEmailMessage ( from, to, subject, text ) );

		auto cc = ParameterAsUTF8String ( parameters, 4 );
		message->set_cc_addresses ( cc );

		auto bcc = ParameterAsUTF8String ( parameters, 5 );
		message->set_bcc_addresses ( bcc );

		auto reply_to = ParameterAsUTF8String ( parameters, 6 );
		message->set_reply_to ( reply_to );

		auto html = ParameterAsUTF8String ( parameters, 7 );
		message->set_html_alternative ( html );

		
		auto attachments = ParameterAsWideString ( parameters, 8 );
		vector<path> paths;
		if ( !attachments.empty() ) {
		
			boost::split ( paths, attachments, boost::is_any_of ( FILEMAKER_END_OF_LINE ), boost::token_compress_on );

			for ( auto const& attach_this : paths ) {
				g_smtp_attachments.add ( attach_this.string() );
			}

		}
		message->set_attachments ( g_smtp_attachments.get_file_list() );

		
		unique_ptr<BESMTP> smtp ( new BESMTP ( g_smtp_host.host, g_smtp_host.port, g_smtp_host.username, g_smtp_host.password ) );
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


fmx::errcode BE_SMTP_AddAttachment ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();

	try {

		if ( parameters.Size() > 0 ) {

			std::string file_name;
			vector<char> contents;
			
			if ( BinaryDataAvailable ( parameters ) ) {

				contents = ParameterAsVectorChar ( parameters );
				file_name = ParameterFileName ( parameters );
				
			} else {
				file_name = ParameterAsPath ( parameters ).string();
			}
			
			auto content_type = ParameterAsUTF8String ( parameters, 1, BE_DEFAULT_SMTP_CONTENT_TYPE );

			g_smtp_attachments.add ( file_name, contents, content_type );

		} else { // destroy the temporary files and clear out the list
			g_smtp_attachments.clear();
		}

		//		string do_nothing = "";
		//		SetResult ( do_nothing, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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


fmx::errcode BE_FMS_Command ( short function_id, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto username = ParameterAsUTF8String ( parameters );
		std::stringstream quoted_username;
		quoted_username << boost::io::quoted ( username );

		auto password = ParameterAsUTF8String ( parameters, 1 );
		std::stringstream quoted_password;
		quoted_password << boost::io::quoted ( password );

		unique_ptr<BEFMS> fms ( new BEFMS ( quoted_username.str(), quoted_password.str() ) );

		string reply = "";

		switch ( function_id ) {

			case kBE_FMS_Close_Files:
				reply = fms->close_files ( ParameterAsUTF8String ( parameters, 2 ) );
				break;

			case kBE_FMS_List_Clients:
				reply = fms->list_clients ( ParameterAsBoolean ( parameters, 2, false ) );
				break;

			case kBE_FMS_List_Files:
				reply = fms->list_files ( ParameterAsBoolean ( parameters, 2, false ) );
				break;

			case kBE_FMS_List_Schedules:
				reply = fms->list_schedules();
				break;

			case kBE_FMS_Open_Files:
				reply = fms->open_files ( ParameterAsUTF8String ( parameters, 2 ) );
				break;

			case kBE_FMS_Pause_Files:
				reply = fms->pause_files ( ParameterAsUTF8String ( parameters, 2 ) );
				break;

			case kBE_FMS_Remove_Files:
				reply = fms->remove_files ( ParameterAsUTF8String ( parameters, 2 ) );
				break;

			case kBE_FMS_Resume_Files:
				reply = fms->resume_files ( ParameterAsUTF8String ( parameters, 2 ) );
				break;

			case kBE_FMS_Run_Schedule:
				reply = fms->run_schedule ( ParameterAsLong ( parameters, 2 ) );
				break;

			default:
				;

		}

		SetResult ( reply, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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


fmx::errcode BE_OAuth_RequestAccessToken ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto uri = ParameterAsUTF8String ( parameters );
		auto consumer_key = ParameterAsUTF8String ( parameters, 1 );
		auto consumer_secret = ParameterAsUTF8String ( parameters, 2 );
		auto request_key = ParameterAsUTF8String ( parameters, 3 );
		auto request_secret = ParameterAsUTF8String ( parameters, 4 );

		if ( g_oauth ) {
			delete g_oauth;
			g_oauth = NULL;
		}

		string response;

		// if the uri is empty then we are only clearing out any set oauth data

		if ( !uri.empty() ) {

			BEOAuth * oauth = new BEOAuth ( consumer_key, consumer_secret );
			error = oauth->oauth_request ( uri, request_key, request_secret );

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

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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


fmx::errcode BE_Xero_SetTokens ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		if ( g_oauth ) {
			delete g_oauth;
			g_oauth = NULL;
		}

		auto consumer_key = ParameterAsUTF8String ( parameters );
		auto consumer_secret = ParameterAsUTF8String ( parameters, 1 );

		// if the consumer_key is empty then we are only clearing out any set oauth data

		if ( !consumer_key.empty() ) {

			boost::algorithm::replace_all ( consumer_secret, FILEMAKER_END_OF_LINE, "\r\n" );

			BEXero * xero = new BEXero ( consumer_key, consumer_secret );
			g_oauth = xero; // must assign after the authorisation request otherwise BECurl will try and use g_oauth

		}

		SetResult ( "", results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Xero_SetTokens


fmx::errcode BE_Xero_GenerateKeys ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto organisation = ParameterAsUTF8String ( parameters );
		auto organisational_unit = ParameterAsUTF8String ( parameters, 1 );
		auto country = ParameterAsUTF8String ( parameters, 2 );
		auto state = ParameterAsUTF8String ( parameters, 3 );
		auto suburb = ParameterAsUTF8String ( parameters, 4 );
		auto domain = ParameterAsUTF8String ( parameters, 5 );
		auto email = ParameterAsUTF8String ( parameters, 6 );

		unique_ptr<BEX509> x509 ( new BEX509 );

		x509->add_name_entry ( "O", organisation );
		x509->add_name_entry ( "OU", organisational_unit );
		x509->add_name_entry ( "C", country );
		x509->add_name_entry ( "ST", state );
		x509->add_name_entry ( "L", suburb );
		x509->add_name_entry ( "CN", domain );
		x509->add_name_entry ( "emailAddress", email );

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


fmx::errcode BE_Values_Unique ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );
		const bool case_sensitive = ParameterAsBoolean ( parameters, 1 );

		unique_ptr< BEValueList<string> > values ( new BEValueList<string> ( value_list, case_sensitive ) );
		string unique_values = values->unique();

		SetResult ( unique_values, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Values_Unique



fmx::errcode BE_Values_FilterOut ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );
		auto filter_out = ParameterAsUTF8String ( parameters, 1 );
		const bool case_sensitive = ParameterAsBoolean ( parameters, 2 );

		unique_ptr< BEValueList<string> > values ( new BEValueList<string> ( value_list, case_sensitive ) );
		unique_ptr< BEValueList<string> > filter ( new BEValueList<string> ( filter_out, case_sensitive ) );
		string filtered_values = values->filter_out ( *filter );

		SetResult ( filtered_values, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Values_FilterOut


fmx::errcode BE_Values_ContainsDuplicates ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );
		auto case_sensitive = ParameterAsBoolean ( parameters, 1 );

		auto values ( new BEValueList<string> ( value_list, case_sensitive ) );
		auto contains_duplicates = values->contains_duplicates();

		SetResult ( contains_duplicates, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Values_ContainsDuplicates


fmx::errcode BE_Values_Sort ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );
		const bool ascending = ParameterAsBoolean ( parameters, 1, true );
		const long type = ParameterAsLong ( parameters, 2, kBE_DataType_String );

		unique_ptr< BEValueList<string> > values ( new BEValueList<string> ( value_list ) );
		string sorted_values = values->sort ( ascending, type );

		SetResult ( sorted_values, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Values_Sort


fmx::errcode BE_Values_TimesDuplicated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );
		const long numberOfTimes = ParameterAsLong ( parameters, 1 );

		unique_ptr< BEValueList<string> > values ( new BEValueList<string> ( value_list ) );
		BEValueList<std::string> times_duplicated = values->times_duplicated ( numberOfTimes );
		std::string found = times_duplicated.get_as_filemaker_string();
		SetResult ( found, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Values_TimesDuplicated


fmx::errcode BE_Values_Trim ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );

		unique_ptr< BEValueList<string> > values ( new BEValueList<string> ( value_list ) );
		values->trim_values();
//		std::string found = values.get_as_filemaker_string();
		SetResult ( values->get_as_filemaker_string(), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Values_Trim



#pragma mark -
#pragma mark Bitwise
#pragma mark -


fmx::errcode BE_XOR ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto text = ParameterAsUTF8String ( parameters );
		const uint8_t xorWith = (uint8_t)ParameterAsLong ( parameters, 1 );

		std::stringstream xord_text;

		for ( std::string::iterator it = text.begin() ; it != text.end() ; ++it ) {
			const uint8_t xord = *it ^ xorWith;
			xord_text << std::setfill ( '0' ) << std::setw ( 2 ) << std::hex << (const unsigned int)xord;
		}

		SetResult ( xord_text.str(), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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


fmx::errcode BE_Vector_DotProduct ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
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

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Vector_DotProduct


fmx::errcode BE_Vector_EuclideanDistance ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
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

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Vector_EuclideanDistance



#pragma mark -
#pragma mark PDF
#pragma mark -


fmx::errcode BE_PDF_Append ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		auto pdf_document = ParameterAsPDF ( parameters );
		auto pdf_document_to_append = ParameterAsPDF ( parameters, 1 );
		pdf_document->Append ( *pdf_document_to_append );
		pdf_document_to_append.reset(); // make sure to close the file
		
		// write out a temporary file
		auto temporary_file = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
		pdf_document->Write ( temporary_file.c_str() );
		pdf_document.reset(); // make sure to close the file
		
		auto output_path = ParameterAsPath ( parameters, 2 );
		if ( output_path.empty() ) {
			
			auto file_data = ReadFileAsBinary ( temporary_file );
			auto destination = ParameterFileName ( parameters );
			SetResult ( destination, file_data, results, FILE_CONTAINER_TYPE );
			
		} else {
			
			rename ( temporary_file, output_path );
			
//			SetResult ( nothing, results );
			
		}
		
	} catch ( filesystem_error& e ) {
		error = e.code().value();
	} catch ( const PoDoFo::PdfError& e ) {
		error = e.GetError();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_PDF_Append


fmx::errcode BE_PDF_PageCount ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		auto pdf_document = ParameterAsPDF ( parameters );
		auto page_count = pdf_document->GetPageCount();
		SetResult ( page_count, results );
		
	} catch ( filesystem_error& e ) {
		error = e.code().value();
	} catch ( const PoDoFo::PdfError& e ) {
		error = e.GetError();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_PDF_PageCount


fmx::errcode BE_PDF_GetPages ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	
	try {
		
		auto pdf_document = ParameterAsPDF ( parameters );
		auto from = ParameterAsIndex ( parameters, 2 );
		auto to = ParameterAsIndex ( parameters, 3 );
		to = to < from ? from : to;
		
		std::unique_ptr<PoDoFo::PdfMemDocument> new_pdf ( new PoDoFo::PdfMemDocument() );
		
		for ( auto i = from ; i <= to ; i++ ) {
			new_pdf->InsertExistingPageAt ( *pdf_document, (int)i, new_pdf->GetPageCount() );
		}
		
		pdf_document.reset(); // make sure to close the file
		
		// write out a temporary file
		auto temporary_file = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
		new_pdf->Write ( temporary_file.c_str() );
		new_pdf.reset(); // make sure to close the file
		
		auto output_path = ParameterAsPath ( parameters, 1 );
		if ( output_path.empty() ) {
			
			auto file_data = ReadFileAsBinary ( temporary_file );
			auto destination = ParameterFileName ( parameters );
			SetResult ( destination, file_data, results, FILE_CONTAINER_TYPE );
			
		} else {
			
			rename ( temporary_file, output_path );
			
			//			SetResult ( nothing, results );
			
		}
		
	} catch ( filesystem_error& e ) {
		error = e.code().value();
	} catch ( const PoDoFo::PdfError& e ) {
		error = e.GetError();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // BE_PDF_GetPages



#pragma mark -
#pragma mark Script Steps
#pragma mark -


fmx::errcode BE_InstallScriptStep ( short /* function_id */, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& /* reply */ )
{
	
	errcode error = NoError();
	
	try {
		
		fmx::TextUniquePtr name;
		name->SetText ( parameters.AtAsText ( 0 ) );
		
		fmx::TextUniquePtr definition;
		definition->SetText ( parameters.AtAsText ( 1 ) );
		
		auto id = ParameterAsLong ( parameters, 2 );
		
		fmx::TextUniquePtr description;
		description->SetText ( parameters.AtAsText ( 3 ) );
			
		const fmx::uint32 flags = fmx::ExprEnv::kAllDeviceCompatible;
		
		error = environment.RegisterScriptStep ( *g_be_plugin->id(), id, *name, *definition, *description, flags, BE_PerformScriptStep );
			
		if ( kNoError == error ) {
			auto calculation = ParameterAsUTF8String ( parameters, 4 );
			g_script_steps[(short)id] = calculation;
		}
		
//		SetResult ( error, reply );
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );

} // BE_InstallScriptStep


fmx::errcode BE_RemoveScriptStep ( short /* function_id */, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& /* reply */ )
{
	errcode error = NoError();
		
	try {
			
		auto id = ParameterAsLong ( parameters );

		error = environment.UnRegisterScriptStep ( *g_be_plugin->id(), (short)id );
		if ( kNoError == error ) {
			g_script_steps.erase ( (short)id );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
		
	return MapError ( error );
		
} // BE_RemoveScriptStep


fmx::errcode BE_PerformScriptStep ( short function_id, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply )
{
	errcode error = NoError();
	
	try {
		
		auto calculation = g_script_steps [ function_id ];
		
		auto number_of_parameters = parameters.Size();
		for ( fmx::uint32 i = 0 ; i < number_of_parameters ; i++ ) {
			
			auto data_type = parameters.At ( i ).GetNativeType();
			
			std::string parameter_value = "";
			
			switch ( data_type ) {
					
				case fmx::Data::kDTDate:
				case fmx::Data::kDTNumber:
				case fmx::Data::kDTTime:
				case fmx::Data::kDTTimeStamp:
				case fmx::Data::kDTBoolean:
					
					parameter_value = std::to_string ( ParameterAsLong ( parameters, i ) );
					break;
					
				case fmx::Data::kDTBinary:
				case fmx::Data::kDTText:
				case fmx::Data::kDTInvalid:
				default:
					parameter_value = ParameterAsUTF8String ( parameters, i );
					
			}
			
			const std::string token = "###";
			auto replace_this = token + std::to_string ( i ) + token;
			
			std::string::size_type position = 0;
			while ( ( position = calculation.find ( replace_this, position ) ) != std::string::npos )
			{
				calculation.replace ( position, replace_this.size(), parameter_value );
				position += parameter_value.size();
			}
			
		}
		
		fmx::TextUniquePtr command;
		command->Assign ( calculation.c_str(), fmx::Text::kEncoding_UTF8 );
		
		error = environment.Evaluate ( *command, reply );
		
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}
	
	return MapError ( error );
	
} // PerformScriptStep



#pragma mark -
#pragma mark Other / Ungrouped
#pragma mark -


/*

 invoked for multiple plug-in functions... funcId is used to determine which one

 constants should be defined in BEPluginGlobalDefines.h

 each set of constants should have it's own range [ 1000 then 2000 then 3000 etc. ]
 with an offset of x000

*/

fmx::errcode BE_NotImplemented ( short /* function_id */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& /* results */ )
{

	return MapError ( kCommandIsUnavailableError );
	
} // BE_NotImplemented


fmx::errcode BE_NumericConstants ( short funcId, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = NoError();

	try {

		SetResult ( funcId % kBE_NumericConstantOffset, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_NumericConstants


fmx::errcode BE_TimeFunctions ( const short funcId, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
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

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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

fmx::errcode BE_ExtractScriptVariables ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto variables ( new BEValueList<std::wstring> );
		auto calculation = ParameterAsWideString ( parameters );
		auto variable_prefix = ParameterAsWideString ( parameters, 1, L"$" ); // look for FM script variables unless otherwise specified

		wstring search_for = variable_prefix + L"/\""; // variables, comments and strings (including escaped strings)
		size_t found = calculation.find_first_of ( search_for );

		while ( found != wstring::npos )
		{
			size_t end = 0;
			size_t search_from = found + 1;
			
			switch ( calculation.at ( found ) ) {

				case L'$': // script variables
				case L'@': // base elements variables
				{
					/*
					 find the end of the variable
						+ - * / ^ & = ≠ < > ≤ ≥ ( , ; ) [ ] " :: $ }
					 unicode escapes are required on Windows
					 */

					end = calculation.find_first_of ( L" ;+-=*/&^<>\t\r[]()\u2260\u2264\u2265,", search_from );
					if ( end == wstring::npos ) {
						end = calculation.size();
					}

					// add the variable to the list
					wstring wanted = calculation.substr ( found, end - found );
					variables->append ( wanted );
					search_from = end + 1;
				}
				break;

				case L'/': // comments
					switch ( calculation.at ( search_from ) ) {
						case L'/':
							end = calculation.find ( L"\r", search_from );
							search_from = end + 1;
							break;

						case L'*':
							end = calculation.find ( L"*/", search_from );
							search_from = end + 2;
							break;

						default:
							break;
					}
					break;

				case L'\"': // escaped strings
					end = calculation.find ( L"\"", search_from );
					while ( (end != string::npos) && (calculation.at ( end - 1 ) == L'\\') ) {
						end = calculation.find ( L"\"", end + 1 );
					}
					search_from = end + 1;
					break;

//				default:
			}

			// this is not on an eternal quest
			if ( (end != string::npos) && (search_from < calculation.size()) ) {
				found = calculation.find_first_of ( search_for, search_from );
			} else {
				found = string::npos;
			}
		}

		SetResult ( variables->unique(), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ExtractScriptVariables



fmx::errcode BE_ExecuteSystemCommand ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto command = ParameterAsUTF8String ( parameters );
		const long timeout = ParameterAsLong ( parameters, 1, kBE_Never );

		std::string response;
		error = ExecuteSystemCommand ( command, response, timeout );

		SetResult ( response, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
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

fmx::errcode BE_OpenURL ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto url = ParameterAsWideString ( parameters );
		bool succeeded = OpenURL ( url );

		SetResult ( succeeded, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_OpenURL



fmx::errcode BE_OpenFile ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		// in this instance the string is a better choice than boost::filesystem::path
		auto path = ParameterAsWideString ( parameters );

		bool succeeded = OpenFile ( path );
		SetResult ( succeeded, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_OpenFile



fmx::errcode BE_ExecuteScript ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		TextUniquePtr script_name;
		script_name->SetText ( parameters.AtAsText ( 0 ) );

		FMX_UInt32 number_of_paramters = parameters.Size();

		TextUniquePtr file_name;
		if ( number_of_paramters >= 2 ) {
			file_name->SetText ( parameters.AtAsText ( 1 ) );
		}

		// get the parameter, if present
		DataUniquePtr parameter;
		if ( number_of_paramters == 3 ) {

			LocaleUniquePtr default_locale;
			parameter->SetAsText ( parameters.AtAsText ( 2 ), *default_locale );

		}

		error = ExecuteScript ( *script_name, *file_name, *parameter, environment );

		SetResult ( error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ExecuteScript



fmx::errcode BE_FileMakerSQL ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		TextUniquePtr expression;
		expression->SetText ( parameters.AtAsText(0) );

		FMX_UInt32 number_of_paramters = parameters.Size();

		TextUniquePtr filename;
		if ( number_of_paramters == 4 ) {
			filename->SetText ( parameters.AtAsText(3) );
		}

		BESQLCommandUniquePtr sql ( new BESQLCommand ( *expression, *filename ) );

		if ( number_of_paramters >= 2 ) {
			TextUniquePtr column_separator;
			column_separator->SetText ( parameters.AtAsText(1) );
			sql->set_column_separator ( *column_separator );
		}

		if ( number_of_paramters >= 3 ) {
			TextUniquePtr row_separator;
			row_separator->SetText ( parameters.AtAsText(2) );
			sql->set_row_separator ( *row_separator );
		}

		sql->execute ( environment );

		SetResult ( *(sql->get_text_result()), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_FileMakerSQL



fmx::errcode BE_MessageDigest_Deprecated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto message = ParameterAsUTF8String ( parameters );
		const unsigned long algorithm = ParameterAsLong( parameters, 1, kBE_MessageDigestAlgorithm_SHA256 );
		const unsigned long output_type = ParameterAsLong( parameters, 2, kBE_Encoding_Hex );

		string digest = message_digest ( message, algorithm, output_type );

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



fmx::errcode BE_HMAC_Deprecated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto message = ParameterAsUTF8String ( parameters );
		auto key = ParameterAsUTF8String ( parameters, 1 );
		const unsigned long algorithm = ParameterAsLong ( parameters, 2, kBE_MessageDigestAlgorithm_SHA1 );
		const unsigned long output_type = ParameterAsLong ( parameters, 3, kBE_Encoding_Hex );
		const unsigned long input_type = ParameterAsLong ( parameters, 4, kBE_Encoding_None );

		string hmac = HMAC ( message, algorithm, output_type, key, input_type );

		SetResult ( hmac, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_HMAC_Deprecated


fmx::errcode BE_JPEG_Recompress ( const short function_id, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		vector<unsigned char> original_jpeg = ParameterAsVectorUnsignedChar ( parameters );

		if ( original_jpeg.size() > 0 ) {

			unique_ptr<BEJPEG> jpeg ( new BEJPEG ( original_jpeg ) );
			const int quality = (const int)ParameterAsLong ( parameters, 1, 75 ); // percent
			jpeg->set_compression_level ( quality );

			if ( function_id == kBE_JPEG_Recompress ) {

				const double scale = ParameterAsDouble ( parameters, 2 );
				jpeg->set_scaling ( scale );

			} else {

				// deprecated

				const int width = (const int)ParameterAsLong ( parameters, 2, 0 );
				jpeg->set_width ( width );

				const int height = (const int)ParameterAsLong ( parameters, 3, 0 );
				jpeg->set_height ( height );

			}

			jpeg->recompress();

			auto image_name = ParameterFileName ( parameters );
			SetResult ( image_name, *jpeg, results );

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


fmx::errcode BE_ConvertContainer ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		const BinaryDataUniquePtr data ( parameters.AtAsBinaryData ( 0 ) );
		if ( data->GetCount() > 0 ) {

			vector<char> container_data = ParameterAsVectorChar ( parameters );
			auto filename = ParameterFileName ( parameters );

			auto type = ParameterAsUTF8String ( parameters, 1, FILE_CONTAINER_TYPE );

			const unsigned long width = ParameterAsLong ( parameters, 2, kErrorUnknown );
			const unsigned long height = ParameterAsLong ( parameters, 3, kErrorUnknown );

			SetResult ( filename, container_data, type, width, height, results );

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


fmx::errcode BE_RegularExpression ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

        auto text = ParameterAsUTF8String ( parameters );
        auto expression = ParameterAsUTF8String ( parameters, 1 );
		auto options = ParameterAsUTF8String ( parameters, 2 );
        auto replace_with = ParameterAsUTF8String ( parameters, 3 );
        const bool replace = parameters.Size() == 4 ;

        BEValueList<std::string> matched = regular_expression ( text, expression, options, replace_with, replace );
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


fmx::errcode BE_Pause ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& /* results */ )
{
	errcode error = NoError();

	try {

		const long milliseconds = ParameterAsLong ( parameters );

		std::this_thread::sleep_for ( std::chrono::milliseconds ( milliseconds ) );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Pause


fmx::errcode BE_Get_Machine_Name ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = NoError();

	try {

		auto machine_name = get_machine_name();
		SetResult ( machine_name, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Get_Machine_Name


