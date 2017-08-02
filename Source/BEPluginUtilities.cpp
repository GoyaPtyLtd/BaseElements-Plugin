/*
 BEPluginUtilities.cpp
 BaseElements Plug-In

 Copyright 2010-2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEPluginGlobalDefines.h"


#if defined FMX_MAC_TARGET

	#include <CoreServices/CoreServices.h>
	#include <Carbon/Carbon.h>
	#include <CoreFoundation/CoreFoundation.h>

	#include "BEAppleFunctionsCommon.h"
	#include "BEMacFunctions.h"

#elif defined FMX_IOS_TARGET

	#include "BEAppleFunctionsCommon.h"
	#include "BEIOSFunctions.h"

#elif defined FMX_LINUX_TARGET

	#include "BELinuxFunctions.h"

#endif


#include "BEPluginException.h"
#include "BEPluginUtilities.h"
#include "BEQuadChar.h"
#include "BEZlib.h"

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <utf8.h>

#include <zlib.h>

#include <sstream>
#include <iostream>
#include <iconv.h>


using namespace std;
using namespace fmx;
using namespace boost::filesystem;


extern errcode g_last_ddl_error;


#pragma mark -
#pragma mark Plug-In Utilities
#pragma mark -

// convenience functions that handle most of the work needed to return text from a function

const errcode TextConstantFunction ( const std::wstring& text, Data& results )
{

	errcode error = kNoError;

	try {

		TextUniquePtr result_text;
		result_text->AssignWide ( text.c_str() );

		SetResult ( *result_text, results );

	} catch ( BEPlugin_Exception& e ) {
		error = e.code();
	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return error;

} // TextConstantFunction


#pragma mark -
#pragma mark SetResult
#pragma mark -


void SetResult ( const double number, Data& results )
{
	FixPtUniquePtr numeric_result;
	numeric_result->AssignDouble ( number );
	results.SetAsNumber ( *numeric_result );
}


void SetResultAsDoubleAsText ( const double number, Data& results )
{
	// do this the c++ vay to avoid rounding errors
	ostringstream stream;
	stream << number;
	SetResult ( stream.str(), results );
}


void SetResult ( const Text& text, Data& results )
{
	LocaleUniquePtr default_locale;
	results.SetAsText ( text, *default_locale );
}


void SetResult ( const string& text, Data& results )
{
	if ( ! utf8::is_valid ( text.begin(), text.end() ) ) {
		throw BEPlugin_Exception ( kInvalidUTF8 );
	}

	TextUniquePtr result_text;
	result_text->Assign ( text.c_str(), Text::kEncoding_UTF8 );
	SetResult ( *result_text, results );
}


void SetResult ( const wstring& text, Data& results )
{
	TextUniquePtr result_text;
	result_text->AssignWide ( text.c_str() );
	SetResult ( *result_text, results );
}



void SetResult ( vector<char>& data, Data& results )
{
	data.push_back ( '\0' );
	const std::string data_string ( &data[0] );//, data.size() );
	SetResult ( data_string, results );
}


void SetResult ( const vector<unsigned char>& data, Data& results )
{
	vector<char> char_data ( data.begin(), data.end() );
	SetResult ( char_data, results );
}


void SetResult ( const std::string& filename, const vector<char>& data, Data& results, std::string data_type )
{
	const short width = 0;
	const short height = 0;

	return SetResult ( filename, data, data_type, width, height, results );
}


void SetResult ( const std::string& filename, const vector<unsigned char>& data, Data& results, std::string data_type )
{
	vector<char> char_data ( data.begin(), data.end() );
	return SetResult ( filename, char_data, results, data_type );
}


void SetResult ( const std::string& filename, const vector<char>& data, const std::string& type, const short width, const short height, Data& results )
{
	bool as_binary = !filename.empty();

	vector<char> output = data;

	BEQuadChar data_type ( type );
	bool compress = data_type.is_zlib();

	if ( as_binary ) {	// if a file name is supplied send back a file

		BinaryDataUniquePtr resultBinary;
		TextUniquePtr file;
		file->Assign ( filename.c_str(), Text::kEncoding_UTF8 );
		resultBinary->AddFNAMData ( *file ); // error =

		//          { "snd " }

		if ( data_type.is_image() && (width != kErrorUnknown && height != kErrorUnknown ) ) {
			resultBinary->AddSIZEData ( width, height ); // error =
		}

		if ( compress ) {
			output = CompressContainerStream ( data );
		}
		
		resultBinary->Add ( *(data_type.get_type()), (FMX_UInt32)output.size(), (void *)&output[0] ); // error =
		results.SetBinaryData ( *resultBinary, true );

	} else { // otherwise try sending back text

		if ( data.size() > 0 ) {

			// filemaker will go into an infinite loop if non-utf8 data is set as utf8
			// so try to convert it first

			std::string utf8 = ConvertTextToUTF8 ( (char *)&data[0], data.size() );

			if ( compress ) {
				vector<char> utf8_text ( utf8.begin(), utf8.end() );
				output = CompressContainerStream ( utf8_text );
			} else {
				output.assign ( utf8.begin(), utf8.end() );
			}

		}

		SetResult ( output, results );

	}

} // SetResult


void SetResult ( const std::string& filename, BEImage& image, fmx::Data& results )
{

	const vector<unsigned char> unsigned_char_data = image.get_data();
	vector<char> char_data ( unsigned_char_data.begin(), unsigned_char_data.end() );

	return SetResult ( filename, char_data, image.get_type(), (short)image.get_width(), (short)image.get_height(), results );

} // SetResult


void SetResult ( const std::string& filename, const std::vector<char>& data, fmx::Data& results )
{
	return SetResult ( filename, data, FILE_CONTAINER_TYPE, 0, 0, results );
}


void SetResult ( const std::string& filename, const std::vector<unsigned char>& data, fmx::Data& results )
{
	return SetResult ( filename, data, results, FILE_CONTAINER_TYPE );
}



#pragma mark -
#pragma mark ParameterAs
#pragma mark -

const bool ParameterAsBoolean ( const DataVect& parameters, const FMX_UInt32 which, const bool default_value )
{
	try {
		return parameters.AtAsBoolean ( which );
	} catch ( exception& /* e */ ) {
		return default_value;
	}
}


const double ParameterAsDouble ( const fmx::DataVect& parameters, const FMX_UInt32 which, const bool default_value )
{
	try {
		return parameters.AtAsNumber ( which ).AsFloat();
	} catch ( exception& /* e */ ) {
		return default_value;
	}
}


const long ParameterAsLong ( const DataVect& parameters, const FMX_UInt32 which, const unsigned long default_value )
{
	try {
		return parameters.AtAsNumber ( which ).AsLong();
	} catch ( exception& /* e */ ) {
		return default_value;
	}
}


// since c++ index starts at 0 and fmp from 1

const long ParameterAsIndex ( const fmx::DataVect& parameters, const FMX_UInt32 which, const unsigned long default_value )
{
	return ParameterAsLong ( parameters, which, default_value ) - 1;
}


const std::string ParameterAsUTF8String ( const DataVect& parameters, const FMX_UInt32 which, const std::string default_value )
{

	std::string result ( default_value );

	try {

		TextUniquePtr raw_data;
		raw_data->SetText ( parameters.AtAsText ( which ) );

		result.assign ( TextAsUTF8String ( *raw_data ) );

	} catch ( exception& /* e */ ) {
		;	// return the default
	}

	return result;

} // ParameterAsUTF8String


const std::wstring ParameterAsWideString ( const DataVect& parameters, const FMX_UInt32 which )
{

	std::wstring result;

	try {

		TextUniquePtr raw_data;
		raw_data->SetText ( parameters.AtAsText(which) );

		FMX_Int32 text_size = raw_data->GetSize();
		FMX_UInt16 * text = new FMX_UInt16 [ text_size + 1 ];
		raw_data->GetUnicode ( text, 0, text_size );
        text[text_size] = 0x0000;

		// wchar_t is 4 bytes on OS X and 2 on Windows

		#if defined FMX_MAC_TARGET || defined FMX_IOS_TARGET || defined FMX_LINUX_TARGET
		
			wchar_t * parameter = new wchar_t [ text_size + 1 ];
			for ( long i = 0 ; i <= text_size ; i++ ) {
				parameter[i] = (wchar_t)text[i];
			}
			delete [] text;

		#elif defined FMX_WIN_TARGET

			wchar_t * parameter = (wchar_t*)text;

		#endif

		result.append ( parameter );
		delete [] parameter; // parameter == text on Windows

	} catch ( exception& /* e */ ) {
		;	// return an empty string
	}

	return result;

} // ParameterAsUnicodeString


const vector<char> ParameterAsVectorChar ( const DataVect& parameters, const FMX_UInt32 which )
{

	vector<char> output;

	// make sure there's a parameter to get
	if ( parameters.Size() > which ) {

		const BinaryDataUniquePtr data ( parameters.AtAsBinaryData ( which ) );
		int count = data->GetCount();

		if ( count > 0 ) {

			int which_type = PreferredContainerType ( *data );

			// dig the data out of the container stream

			if ( which_type != kBE_DataType_Not_Found ) {

				output = DataAsVectorChar ( *data, which_type );

				if ( StreamIsCompressed ( *data ) ) {
					output = UncompressContainerStream ( output );
				}

			} else {
				g_last_error = kRequestedDataIsMissingError;
			}

		} else {

			// if we don't have any streams try getting as text
			// note: we also end up here for anything inserted as QuickTime, which is probably not what the user wants, but...

			std::string text = ParameterAsUTF8String ( parameters, which );
			output.assign ( text.begin(), text.end() );

		}

	}

	return output;

} // ParameterAsVectorChar


const vector<unsigned char> ParameterAsVectorUnsignedChar ( const DataVect& parameters, const FMX_UInt32 which )
{

	vector<char> data = ParameterAsVectorChar ( parameters, which );
	vector<unsigned char> output (data.begin(), data.end() );

	return output;

} // ParameterAsVectorUnsignedChar


const std::vector<double> ParameterAsVectorDouble ( const fmx::DataVect& parameters, const FMX_UInt32 which )
{

	std::string value_list = ParameterAsUTF8String ( parameters, which );
	unique_ptr< BEValueList<string> > values ( new BEValueList<string> ( value_list ) );

	return values->get_as_vector_double();

} // ParameterAsVectorDouble


const boost::filesystem::path ParameterAsPath ( const DataVect& parameters, const FMX_UInt32 which )
{

	std::wstring file = ParameterAsWideString ( parameters, which );
	boost::filesystem::path path = file;
	path.make_preferred();

	return path;

}


const std::string ParameterFileName ( const DataVect& parameters, const FMX_UInt32 which )
{

	std::string file_name;

	// make sure there's a parameter to get
	if ( parameters.Size() > which ) {

		if ( BinaryDataAvailable ( parameters, which ) ) {

			const BinaryDataUniquePtr data ( parameters.AtAsBinaryData ( which ) );

			fmx::TextUniquePtr name_as_fmx_text;
			data->GetFNAMData ( *name_as_fmx_text );
			std::string name_as_string = TextAsUTF8String ( *name_as_fmx_text );

			// if the file name is for an image strip the image: prefix
			auto colon = name_as_string.find ( ":" );
			if ( colon != std::string::npos ) {
				name_as_string.erase ( 0, colon + 1 );
			}

			file_name.assign ( name_as_string );
			
		} else {
			auto path = ParameterAsPath ( parameters, which );
			file_name = path.filename().string();
		}

	}

	return file_name;

} // ParameterFileName


const std::string ParameterPathOrContainerAsUTF8 ( const DataVect& parameters, const fmx::uint32 which )
{
	std::string file_contents;
	
	if ( BinaryDataAvailable ( parameters, which ) ) {
		auto binary_contents = ParameterAsVectorChar ( parameters, which );
		const std::string temporary_contents ( binary_contents.begin(), binary_contents.end() );
		file_contents = temporary_contents;
	} else {
		auto file = ParameterAsPath ( parameters, which );
		file_contents = ReadFileAsUTF8 ( file );
	}
	
	return file_contents;
	
} // ParameterPathOrContainerAsUTF8


#if ! ( FMX_IOS_TARGET )
std::unique_ptr<PoDoFo::PdfMemDocument> ParameterAsPDF ( const DataVect& parameters, const fmx::uint32 which )
{
	std::unique_ptr<PoDoFo::PdfMemDocument> pdf_document ( new PoDoFo::PdfMemDocument ( ) );
	
	if ( BinaryDataAvailable ( parameters, which ) ) {
		auto pdf = ParameterAsVectorChar ( parameters, which );
		pdf_document->Load ( pdf.data(), pdf.size() );
	} else {
		auto pdf_path = ParameterAsPath ( parameters, which );
		pdf_path.make_preferred();
		pdf_document->Load ( pdf_path.c_str() );
	}
	
	return pdf_document;
	
} // ParameterPathOrContainerAsUTF8
#endif


#pragma mark -
#pragma mark Containers
#pragma mark -

const int PreferredContainerType ( const BinaryData& data )
{

	fmx::int32 which_type = IndexForStream ( data, MAIN_CONTAINER_TYPE );
	if ( which_type == kBE_DataType_Not_Found ) {

		which_type = IndexForStream ( data, FILE_CONTAINER_TYPE );

		if ( which_type == kBE_DataType_Not_Found ) {

			which_type = IndexForStream ( data, COMPRESSED_CONTAINER_TYPE );

			// and then a sound
			if ( which_type == kBE_DataType_Not_Found ) {
				which_type = IndexForStream ( data, SOUND_CONTAINER_TYPE );
			}

		}

		// try and guess which image format to try

		if ( which_type == kBE_DataType_Not_Found ) {

			fmx::int32 count = data.GetCount();

			for ( fmx::int32 i = 0 ; i < count ; i++ ) {

				BEQuadChar stream_type ( data, i );

				if ( !stream_type.is_image_attribute() && !stream_type.is_fork() ) {

					which_type = i;

					// don't overwrite another type with an fm generated jpeg preview
					if ( !stream_type.is_jpeg() ) {
						break;
					}

				}

			} // for

		}

	} // if MAIN_CONTAINER_TYPE

	return which_type;

} // PreferredContainerType


const bool BinaryDataAvailable ( const DataVect& parameters, const FMX_UInt32 which )
{

	// make sure there's a parameter to get
	if ( parameters.Size() > which ) {

		const BinaryDataUniquePtr data ( parameters.AtAsBinaryData ( which ) );
		int count = data->GetCount();

		return count > 0;
	}

	return false;

} // BinaryDataAvailable



#pragma mark -
#pragma mark Data Streams
#pragma mark -


const fmx::int32 StreamIndex ( const BinaryData& data, const std::string stream_type )
{

	QuadCharUniquePtr type ( stream_type[0], stream_type[1], stream_type[2], stream_type[3] );

	fmx::int32 stream_index = data.GetIndex ( *type );

	return stream_index;

}


const fmx::int32 IndexForStream ( const BinaryData& data, const std::string stream_type )
{

	fmx::int32 stream_index = StreamIndex ( data, stream_type );

	if ( stream_index != kErrorUnknown && stream_type == MAIN_CONTAINER_TYPE ) {

		char quad_char[4] = {};
		data.GetData ( stream_index, 0, 4, quad_char ); // error =
		stream_index = StreamIndex ( data, quad_char );

	}

	return stream_index;

}


const vector<char> DataAsVectorChar ( const BinaryData& data, const FMX_UInt32 which )
{
	uint32 size = data.GetSize ( which );
	char * output_buffer = new char [ size ];
	data.GetData ( which, 0, size, (void *)output_buffer );

	vector<char> output ( output_buffer, output_buffer + size );

	delete[] output_buffer;

	return output;
}


const bool StreamIsCompressed ( const BinaryData& data )
{
	bool compressed = false;

	int which_type = IndexForStream ( data, COMPRESSED_CONTAINER_TYPE );

	if ( which_type != kBE_DataType_Not_Found ) {
		compressed = true;
	}

	return compressed;

}


#pragma mark -
#pragma mark Files
#pragma mark -


const std::vector<char> ReadFileAsBinary ( const boost::filesystem::path path )
{
	
	std::vector<char> file_data;
	
	if ( exists ( path ) ) {
		size_t length = (size_t)file_size ( path ); // boost::uintmax_t
		
		if ( length > 0 ) {
			
			// slurp up the file contents
			boost::filesystem::ifstream input_file ( path, ios_base::in | ios_base::binary | ios_base::ate );
			input_file.exceptions ( boost::filesystem::ofstream::badbit | boost::filesystem::ofstream::failbit );
			input_file.seekg ( 0, ios::beg );
			file_data.resize ( length );
			input_file.read ( &file_data[0], length );
			
		}
		
	} else {
		throw BEPlugin_Exception ( kNoSuchFileOrDirectoryError );
	}
	
	return file_data;
	
} // ReadFileAsUTF8


std::string ReadFileAsUTF8 ( const boost::filesystem::path path )
{

	std::string result;

	if ( exists ( path ) ) {
		size_t length = (size_t)file_size ( path ); // boost::uintmax_t

		if ( length > 0 ) {

			boost::filesystem::ifstream inFile ( path, ios_base::in | ios_base::binary | ios_base::ate );
			inFile.seekg ( 0, ios::beg );

			// slurp up the file contents
			std::vector<char> buffer ( length );
			inFile.read ( &buffer[0], length );
			inFile.close ();

			// convert the text in the file to utf-8 if possible
			result = ConvertTextToUTF8 ( &buffer[0], length );
			if ( result.length() == 0 ) {
				result.assign ( &buffer[0] );
			}

		}

	} else {
		g_last_error = kNoSuchFileOrDirectoryError;
	}

	return result;

} // ReadFileAsUTF8



#pragma mark -
#pragma mark Unicode
#pragma mark -

vector<char> ConvertTextEncoding ( char * in, const size_t length, const string& to, const string& from )
{
	vector<string> codesets;
	if ( from != UTF8 ) {
		codesets.push_back ( from );
	}
	codesets.push_back ( UTF8 );
	codesets.push_back ( UTF16 ); // backwards compatibility with v1.2

	/*
	 there no clean way to determine the codeset of the supplied text so
	 try each converting from each of the codesets in turn
	 */

	const size_t kIconvError = -1;
	size_t error_result = kIconvError;
	vector<string>::iterator it = codesets.begin();

	vector<char> out;

	while ( error_result == kIconvError && it != codesets.end() ) {

		char * start = in;
		size_t start_length = length;

		size_t available = (length * 4) + 1;	// worst case for utf-32 to utf-8 ?
		std::vector<char> encoded ( available );
		char * encoded_start = &encoded[0];
		size_t remaining = available;

		iconv_t conversion = iconv_open ( to.c_str(), it->c_str() );
		if ( conversion != (iconv_t)kIconvError ) {
			error_result = iconv ( conversion, &start, &start_length, &encoded_start, &remaining );
			if ( error_result != kIconvError ) {
				iconv_close ( conversion );
				out.assign ( &encoded[0], &encoded[0] + available - remaining );
			} else {
				throw BEPlugin_Exception ( errno );
			}

		} else {
			throw BEPlugin_Exception ( errno );
		}

		++it;
	}

	return out;

} // ConvertTextEncoding



std::string ConvertTextEncoding ( std::string& in, const string& to, const std::string& from )
{
	vector<char> text = ConvertTextEncoding ( (char *)in.c_str(), (const size_t)in.size() - 1, to, from );
	std::string out ( text.begin(), text.end() );
	return out;
}


// convert text to utf-8
// currently handles utf-16, ascii and utf-8 text

std::string ConvertTextToUTF8 ( char * in, const size_t length, const std::string& from )
{
	vector<char> text = ConvertTextEncoding ( in, length, UTF8, from );
	if ( ! utf8::is_valid ( text.begin(), text.end() ) ) {
		throw BEPlugin_Exception ( kInvalidUTF8 );
	}

	std::string out ( text.begin(), text.end() );
	return out;

} // ConvertToUTF8


void SetTextEncoding ( const string& encoding )
{
	g_text_encoding = encoding;
}



std::string TextAsUTF8String ( const Text& fmx_text )
{

	std::string result;

	try {

		FMX_UInt32 text_size = ( 4 * ( fmx_text.GetSize() ) ) + 1;
		char * text = new char [ text_size ]();
		fmx_text.GetBytes ( text, text_size, 0, (FMX_UInt32)Text::kSize_End, Text::kEncoding_UTF8 );
		result.assign ( text );
		delete [] text;

	} catch ( exception& /* e */ ) {
		;	// return an empty string
	}

	return result;

} // TextAsString


std::string TextAsNumberString ( const Text& fmx_text )
{
	auto number_string = TextAsUTF8String ( fmx_text );

	if ( !number_string.empty() ) {

		// bug in fm text to float conversion removes a leading 0
		const std::string decimal_point = ".";
		auto found = std::mismatch ( decimal_point.begin(), decimal_point.end(), number_string.begin() );
		if ( found.first == decimal_point.end() ) {
			number_string = "0" + number_string;
		}

	} else {
		; // do nothing, this waste of code brought to you courtesy of msvc
	}

	return number_string;
}


string DataAsUTF8String ( const Data& data )
{
	return TextAsUTF8String ( data.GetAsText() );
}




long DataAsLong ( const Data& data )
{
	FixPtUniquePtr number;
	number->AssignFixPt ( data.GetAsNumber() );

	return ( number->AsLong() );
}



double DataAsDouble ( const Data& data )
{
	FixPtUniquePtr number;
	number->AssignFixPt ( data.GetAsNumber() );

	return ( number->AsFloat() );
}


#pragma mark -
#pragma mark FMX
#pragma mark -

errcode ExecuteScript ( const Text& script_name, const Text& file_name, const Data& parameter, const ExprEnv& environment )
{
	errcode error = kNoError;

	try {

		TextUniquePtr database;

		if ( file_name.GetSize() != 0 ) {
			database->SetText ( file_name );
		} else {

			TextUniquePtr command;
			command->Assign ( "Get ( FileName )" );

			DataUniquePtr name;
			environment.Evaluate ( *command, *name );

			database->SetText ( name->GetAsText() );

		}

		error = FMX_StartScript ( &(*database), &script_name, kFMXT_Pause, &parameter );

	} catch ( bad_alloc& /* e */ ) {
		error = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error = kErrorUnknown;
	}

	return error;

} // ExecuteScript


#pragma mark -
#pragma mark Errors
#pragma mark -

// Should be called at the start of each plugin function to clear out the global error

errcode NoError ( void )
{
	g_last_ddl_error = kNoError;

	g_last_error = kNoError;
	return g_last_error;
}

// use as the return value for al plugin functions
// only return errors to FileMaker that it can make sense of

errcode MapError ( const errcode error, const bool map )
{

	errcode mapped_error = kNoError;

	// map the error to a FileMaker error (so that FMP can display an
	// appropriate dialog etc.

	if ( map ) {

		mapped_error = error;
		g_last_error = error;

	} else {

		// map all other errors to "unknown"
		if ( error != kNoError || g_last_error != kNoError ) {
			mapped_error = kErrorUnknown;
		}

		// overwrite g_last_error, but only if it's not already set
		if ( g_last_error == kNoError ) {
			g_last_error = error;
		}

	}

	return mapped_error;
}


#pragma mark -
#pragma mark Other
#pragma mark -


void set_name_value_pair ( std::map<std::string, std::string>& pair, const std::string name, const std::string value ) {
	
	if ( value.empty() ) {
		pair.erase ( name );
	} else {
		pair [ name ] = value;
	}
	
} // set_name_value_pair


bool AllowUserAbort ( const ExprEnv& environment )
{
	TextUniquePtr command;
	command->Assign ( "Get ( AllowAbortState )" );

	DataUniquePtr reply;
	environment.Evaluate ( *command, *reply );
	bool allow_abort = reply->GetAsBoolean();

	return allow_abort;

} // AllowUserAbort


std::string GetFileMakerTemporaryDirectory ( const ExprEnv& environment )
{
	TextUniquePtr command;
	command->Assign ( "Get ( TemporaryPath )" );

	DataUniquePtr reply;
	environment.Evaluate ( *command, *reply );

	// we want the filesystem path, not the "filemaker" path
	std::string temporary_path = TextAsUTF8String ( reply->GetAsText() );
	auto root = temporary_path.find ( "/", 1 );
	temporary_path.erase ( 0, root );

	return temporary_path;

} // GetFileMakerTemporaryDirectory



/*
 The following functions are modifications of ones included in the Example plug-in
 shipped by FileMaker Inc. with the External Plug-In API on the CDs for
 FileMaker Developer/Pro Advance versions 7 through 10 and are used by permission.
 */

void Do_GetString(unsigned long whichString, FMX_PtrType /* winLangID */, FMX_PtrType resultsize, FMX_Unichar* string)
{

	switch ( whichString )
	{
		case kFMXT_OptionsStr:
		{
#if defined FMX_MAC_TARGET || defined FMX_IOS_TARGET
			Sub_OSXLoadString(kBE_OptionsStringID, string, resultsize, BUNDLE_STRINGS_ID);
#elif defined FMX_WIN_TARGET
			LoadStringW( (HINSTANCE)(gFMX_ExternCallPtr->instanceID), kBE_OptionsStringID, (LPWSTR)string, (uint32)resultsize);
#elif defined FMX_LINUX_TARGET
			Sub_LinuxLoadString ( (unsigned long)kBE_OptionsStringID, string, resultsize );
#endif
			break;
		}

		case kFMXT_AppConfigStr:
		{
#if defined FMX_MAC_TARGET || defined FMX_IOS_TARGET
			Sub_OSXLoadString ( PLUGIN_DESCRIPTION_STRING_ID, string, resultsize, BUNDLE_VERSION_ID );
#elif defined FMX_WIN_TARGET
			LoadStringW( (HINSTANCE)(gFMX_ExternCallPtr->instanceID), (unsigned int)PLUGIN_DESCRIPTION_STRING_ID, (LPWSTR)string, (uint32)resultsize);
#elif defined FMX_LINUX_TARGET
			Sub_LinuxLoadString ( PLUGIN_DESCRIPTION_STRING_ID, string, resultsize );
#endif
			break;
		}

		default:
#if defined FMX_MAC_TARGET || defined FMX_IOS_TARGET
			Sub_OSXLoadString ( whichString, string, resultsize, BUNDLE_STRINGS_ID );
#elif defined FMX_WIN_TARGET
			LoadStringW( (HINSTANCE)(gFMX_ExternCallPtr->instanceID), (unsigned int)whichString, (LPWSTR)string, (uint32)resultsize);
#elif defined FMX_LINUX_TARGET
			Sub_LinuxLoadString ( whichString, string, resultsize );
#endif

	} // switch ( whichString )


} // Do_GetString ( FMX_Unichar* version )


void Do_GetString ( const unsigned long whichStringID, TextUniquePtr& function_information, const int which_string )
{
	fmx::unichar16 temp_buffer [ PATH_MAX ];

	Do_GetString ( whichStringID, 0, PATH_MAX, temp_buffer );
	function_information->AssignUnicode ( temp_buffer );

	TextUniquePtr delimiter;

	switch ( which_string ) {
			
		case kFunctionName:
		{
			// The string for this whichStringID is a Function Prototype, but all the plug-in needs now is the Function Name by itself.

			delimiter->Assign ( " (" );
			fmx::uint32 wanted = function_information->Find ( *delimiter, 0 );
			fmx::uint32 length = function_information->GetSize();
			function_information->DeleteText ( wanted, length - wanted );
			
		}
		case kFunctionDescription:
		{
			delimiter->Assign ( "|" );
			fmx::uint32 wanted = function_information->Find ( *delimiter, 0 );
			function_information->DeleteText ( 0, wanted + 1 );
		}
		default:
		{
			;// nothing to do
		}

	} // stripFunctionParams

} // Do_GetString (TextUniquePtr version)



