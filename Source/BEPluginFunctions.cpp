/*
 BEPluginFunctions.cpp
 BaseElements Plug-In

 Copyright 2010-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


// need (Parser) to go before BEPluginGlobalDefines.h or we not happy

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#include <Poco/Dynamic/Var.h>
#include <Poco/Exception.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Query.h>
#include <Poco/Path.h>
#include <Poco/RegularExpression.h>
#include <Poco/String.h>
#pragma GCC diagnostic pop


#include "BEPluginGlobalDefines.h"


#if defined FMX_MAC_TARGET

	#include "apple/BEMacFunctions.h"

#elif FMX_IOS_TARGET

	#include "apple/BEIOSFunctions.h"

#elif defined FMX_WIN_TARGET

	#include <locale.h>
	#include "win/BEWinFunctions.h"

#elif defined FMX_LINUX_TARGET

	#include "linux/BELinuxFunctions.h"

#endif


#include <../Headers/iconv/iconv.h>

#include "BEPluginFunctions.h"

#include "BECurl.h"
#include "BECurlOption.h"
#include "BECppUtilities.h"
#include "BEDebugInformation.h"
#include "BEFileMakerPlugin.h"
#include "BEFileSystem.h"
#include "BEJavaScript.h"
#include "BEJSON.h"
#include "BEPDF.h"
#include "BEPluginException.h"
#include "BEQuadChar.h"
#include "BERegularExpression.h"
#include "BESMTP.h"
#include "BESMTPContainerAttachments.h"
#include "BESQLCommand.h"
#include "BESystemCommand.h"
#include "BETime.h"
#include "BEValueList.h"
#include "BEXMLReader.h"
#include "BEXMLSchema.h"
#include "BEXMLTextReader.h"
#include "BEXSLT.h"
#include "BEZlib.h"
#include "Crypto/BEBase64.h"
#include "Crypto/BEMessageDigest.h"
#include "Crypto/BEOpenSSLAES.h"
#include "Crypto/BEOpenSSLRSA.h"
#include "Crypto/BEX509.h"
#include "Images/BEJPEG.h"

#include <algorithm>
#include <list>
#include <numeric> // for inner_product
#include <thread>

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include <podofo/podofo.h>

using namespace std;
using namespace fmx;
using namespace boost::filesystem;


#pragma mark -
#pragma mark Globals
#pragma mark -

#define GZIP_FILE_EXTENSION string ( ".gz" )


thread_local std::map<short, std::string> g_script_steps;
thread_local errcode g_last_error;
thread_local errcode g_last_ddl_error;
thread_local string g_text_encoding = UTF8;
thread_local string g_json_error_description;
thread_local struct host_details g_smtp_host;
thread_local BESMTPContainerAttachments g_smtp_attachments;
thread_local vector<BEValueListStringSharedPtr> arrays;

extern BEFileMakerPlugin * g_be_plugin;

extern thread_local int g_http_response_code;
extern thread_local string g_http_response_headers;
extern thread_local CustomHeaders g_http_custom_headers;
extern thread_local CustomHeaders g_smtp_custom_headers;
extern thread_local std::stringstream g_curl_trace;
extern thread_local struct host_details g_http_proxy;
extern thread_local BECurlOptionMap g_curl_options;


#pragma mark -
#pragma mark Version
#pragma mark -


fmx::errcode BE_Version ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	return TextConstantFunction ( WSTRING ( VERSION_STRING ), results );
}


fmx::errcode BE_VersionAutoUpdate ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	return TextConstantFunction ( WSTRING ( AUTO_UPDATE_VERSION ), results );
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


fmx::errcode BE_ClipboardGetText ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto atype = ParameterAsWideString ( parameters );
		auto clipboard_contents = ClipboardText ( atype );

		SetResult ( clipboard_contents, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ClipboardGetText


fmx::errcode BE_ClipboardSetText ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto to_copy = ParameterAsUTF8String ( parameters );
		auto atype = ParameterAsWideString ( parameters, 1 );
		auto success = SetClipboardText ( to_copy, atype );

		SetResult ( success, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ClipboardSetText


fmx::errcode BE_ClipboardGetFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto atype = ParameterAsWideString ( parameters );
		auto file_name = ParameterAsUTF8String ( parameters, 1, ParameterAsUTF8String ( parameters ) /* atype */ );

		auto clipboard_contents = ClipboardFile ( atype );

		SetResult ( file_name, clipboard_contents, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ClipboardGetFile


fmx::errcode BE_ClipboardSetFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto to_copy = ParameterAsVectorUnsignedChar ( parameters );
		auto atype = ParameterAsWideString ( parameters, 1 );
		auto success = false;
		if ( ! atype.empty() ) {
			success = SetClipboardFile ( to_copy, atype );
		}

		SetResult ( success, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ClipboardSetFile


#pragma mark -
#pragma mark Files & Folders
#pragma mark -


#pragma NOTE (consider refactoring some of the detail from the file functions into BEFileSystem)


fmx::errcode BE_FolderCreate ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_FolderCreate


fmx::errcode BE_FileDelete ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_FileDelete


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

		SetResult ( (double)size, results );

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


fmx::errcode BE_FileModificationTimestamp ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
    errcode error = NoError();

    try {

        const path path = ParameterAsPath ( parameters );

        const std::time_t last_modified = boost::filesystem::last_write_time ( path );
		const fmx::int64 timestamp = std_time_to_timestamp ( last_modified );

		SetResult ( (double)timestamp, results );

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

} // BE_FileModificationTimestamp


fmx::errcode BE_FileReadText ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {
		long start = ParameterAsLong ( parameters, 1, 0) ;
		if ( start < 0 ) {
			start = 0 ;
		}
		long to = ParameterAsLong ( parameters, 2, 0 ) ;
		if ( to < 0 ) {
			to = 0 ;
		}
		std::string delimiter = ParameterAsUTF8String ( parameters, 3, "" ) ;
		if ( delimiter.length() > 1 ) {
			delimiter = delimiter.substr(0,1) ;
		}
		const std::string file_contents = ParameterPathOrContainerAsUTF8 ( parameters, 0, start, to, delimiter ) ;
        SetResult ( file_contents, results ) ;
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

} // BE_FileReadText


fmx::errcode BE_FileWriteText ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_FileWriteText


fmx::errcode BE_WriteTextFileToContainer_Deprecated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_WriteTextFileToContainer_Deprecated


/*
 filemaker can create DDRs which contain utf-16 characters that are not
 valid in an XML document. reads the DDR and writes out a new one, skipping
 any invalid characters, and replaces the old file
 */

fmx::errcode BE_XMLStripInvalidCharacters ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

		SetResult ( (double)skipped, results );

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

} // BE_XMLStripInvalidCharacters



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


fmx::errcode BE_FileImport ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

		SetResult ( from.filename().string(), file_data, data_type, results );

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

} // BE_FileImport


fmx::errcode BE_FilePatternCount ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto file_list = ParameterAsStringValueList ( parameters );

		auto search_strings = ParameterAsStringValueList ( parameters, 1 );
		search_strings->to_lower(); // case insensitve

		BEValueList<string> needle_counts;

		// we need a 0 count for an empty search
		if ( search_strings->not_empty() ) {

			// we're searching for needles in a haystack
			std::vector<string> haystack;

			try {

				// gather the hay

				for ( typename BEValueList<string>::iterator it = file_list->begin() ; it != file_list->end(); ++it ) {

					boost::filesystem::path text_file_path ( *it );
					text_file_path.make_preferred();
					auto hay = ReadFileAsUTF8 ( text_file_path );
					std::transform ( hay.begin(), hay.end(), hay.begin(), ::tolower ); // case insensitive
					haystack.push_back ( hay );

				}

			} catch ( boost::filesystem::ifstream::failure& /* e */ ) {
				error = errno; // cannot read the file
			} catch ( boost::filesystem::filesystem_error& e ) {
				error = e.code().value();
			}

			// search for needles

			for ( typename BEValueList<string>::iterator it = search_strings->begin() ; it != search_strings->end(); ++it ) {

				auto needle = *it;
				auto found_count = 0;

				for ( typename std::vector<string>::iterator jt = haystack.begin() ; jt != haystack.end(); ++jt ) {

					auto hay = *jt;

					auto found_at = hay.find ( needle, 0 );
					while ( found_at != std::string::npos ) {
						found_at = hay.find ( needle, found_at + needle.length() );
						++found_count;
					}

				} // for... haystack...

				needle_counts.append ( to_string ( found_count ) );

			} // for... search_strings...

		} else {
			needle_counts.append ( "0" );
		}

		SetResult ( needle_counts, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_FilePatternCount


fmx::errcode BE_FileReplaceText ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto text = ParameterPathOrContainerAsUTF8 ( parameters );
		auto expression = ParameterAsUTF8String ( parameters, 1 );
		auto replace_with = ParameterAsUTF8String ( parameters, 2 );
		auto options = ParameterAsUTF8String ( parameters, 3, "gi" );

		auto altered_text = regular_expression ( text, expression, options, replace_with, true );

		const std::vector<char> out_to_file ( altered_text.begin(), altered_text.end() );

		auto is_container = BinaryDataAvailable ( parameters );
		if ( is_container ) {

			const BinaryDataUniquePtr binary_data ( parameters.AtAsBinaryData(0) );
			unique_ptr<BEQuadChar> data_type ( new BEQuadChar ( *binary_data, PreferredContainerType ( *binary_data ) ) );
			auto file_name = ParameterFileName ( parameters );
			SetResult ( file_name, out_to_file, data_type->as_string(), results );

		} else {

			auto path = ParameterAsPath ( parameters );
			error = write_to_file ( path, out_to_file, ios_base::trunc );
//			SetResult ( error, results );

		}


	} catch ( filesystem_error& e ) {
		error = e.code().value();
	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_FileReplaceText


fmx::errcode BE_FileMove ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_FileMove


fmx::errcode BE_FileCopy ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_FileCopy



fmx::errcode BE_FileListFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		const path directory = ParameterAsPath ( parameters );
		const long file_type_wanted = ParameterAsLong ( parameters, 1, kBE_FileTypeFile );
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

} // BE_FileListFolder



#pragma mark -
#pragma mark Dialogs
#pragma mark -

fmx::errcode BE_FileSelect ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	if ( g_be_plugin->running_on_server() ) {
		return MapError ( kCommandIsUnavailableError );
	}

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

} // BE_FileSelect


fmx::errcode BE_FolderSelect ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	if ( g_be_plugin->running_on_server() ) {
		return MapError ( kCommandIsUnavailableError );
	}

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

} // BE_FolderSelect


fmx::errcode BE_FileSaveDialog ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	if ( g_be_plugin->running_on_server() ) {
		return MapError ( kCommandIsUnavailableError );
	}

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

} // BE_FileSaveDialog


fmx::errcode BE_DialogDisplay ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	if ( g_be_plugin->running_on_server() ) {
		return MapError ( kCommandIsUnavailableError );
	}

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

} // BE_DialogDisplay



fmx::errcode BE_DialogProgress ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& /* results */ )
{
	if ( g_be_plugin->running_on_server() ) {
		return MapError ( kCommandIsUnavailableError );
	}

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

} // BE_DialogProgress


fmx::errcode BE_DialogProgressUpdate ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& /* results */ )
{
	if ( g_be_plugin->running_on_server() ) {
		return MapError ( kCommandIsUnavailableError );
	}

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

} // BE_DialogProgressUpdate



#pragma mark -
#pragma mark XML / XSLT
#pragma mark -


fmx::errcode BE_XSLTApply ( short function_id, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		std::string xml;
		boost::filesystem::path xml_path;
		boost::filesystem::path csv_path;

		if ( function_id == kBE_XSLTApply ) {
			xml = ParameterPathOrContainerAsUTF8 ( parameters );
			xml_path = ParameterAsPath ( parameters );
			csv_path = ParameterAsPath ( parameters, 2 );
		} else {
			xml = ParameterAsUTF8String ( parameters );
		}

		auto xslt = ParameterAsUTF8String ( parameters, 1 );

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

} // BE_XSLTApply


fmx::errcode BE_XPath ( short function_id, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

		xmlXPathObjectType xpath_object_type = XPATH_NODESET; // for kBE_XPathAll
		if ( function_id == kBE_XPath ) {

			bool as_text = ParameterAsBoolean ( parameters, 3, true );
			if ( as_text != true ) {
				xpath_object_type = XPATH_UNDEFINED; // get the result as xml
			} else {
				xpath_object_type = XPATH_STRING;
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


fmx::errcode BE_XMLStripNodes ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_XMLStripNodes



fmx::errcode BE_XMLParse ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();

	try {

		std::string result;
		std::unique_ptr<BEXMLTextReader> reader;

		auto xml_input = ParameterAsUTF8String ( parameters );

		// if the input, excluding whitespace, begins with < we treat it as xml... if not, treat it as a file path
		auto it = find_if_not ( xml_input.begin(), xml_input.end(), [](int c){ return isspace(c); } );
		if ( "<" == xml_input.substr ( 0, 1 ) || *it == '<' ) {
			reader.reset ( new BEXMLTextReader ( xml_input ) );
		} else {
			auto input_file = ParameterAsPath ( parameters );
			reader.reset ( new BEXMLTextReader ( input_file ) );
		}

		result = reader->parse();

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

} // BE_XMLParse



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


fmx::errcode BE_XMLValidate ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_XMLValidate



fmx::errcode BE_XMLCanonical ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();

	try {

		auto xml = ParameterAsUTF8String ( parameters );

		auto canonized_xml = canonical_xml ( xml );

		SetResult ( canonized_xml, results );

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

} // BE_XMLCanonical


fmx::errcode BE_XMLTidy ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error =	NoError();

	try {

		auto xml = ParameterAsUTF8String ( parameters );

		auto pretty_printed = pretty_print_xml ( xml );

		SetResult ( pretty_printed, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_XMLTidy



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


fmx::errcode BE_JSON_ArraySize ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();
	g_json_error_description.clear();

	try {

		std::size_t array_size = 0; // if there's no array or the path cannot be found...
		auto json = ParameterAsUTF8String ( parameters );

		try {

			Poco::JSON::Parser json_parser;
			const auto json_object = json_parser.parse ( json );

			auto path = ParameterAsUTF8String ( parameters, 1 );
			const auto json_query = Poco::JSON::Query ( json_object );
			const auto json_array = json_query.findArray ( path ); // empty path gives the whole thing
			array_size = json_array->size();

		} catch ( Poco::BadCastException& /* e */ ) {
			; // not an array... don't error... array_size = 0;
		} catch ( Poco::InvalidAccessException& /* e */ ) {
			; // didn't find path... don't error... array_size = 0;
		} catch ( Poco::Exception& e ) {
			error = e.code();
//			g_json_error_description = e.what(); // deprecated
		}

		SetResult ( (double)array_size, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_JSON_ArraySize


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
#pragma mark Arrays & Variables
#pragma mark -


fmx::errcode BE_ArraySetFromValueList ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto  retain_empty_values = ParameterAsBoolean ( parameters, 1, false ); // false -> backwards compatibility
		BEValueListStringSharedPtr array = ParameterAsStringValueList ( parameters, 0, false, retain_empty_values );

		arrays.push_back ( array );
		const size_t size = arrays.size();

		SetResult ( (double)size, results );

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
			SetResult ( (double)array_size, results );

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


fmx::errcode BE_ArrayDelete ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
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

} // BE_ArrayDelete


fmx::errcode BE_ArrayFind ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto array_id = ParameterAsIndex ( parameters );

		try {

			auto wanted = arrays.at ( array_id ); // so we throw if the index is invalid
			auto find_this = ParameterAsUTF8String ( parameters, 1 );
			auto found = wanted->find ( find_this );

			SetResult ( found, results );

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

} // BE_ArrayFind


fmx::errcode BE_ArrayChangeValue ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
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

} // BE_ArrayChangeValue


fmx::errcode BE_Variable ( short function_id, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto name = ParameterAsUTF8String ( parameters );

		if ( !name.empty() ) {

			static thread_local map<string,string> variables;

			switch ( function_id ) {

				case kBE_VariableSet:
				{
					auto set_this = ParameterAsUTF8String ( parameters, 1 );
					if ( !set_this.empty() ) {
						variables [ name ] = set_this;
						SetResult ( set_this, results );
					} else {
						variables.erase ( name );
					}
					break;
				}

				case kBE_VariableGet:
				{
					auto value = variables [ name ];
					SetResult ( value, results );
					break;
				}

				default:
					; // should never get here

			} // switch

		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Variable


fmx::errcode BE_Stack ( short function_id, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		auto name = ParameterAsUTF8String ( parameters );
		if ( !name.empty() ) {

			static thread_local map<string,stack<string>> stacks;

			stack<string> the_stack;
			auto it = stacks.find ( name );
			if (  it != stacks.end() ) {
				the_stack = stacks [ name ];
			}

			switch ( function_id ) {

				case kBE_StackPush:
				{
					auto push_this = ParameterAsUTF8String ( parameters, 1 );
					the_stack.push ( push_this );
					stacks [ name ] = the_stack;
					SetResult ( push_this, results );
					break;
				}

				case kBE_StackPop:
				{
					if ( !the_stack.empty() ) {
						auto value = the_stack.top();
						the_stack.pop();
						stacks [ name ] = the_stack;
						SetResult ( value, results );
					}
					break;
				}

				case kBE_StackCount:
				{
					auto size = the_stack.size();
					SetResult ( (double)size, results );
					break;
				}

				case kBE_StackDelete:
					stacks.erase ( name );
					break;

				default:
					; // should never get here

			} // switch

		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Stack



#pragma mark -
#pragma mark User Preferences
#pragma mark -


fmx::errcode BE_PreferenceSet ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto key = ParameterAsUTF8String ( parameters );
		auto value = ParameterAsUTF8String ( parameters, 1 );
		auto domain = ParameterAsUTF8String ( parameters, 2, USER_PREFERENCES_DOMAIN );

		SetResult ( SetPreference ( key, value, domain ), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_PreferenceSet


// BE_PreferenceGet
// BE_PreferenceDelete

fmx::errcode BE_Preference ( short function_id, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto key = ParameterAsUTF8String ( parameters );
		auto domain = ParameterAsUTF8String ( parameters, 1, USER_PREFERENCES_DOMAIN );

		if ( kBE_PreferenceGet == function_id ) {
			SetResult ( GetPreference ( key, domain ), results );
		} else {
			DeletePreference ( key, domain );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_Preference



#pragma mark -
#pragma mark Compression / Encoding
#pragma mark -



fmx::errcode BE_Unzip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto output_directory = ParameterAsUTF8String ( parameters, 1 );

		auto is_container = BinaryDataAvailable ( parameters );
		if ( is_container ) {

			auto archive = ParameterAsVectorChar ( parameters );
			error = (fmx::errcode)UnZipMemory ( archive, output_directory );

		} else {

			auto archive = ParameterAsUTF8String ( parameters );
			error = (fmx::errcode)UnZipFile ( archive, output_directory );

		}

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

		long zip_error = kNoError;
		auto output_directory = ParameterAsUTF8String ( parameters, 1 );

		auto is_container = BinaryDataAvailable ( parameters );
		if ( is_container ) {

			auto zip_this = ParameterAsVectorChar ( parameters );
			auto file_name = ParameterFileName( parameters );
			zip_error = ZipMemory ( zip_this, file_name, output_directory );

		} else {

			// should there be multiple files with the same name... keep only the last one
			auto values = ParameterAsStringValueList ( parameters, 0, true, false );

			std::map<std::string, std::string> unique_file_names;

			for ( typename BEValueList<string>::iterator it = values->begin() ; it != values->end(); ++it ) {

				boost::filesystem::path file_path ( *it );
				auto file_name = boost::algorithm::to_lower_copy ( file_path.filename().string() );
				auto inserted = unique_file_names.insert ( { file_name, file_path.string() } );

				if ( ! inserted.second ) {
					error = kNameAlreadyExists;
					auto next = unique_file_names.erase ( inserted.first );
					if ( next == unique_file_names.end() ) {
						unique_file_names.insert ( { file_name, file_path.string() } ); // auto replaced =
					}
				}

			} // for...

			BEValueListStringUniquePtr files ( new BEValueList<string> ( map_values ( unique_file_names ) ) );
			zip_error = ZipFiles ( files.get(), output_directory );

		}

		if ( kNoError != zip_error ) {
			error = zip_error;
		}

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

} // BE_Zip



fmx::errcode BE_SetTextEncoding ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto encoding = ParameterAsUTF8String ( parameters );

		iconv_t conversion = iconv_open ( encoding.c_str(), encoding.c_str() );
		if ( conversion != (iconv_t)-1 ) {
			g_text_encoding = encoding;
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

		SetResult ( filename, to_compress, COMPRESSED_CONTAINER_TYPE, results );

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


fmx::errcode BE_ContainerListTypes ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		const BinaryDataUniquePtr container ( parameters.AtAsBinaryData ( 0 ) );
		fmx::int32 count = container->GetCount();
		BEValueListStringUniquePtr types ( new BEValueList<string> ( "" ) );

		for ( fmx::int32 i = 0 ; i < count ; i++ ) {

			BEQuadChar stream_type ( *container, i );
			types->append ( stream_type.as_string() );

		} // for

		SetResult ( *types, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ContainerListTypes


fmx::errcode BE_ContainerGetType ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
{
	errcode error = NoError();

	try {

		const BinaryDataUniquePtr container ( parameters.AtAsBinaryData ( 0 ) );
		auto stream_type = ParameterAsUTF8String ( parameters, 1 );
		auto stream_index = IndexForStream ( *container, stream_type, false );

		if ( kBE_DataType_Not_Found != stream_index ) {

			fmx::TextUniquePtr file_name;
			auto fnam_error = container->GetFNAMData ( *file_name );

			short width = 0;
			short height = 0;
			auto size_error = container->GetSIZEData ( width, height );

			if ( stream_type == SIZE_CONTAINER_TYPE ) {

				error = size_error;
				stringstream size;
				size << width << FILEMAKER_END_OF_LINE << height;
				SetResult ( size.str(), results );

			} else if ( stream_type == FILENAME_CONTAINER_TYPE ) {

				error = fnam_error;
				SetResult ( *file_name, results );

			} else if ( stream_type == MAIN_CONTAINER_TYPE ) {

				auto quad_char = BinaryDataAsVectorChar ( *container, stream_index );
				SetResult ( quad_char, results );

			} else {

				auto stream_data = BinaryDataAsVectorChar ( *container, stream_index );
				auto name_of_file = TextAsUTF8String ( *file_name );
				if ( name_of_file.empty() ) {
					name_of_file = stream_type;
				}

				SetResult ( name_of_file, stream_data, stream_type, results );

			}

		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ContainerGetType


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


fmx::errcode BE_EncryptAES ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_EncryptAES



fmx::errcode BE_DecryptAES ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_DecryptAES


fmx::errcode BE_SignatureGenerateRSA ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_SignatureGenerateRSA

fmx::errcode BE_SignatureVerifyRSA ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_SignatureVerifyRSA


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


fmx::errcode BE_HTTP_GETFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& /* results */ )
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

} // BE_HTTP_GETFile


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


		} else if ( funcId == kBE_HTTP_PUTFile ) {

			path filename = ParameterAsPath ( parameters, 1 );
			BECurl curl ( url, kBE_HTTP_METHOD_PUT, filename, username, password );
			response = curl.http_put ( );


		} else { // kBE_HTTP_PUTData

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



fmx::errcode BE_HTTP_ResponseCode ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
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

} // BE_HTTP_ResponseCode



fmx::errcode BE_HTTP_ResponseHeaders ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		const unsigned long number_of_parameters = parameters.Size();

		if ( number_of_parameters == 0 ) { // get them all

			auto headers ( g_http_response_headers );
			SetResult ( headers, results );

		} else {

			auto http_headers = boost::replace_all_copy ( g_http_response_headers, NETWORK_ENDL, "\n" );

			BEValueList<string> headers ( http_headers, "\n", false, false );
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

} // BE_HTTP_ResponseHeaders


fmx::errcode BE_Net_Set_Header ( short funcId, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		if ( funcId == kBE_HTTP_SetCustomHeader ) {
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



fmx::errcode BE_HTTP_SetProxy ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_HTTP_SetCustomHeader


fmx::errcode BE_CurlSetOption ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
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

} // BE_CurlSetOption


fmx::errcode BE_CurlTrace ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& /* parameters */, fmx::Data& results )
{
	errcode error = kNoError; // do not set the global last error

	try {

		SetResult ( g_curl_trace.str(), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return error;

} // BE_CurlTrace


fmx::errcode BE_FTP_Upload ( short function_id, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto url = ParameterAsUTF8String ( parameters );
		auto username = ParameterAsUTF8String ( parameters, 2 );
		auto password = ParameterAsUTF8String ( parameters, 3 );

		std::vector<char> response;

		if ( function_id == kBE_FTP_Upload ) {

			auto data = ParameterAsVectorChar ( parameters, 1 );
			BECurl curl ( url, kBE_FTP_METHOD_UPLOAD, "", username, password, "", data );
			response = curl.ftp_upload ( );

		} else { // kBE_FTP_UploadFile

			auto file_to_upload = ParameterAsPath ( parameters, 1 );
			BECurl curl ( url, kBE_FTP_METHOD_UPLOAD, file_to_upload.string(), username, password );
			response = curl.ftp_upload ( );

		}

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


fmx::errcode BE_SMTPServer ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
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


fmx::errcode BE_SMTPSend ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
{
	errcode error = NoError();

	try {

		auto from = ParameterAsUTF8String ( parameters );
		auto to = ParameterAsStringValueList ( parameters, 1, true, false );
		auto subject = ParameterAsUTF8String ( parameters, 2 );
		auto text = ParameterAsUTF8String ( parameters, 3 );
		auto html = ParameterAsUTF8String ( parameters, 7 );

		unique_ptr<BESMTPEmailMessage> message ( new BESMTPEmailMessage ( from, to, subject, text, html ) );

		auto cc = ParameterAsStringValueList ( parameters, 4, true, false );
		message->set_cc_addresses ( cc );

		auto bcc = ParameterAsStringValueList ( parameters, 5, true, false );
		message->set_bcc_addresses ( bcc );

		auto reply_to = ParameterAsUTF8String ( parameters, 6 );
		message->set_reply_to ( reply_to );

		auto attachments = ParameterAsStringValueList ( parameters, 8, true, false );
		for ( auto it = attachments->begin() ; it != attachments->end() ; it++ ) {
			g_smtp_attachments.add ( *it );
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

} // BE_SMTPSend


fmx::errcode BE_SMTPAddAttachment ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& /* results */ )
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

} // BE_SMTPAddAttachment



#pragma mark -
#pragma mark Value Lists
#pragma mark -


fmx::errcode BE_ValuesUnique ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto case_sensitive = ParameterAsBoolean ( parameters, 1 );
		auto values = ParameterAsStringValueList ( parameters, 0, case_sensitive );
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

} // BE_ValuesUnique



fmx::errcode BE_ValuesFilterOut ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_ValuesFilterOut


fmx::errcode BE_ValuesContainsDuplicates ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );
		auto case_sensitive = ParameterAsBoolean ( parameters, 1 );

		BEValueListStringUniquePtr values ( new BEValueList<string> ( value_list, case_sensitive ) );
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

} // BE_ValuesContainsDuplicates


fmx::errcode BE_ValuesSort ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );
		auto ascending = ParameterAsBoolean ( parameters, 1, true );
		auto type = ParameterAsBoolean ( parameters, 2, kBE_DataType_String );

		unique_ptr< BEValueList<string> > values ( new BEValueList<string> ( value_list ) );
		auto sorted_values = values->sort ( ascending, type );

		SetResult ( sorted_values, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ValuesSort


fmx::errcode BE_ValuesTimesDuplicated ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto value_list = ParameterAsUTF8String ( parameters );
		const long numberOfTimes = ParameterAsLong ( parameters, 1 );

		unique_ptr< BEValueList<string> > values ( new BEValueList<string> ( value_list ) );
		BEValueList<std::string> times_duplicated = values->times_duplicated ( numberOfTimes );
		SetResult ( times_duplicated, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ValuesTimesDuplicated


fmx::errcode BE_ValuesTrim ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto values = ParameterAsStringValueList ( parameters );
		values->trim_values();
		SetResult ( *values, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ValuesTrim



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


fmx::errcode BE_VectorDotProduct ( short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& results )
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

} // BE_VectorDotProduct


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


fmx::errcode BE_PDFAppend ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto pdf_document = ParameterAsPDF ( parameters );

		auto which = 1;

		if ( BinaryDataAvailable ( parameters, which ) ) {

			auto pdf = ParameterAsVectorChar ( parameters, which );

			std::unique_ptr<PoDoFo::PdfMemDocument> pdf_document_to_append ( new PoDoFo::PdfMemDocument ( ) );
			pdf_document_to_append->LoadFromBuffer ( pdf.data(), (long)pdf.size() );
			pdf_document->Append ( *pdf_document_to_append );
			pdf_document_to_append.reset(); // make sure to close the file

		} else {

			auto file_list = ParameterAsStringValueList ( parameters, which, true, false );

			for ( typename BEValueList<string>::iterator it = file_list->begin() ; it != file_list->end(); ++it ) {

				boost::filesystem::path pdf_path ( *it );
				pdf_path.make_preferred();

				std::unique_ptr<PoDoFo::PdfMemDocument> pdf_document_to_append ( new PoDoFo::PdfMemDocument ( ) );
				pdf_document_to_append->Load ( pdf_path.c_str() );
				pdf_document->Append ( *pdf_document_to_append );
				pdf_document_to_append.reset(); // make sure to close the file

			} // for...

		} // if ( BinaryDataAvailable...

		// write out a temporary file
		auto temporary_file = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
		pdf_document->Write ( temporary_file.c_str() );
		pdf_document.reset(); // make sure to close the file

		auto output_path = ParameterAsPath ( parameters, 2 );
		if ( output_path.empty() ) {

			auto file_data = ReadFileAsBinary ( temporary_file );
			auto destination = ParameterFileName ( parameters );
			SetResult ( destination, file_data, PDF_CONTAINER_TYPE, results );

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

} // BE_PDFAppend


fmx::errcode BE_PDFPageCount ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_PDFPageCount


fmx::errcode BE_PDFGetPages ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto pdf_document = ParameterAsPDF ( parameters );
		auto number_of_pages_in_pdf = pdf_document->GetPageCount();

		auto from = ParameterAsLong ( parameters, 2 );
		auto first_page = 1L;
		if ( from > first_page ) {
			first_page = from;
		}

		auto to = ParameterAsLong ( parameters, 3 );
		auto number_of_pages_to_get = to;
		if ( number_of_pages_to_get <= 0 || number_of_pages_to_get > number_of_pages_in_pdf ) {
			number_of_pages_to_get = number_of_pages_in_pdf;
		}

		if ( number_of_pages_to_get > 0 ) {

			std::vector<long> pages_wanted;
			for ( long i = first_page ; i <= number_of_pages_to_get ; i++ ) {
				pages_wanted.push_back ( i - 1 );
			}

			std::unique_ptr<BEPDFDocument> new_pdf ( new BEPDFDocument() );
			new_pdf->InsertPages ( *pdf_document, pages_wanted );
			pdf_document.reset(); // make sure to close the file

			// write out a temporary file
			auto temporary_file = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
			new_pdf->Write ( temporary_file.c_str() );
			new_pdf.reset(); // make sure to close the file

			auto output_path = ParameterAsPath ( parameters, 1 );
			if ( output_path.empty() ) {

				auto file_data = ReadFileAsBinary ( temporary_file );
				auto destination = ParameterFileName ( parameters );
				SetResult ( destination, file_data, PDF_CONTAINER_TYPE, results );

			} else {

				rename ( temporary_file, output_path );
				//	SetResult ( nothing, results );

			}
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

} // BE_PDFGetPages



#pragma mark -
#pragma mark Script Steps
#pragma mark -


fmx::errcode BE_ScriptStepInstall ( short /* function_id */, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& /* reply */ )
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

		error = (fmx::errcode)environment.RegisterScriptStep ( *g_be_plugin->id(), (const short)id, *name, *definition, *description, flags, (fmx::ExtPluginType)BE_ScriptStepPerform);

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

} // BE_ScriptStepInstall


fmx::errcode BE_ScriptStepRemove ( short /* function_id */, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& /* reply */ )
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

} // BE_ScriptStepRemove


fmx::errcode BE_ScriptStepPerform ( short function_id, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply )
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

} // ScriptStepPerform



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

			case kBE_TimeCurrentMilliseconds:
				reply = ptime_to_milliseconds ( boost::posix_time::microsec_clock::local_time() );
				break;

			case kBE_TimeUTCMilliseconds:
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

		SetResult ( (double)reply, results );

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
 BE_TextExtractWords implements are somewhat imperfect heuristic for finding
 script variables within chunks of filemaker calculation

 try to stip out unwanted text such as strings and comments and then, when a $ is
 found, attempt to guess the where the variable name ends
 */

fmx::errcode BE_TextExtractWords ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		BEValueListWideStringUniquePtr variables ( new BEValueList<std::wstring> );
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

			} // switch

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

} // BE_TextExtractWords



fmx::errcode BE_ExecuteSystemCommand ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		SystemCommand::system_command command_to_execute;

		command_to_execute.command_text = ParameterAsUTF8String ( parameters );
		auto timeout = ParameterAsLong ( parameters, 1, kBE_Never );
		command_to_execute.execute_using_shell = ParameterAsBoolean ( parameters, 2, true );

		SystemCommand command;
		auto result = command.execute ( command_to_execute );

		switch ( timeout ) {

			case kBE_Never:
				result.wait ( );
				break;

			default:

				try {
					result.wait ( timeout );
				} catch ( Poco::TimeoutException& /* e */ ) {
					error = kCommandTimeout;
				}

		}

		if ( result.available() ) {
			SetResult ( result.data(), results );
		}

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

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



fmx::errcode BE_FileOpen ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
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

} // BE_FileOpen



fmx::errcode BE_ScriptExecute ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& results )
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
		if ( number_of_paramters >= 3 ) {

			LocaleUniquePtr default_locale;
			parameter->SetAsText ( parameters.AtAsText ( 2 ), *default_locale );

		}

		auto script_control = ParameterAsLong ( parameters, 3, kFMXT_Pause );

		error = ExecuteScript ( *script_name, *file_name, *parameter, script_control, environment );

		SetResult ( error, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_ScriptExecute



fmx::errcode BE_FileMakerSQL ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		TextUniquePtr expression;
		expression->SetText ( parameters.AtAsText(0) );

		FMX_UInt32 number_of_paramters = parameters.Size();

		TextUniquePtr filename;
		if ( number_of_paramters >= 4 ) {
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

		auto text_result_wanted = ParameterAsBoolean ( parameters, 4, true );
		sql->execute ( environment, text_result_wanted );


		if ( parameters.Size() >= 6 ) { // writing a file

			ios_base::openmode mode = ios_base::trunc;
			if ( !text_result_wanted ) {
				mode |= ios_base::binary;
			}

			auto path = ParameterAsUTF8String ( parameters, 5 );
			auto sql_result = sql->get_vector_result();
			error = write_to_file ( path, sql_result, mode );

		} else { // sending the result to fmp

			if ( text_result_wanted ) {

				auto sql_result = sql->get_text_result();
				SetResult ( *sql_result, results );

			} else {

				auto sql_result = sql->get_data_result();
				results.SetBinaryData ( sql_result->GetBinaryData() );

			}

		}


	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_FileMakerSQL



fmx::errcode BE_MessageDigest ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		auto message = ParameterAsUTF8String ( parameters );
		const unsigned long algorithm = ParameterAsLong( parameters, 1, kBE_MessageDigestAlgorithmSHA256 );
		const unsigned long output_type = ParameterAsLong( parameters, 2, kBE_EncodingHex );

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



fmx::errcode BE_JPEGRecompress ( const short /* function_id */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results )
{
	errcode error = NoError();

	try {

		vector<unsigned char> original_jpeg = ParameterAsVectorUnsignedChar ( parameters );

		if ( original_jpeg.size() > 0 ) {

			unique_ptr<BEJPEG> jpeg ( new BEJPEG ( original_jpeg ) );
			const int quality = (const int)ParameterAsLong ( parameters, 1, 75 ); // percent
			jpeg->set_compression_level ( quality );

			const double scale = ParameterAsDouble ( parameters, 2 );
			jpeg->set_scaling ( scale );

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

			SetResult ( filename, container_data, type, results );

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

		// should the input be treated as a value list ???
		auto treat_input_as_value_list = false;
		auto found = options.find ( "v" );
		if ( found != std::string::npos ) {
			treat_input_as_value_list = true;
		}

		std::vector<std::string> values;
		if ( treat_input_as_value_list ) {
			boost::split ( values, text, boost::is_any_of ( FILEMAKER_END_OF_LINE ), boost::token_compress_off );
		} else {
			values.push_back ( text );
		}

		// and match
		BEValueListStringUniquePtr matched_text ( new BEValueList<std::string> );
		for ( auto& each_value : values ) {
			matched_text->append ( regular_expression ( each_value, expression, options, replace_with, replace ) );
		}

        SetResult ( *matched_text, results );

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


fmx::errcode BE_GetMachineName ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
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

} // BE_GetMachineName


fmx::errcode BE_DebugInformation ( short /* funcId */, const ExprEnv& environment, const DataVect& /* parameters */, Data& results )
{
	errcode error = NoError();

	try {

		SetResult ( debug_information ( environment ), results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_DebugInformation


fmx::errcode BE_GetSystemDrive ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* parameters */, Data& results )
{
	errcode error = NoError();

	try {

		auto system_drive = get_system_drive();
		SetResult ( system_drive, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return MapError ( error );

} // BE_GetSystemDrive



