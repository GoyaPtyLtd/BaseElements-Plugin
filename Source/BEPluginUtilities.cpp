/*
 BEPluginUtilities.cpp
 BaseElements Plug-In
 
 Copyright 2010-2015 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEPluginGlobalDefines.h"


#if defined(FMX_MAC_TARGET)

	#include <CoreServices/CoreServices.h>
	#include <Carbon/Carbon.h>

#endif 


#include "BEPluginException.h"
#include "BEPluginUtilities.h"
#include "BEQuadChar.h"
#include "BEZlib.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-pedantic"
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wshadow"

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <utf8.h>

#pragma clang diagnostic pop


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

errcode TextConstantFunction ( wstring text, Data& results )
{
	
	WStringAutoPtr text_constant ( new wstring ( text ) );
	
	return TextConstantFunction ( text_constant, results );	
		
} // TextConstantFunction


errcode TextConstantFunction ( WStringAutoPtr text, Data& results )
{
	
	errcode error_result = kNoError;
	
	try {
		
		TextAutoPtr result_text;
		result_text->AssignWide ( text->c_str() );
		
		SetResult ( *result_text, results );
		
	} catch ( bad_alloc& /* e */ ) {
		error_result = kLowMemoryError;
	} catch ( exception& /* e */ ) {
		error_result = kErrorUnknown;
	}
	
	return error_result;
	
} // TextConstantFunction


// get parameters and set function results


#pragma mark -
#pragma mark SetResult
#pragma mark -

void SetResult ( const intmax_t number, Data& results )
{
	FixPtAutoPtr numeric_result;
	numeric_result->AssignDouble ( (double)number );
	results.SetAsNumber ( *numeric_result );
}


// Cannot use SetResult as the compiler is not disignusing const double and const intmax_t

void SetResultAsDouble ( const double number, Data& results )
{
//	FixPtAutoPtr numeric_result;
//	numeric_result->AssignDouble ( number );
//	results.SetAsNumber ( *numeric_result );

//	const std::string as_string = boost::lexical_cast<std::string>( number );
	
	// do this the c++ vay to avoid rounding errors
	ostringstream stream;
	stream << number;
	StringAutoPtr result ( new string );
	*result = stream.str();
	SetResult ( result, results );
}


void SetResult ( const Text& text, Data& results )
{
// defeat: Returning null reference (within a call to 'operator*')
// default constructor for default_locale gives the current locale
#ifndef __clang_analyzer__
	LocaleAutoPtr default_locale;
	results.SetAsText ( text, *default_locale );
#endif
}


void SetResult ( const string& text, Data& results )
{
	TextAutoPtr result_text;
	result_text->Assign ( text.c_str(), Text::kEncoding_UTF8 );
	SetResult ( *result_text, results );
}


void SetResult ( const wstring& text, Data& results )
{
	TextAutoPtr result_text;
	result_text->AssignWide ( text.c_str() );
	SetResult ( *result_text, results );
}


void SetResult ( const StringAutoPtr text, Data& results )
{
	if ( ! utf8::is_valid ( text->begin(), text->end() ) ) {
		throw BEPlugin_Exception ( kInvalidUTF8 );
	}

	TextAutoPtr result_text;
	result_text->Assign ( text->c_str(), Text::kEncoding_UTF8 );			
	SetResult ( *result_text, results );
}


void SetResult ( const WStringAutoPtr text, Data& results )
{
	TextAutoPtr result_text;
	result_text->AssignWide ( text->c_str() );			
	SetResult ( *result_text, results );
}

void SetResult ( vector<char>& data, Data& results )
{
	data.push_back ( '\0' );
	StringAutoPtr data_string ( new string ( &data[0], data.size() ) );
	SetResult ( data_string, results );

//	SetResult ( "", data, results );

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
		
		BinaryDataAutoPtr resultBinary;
		TextAutoPtr file;
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
		
			StringAutoPtr utf8 = ConvertTextToUTF8 ( (char *)&data[0], data.size() );

			if ( compress ) {
				vector<char> utf8_text ( utf8->begin(), utf8->end() );
				output = CompressContainerStream ( utf8_text );
			} else {
				output.assign ( utf8->begin(), utf8->end() );
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



#pragma mark -
#pragma mark ParameterAs
#pragma mark -

bool ParameterAsBoolean ( const DataVect& parameters, const FMX_UInt32 which, const bool default_value )
{
	try {
		return parameters.AtAsBoolean ( which );
	} catch ( exception& /* e */ ) {
		return default_value;
	}
}
	
	
long ParameterAsLong ( const DataVect& parameters, const FMX_UInt32 which, const unsigned long default_value )
{
	try {
		return parameters.AtAsNumber ( which ).AsLong();
	} catch ( exception& /* e */ ) {
		return default_value;
	}	
}


StringAutoPtr ParameterAsUTF8String ( const DataVect& parameters, const FMX_UInt32 which, const std::string default_value )
{	
	
	StringAutoPtr result ( new string ( default_value ) );
	
	try {
		
		TextAutoPtr raw_data;
		raw_data->SetText ( parameters.AtAsText ( which ) );
		
		result->assign ( TextAsUTF8String ( *raw_data ) );
		
	} catch ( exception& /* e */ ) {
		;	// return the default
	}
	
	return result;
	
} // ParameterAsUTF8String


WStringAutoPtr ParameterAsWideString ( const DataVect& parameters, const FMX_UInt32 which )
{	
	
	WStringAutoPtr result ( new wstring );
	
	try {
		
		TextAutoPtr raw_data;
		raw_data->SetText ( parameters.AtAsText(which) );
		
		FMX_Int32 text_size = raw_data->GetSize();
		FMX_UInt16 * text = new FMX_UInt16 [ text_size + 1 ];
		raw_data->GetUnicode ( text, 0, text_size );
        text[text_size] = 0x0000;

		// wchar_t is 4 bytes on OS X and 2 on Windows
		
		#if defined(FMX_MAC_TARGET)
		
			wchar_t * parameter = new wchar_t [ text_size + 1 ];
			for ( long i = 0 ; i <= text_size ; i++ ) {
				parameter[i] = (wchar_t)text[i];
			}
			delete [] text;
		
		#endif 
		
		#if defined(FMX_WIN_TARGET)
		
			wchar_t * parameter = (wchar_t*)text;
		
		#endif
		
		result->append ( parameter );
		delete [] parameter; // parameter == text on Windows
				
	} catch ( exception& /* e */ ) {
		;	// return an empty string
	}
	
	return result;
	
} // ParameterAsUnicodeString


vector<char> ParameterAsVectorChar ( const DataVect& parameters, const FMX_UInt32 which )
{

	vector<char> output;
	
	// make sure there's a parameter to get
	if ( parameters.Size() > which ) {

		const BinaryDataAutoPtr data ( parameters.AtAsBinaryData ( which ) );
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
			
			StringAutoPtr text = ParameterAsUTF8String ( parameters, which );
			output.assign ( text->begin(), text->end() );
			
		}

	}
	
	return output;
	
} // ParameterAsVectorChar


vector<unsigned char> ParameterAsVectorUnsignedChar ( const DataVect& parameters, const FMX_UInt32 which )
{

	vector<char> data = ParameterAsVectorChar ( parameters, which );
	vector<unsigned char> output (data.begin(), data.end() );

	return output;

} // ParameterAsVectorUnsignedChar


boost::filesystem::path ParameterAsPath ( const DataVect& parameters, const FMX_UInt32 which )
{
	
	WStringAutoPtr file = ParameterAsWideString ( parameters, which );
	boost::filesystem::path path = *file;
	path.make_preferred();
	
	return path;
	
}


StringAutoPtr ParameterFileName ( const DataVect& parameters, const FMX_UInt32 which )
{

	StringAutoPtr file_name ( new string );

	// make sure there's a parameter to get
	if ( parameters.Size() > which ) {

		const BinaryDataAutoPtr data ( parameters.AtAsBinaryData ( which ) );

		fmx::TextAutoPtr name_as_fmx_text;
		name_as_fmx_text->Assign ( "" ); // defeat clang: Returning null reference (within a call to 'operator*')
		data->GetFNAMData ( *name_as_fmx_text );
		std::string name_as_string = TextAsUTF8String ( *name_as_fmx_text );
		file_name->assign ( name_as_string );

	}

	return file_name;

} // ParameterFileName


#pragma mark -
#pragma mark Containers
#pragma mark -

int PreferredContainerType ( const BinaryData& data )
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

				if ( !stream_type.is_image_attribute() ) {

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


#pragma mark -
#pragma mark Data Streams
#pragma mark -


const fmx::int32 StreamIndex ( const BinaryData& data, const std::string stream_type )
{

	QuadCharAutoPtr type ( stream_type[0], stream_type[1], stream_type[2], stream_type[3] );

// defeat: Returning null reference (within a call to 'operator*')
#ifndef __clang_analyzer__
	fmx::int32 stream_index = data.GetIndex ( *type );
#else
	fmx::int32 stream_index = kBE_DataType_Not_Found;
#endif

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


vector<char> DataAsVectorChar ( const BinaryData& data, const FMX_UInt32 which )
{
	uint32 size = data.GetSize ( which );
	char * output_buffer = new char [ size ];
	data.GetData ( which, 0, size, (void *)output_buffer );

	vector<char> output ( output_buffer, output_buffer + size );

	delete[] output_buffer;
	
	return output;
}


bool StreamIsCompressed ( const BinaryData& data )
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

StringAutoPtr ReadFileAsUTF8 ( const boost::filesystem::path path )
{
	
	StringAutoPtr result ( new string );
	
	if ( exists ( path ) ) {
		size_t length = (size_t)file_size ( path ); // boost::uintmax_t
		
		boost::filesystem::ifstream inFile ( path, ios_base::in | ios_base::binary | ios_base::ate );
		inFile.seekg ( 0, ios::beg );
		
		// slurp up the file contents
		std::vector<char> buffer ( length );
		inFile.read ( &buffer[0], length );
		inFile.close ();
				
		// convert the text in the file to utf-8 if possible
		result = ConvertTextToUTF8 ( &buffer[0], length );
		if ( result->length() == 0 ) {
			result->assign ( &buffer[0] );
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



StringAutoPtr ConvertTextEncoding ( StringAutoPtr in, const string& to, const std::string& from )
{
	vector<char> text = ConvertTextEncoding ( (char *)in->c_str(), (const size_t)in->size() - 1, to, from );
	StringAutoPtr out ( new string ( text.begin(), text.end() ) );
	return out;
}


// convert text to utf-8
// currently handles utf-16, ascii and utf-8 text

StringAutoPtr ConvertTextToUTF8 ( char * in, const size_t length, const std::string& from )
{
	vector<char> text = ConvertTextEncoding ( in, length, UTF8, from );
	if ( ! utf8::is_valid ( text.begin(), text.end() ) ) {
		throw BEPlugin_Exception ( kInvalidUTF8 );
	}

	StringAutoPtr out ( new string ( text.begin(), text.end() ) );
	return out;

} // ConvertToUTF8


void SetTextEncoding ( const string& encoding )
{
	g_text_encoding = encoding;
}



string TextAsUTF8String ( const Text& fmx_text )
{
	
	string result;
	
	try {
		
		FMX_UInt32 text_size = (2*(fmx_text.GetSize())) + 1;
		char * text = new char [ text_size ]();
		fmx_text.GetBytes ( text, text_size, 0, (FMX_UInt32)Text::kSize_End, Text::kEncoding_UTF8 );
		result.assign ( text );
		delete [] text;
		
	} catch ( exception& /* e */ ) {
		;	// return an empty string
	}
	
	return result;
	
} // TextAsString



string TextAsNumberString ( const Text& fmx_text )
{
	std::string number_string = TextAsUTF8String ( fmx_text );
	
	// bug in fm text to float conversion removes a leading 0
	std::string decimal_point = ".";
	auto found = std::mismatch ( decimal_point.begin(), decimal_point.end(), number_string.begin() );
	if ( found.first == decimal_point.end()) {
		number_string = "0" + number_string;
	}

	return number_string;
}


string DataAsUTF8String ( const Data& data )
{
	return TextAsUTF8String ( data.GetAsText() );	
}




long DataAsLong ( const Data& data )
{
	FixPtAutoPtr number;
	number->AssignFixPt ( data.GetAsNumber() );
	
	return ( number->AsLong() );
}



double DataAsDouble ( const Data& data )
{
	FixPtAutoPtr number;
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
		
		TextAutoPtr database;
	
		if ( file_name.GetSize() != 0 ) {
			database->SetText ( file_name );
		} else {

			TextAutoPtr command;
			command->Assign ( "Get ( FileName )" );
			
			DataAutoPtr name;

// defeat: Returning null reference (within a call to 'operator*')
#ifndef __clang_analyzer__
			environment.Evaluate ( *command, *name );
#endif
			
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


bool AllowUserAbort ( const ExprEnv& environment )
{
	TextAutoPtr command;
	command->Assign ( "Get ( AllowAbortState )" );
	
	DataAutoPtr reply;

#ifdef __clang_analyzer__
	reply->Clear(); // defeat: Returning null reference (within a call to 'operator*')
#endif
	
	environment.Evaluate ( *command, *reply );
	bool allow_abort = reply->GetAsBoolean();

	return allow_abort;
}


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
#if defined(FMX_WIN_TARGET)
			LoadStringW( (HINSTANCE)(gFMX_ExternCallPtr->instanceID), kBE_OptionsStringID, (LPWSTR)string, (uint32)resultsize);
#endif
			
#if defined(FMX_MAC_TARGET)
			Sub_OSXLoadString(kBE_OptionsStringID, string, resultsize);
#endif

//			processedSpecialStringID = true;
			break;
		}
		default:
#if defined(FMX_WIN_TARGET)
			LoadStringW( (HINSTANCE)(gFMX_ExternCallPtr->instanceID), (unsigned int)whichString, (LPWSTR)string, (uint32)resultsize);
#endif
				
#if defined(FMX_MAC_TARGET)
			Sub_OSXLoadString(whichString, string, resultsize);
#endif
	
	} // switch (whichString)
	
	
} // Do_GetString (FMX_Unichar* version)


void Do_GetString(unsigned long whichStringID, TextAutoPtr& intoHere, bool stripFunctionParams)
{
	FMX_Unichar			tempBuffer[kBE_GetStringMaxBufferSize];
	
	Do_GetString ( whichStringID, 0, kBE_GetStringMaxBufferSize, tempBuffer );
	intoHere->AssignUnicode(tempBuffer);
	
	if(stripFunctionParams) {
		
		// The string for this whichStringID is a Function Prototype, but all the plug-in needs now is the Function Name by itself.
		
		TextAutoPtr		parenToken;
		parenToken->Assign ( " (" );
		
		FMX_UInt32 originalSize = intoHere->GetSize();
		FMX_UInt32 firstParenLocation;
		firstParenLocation = intoHere->Find(*parenToken, 0);
		
		intoHere->DeleteText(firstParenLocation, originalSize-firstParenLocation);
		
	} // stripFunctionParams
	
} // Do_GetString (TextAutoPtr version)



#if defined(FMX_MAC_TARGET)

unsigned long Sub_OSXLoadString(unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax)
{
	unsigned long returnResult = 0;
	
	if( (intoHere != NULL) && (intoHereMax > 1) ) {
		// Turn stringID to a textual identifier, then get the string from the .strings file as a null-term unichar array.
		CFStringRef 	strIdStr = CFStringCreateWithFormat( kCFAllocatorDefault, NULL, CFSTR("%ld"), stringID );
		
		// Note: The plug-in must be explicit about the bundle and file it wants to pull the string from.
		CFStringRef 	osxStr = CFBundleCopyLocalizedString( reinterpret_cast<CFBundleRef>(gFMX_ExternCallPtr->instanceID), strIdStr, strIdStr, CFSTR( BUNDLE_STRINGS_ID ) );
		
		if((osxStr != NULL) && (osxStr != strIdStr)) {
			
			long	osxStrLen = CFStringGetLength(osxStr);
			
			if( osxStrLen < (intoHereMax-1) ) {
				
				CFRange		allChars;
				allChars.location = 0;
				allChars.length = osxStrLen;
				
				CFStringGetCharacters(osxStr, allChars, (UniChar*)(intoHere));
				intoHere[osxStrLen] = 0x0000;
				returnResult = (unsigned long)osxStrLen;
				
			} // osxStrLen
			
		} // osxStr
		
		if ( osxStr ) { CFRelease( osxStr ); }
		CFRelease( strIdStr );
		
	} // intoHere
	
	return(returnResult);
	
} // Sub_OSXLoadString

#endif


