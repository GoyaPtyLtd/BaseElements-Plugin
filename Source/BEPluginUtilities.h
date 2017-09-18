/*
 BEPluginUtilities.h
 BaseElements Plug-In
	
 Copyright 2010-2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#if !defined(BEPLUGINUTILITIES_H)
	#define BEPLUGINUTILITIES_H


#include "BEPluginGlobalDefines.h"
#include "Images/BEImage.h"

#include <boost/filesystem/path.hpp>

#include <map>
#include <vector>
#include <stdint.h>

#include <podofo/podofo.h>


enum {
	kFunctionName,
	kFunctionPrototype,
	kFunctionDescription
};


extern std::string g_text_encoding;


const fmx::errcode TextConstantFunction ( const std::wstring& text, fmx::Data& results );

void SetResult ( const double number, fmx::Data& results );
void SetResultAsDoubleAsText ( const double number, fmx::Data& results );
void SetResult ( const fmx::Text& text, fmx::Data& results );

void SetResult ( const std::string& text, fmx::Data& results );
void SetResult ( const std::wstring& text, fmx::Data& results );
void SetResult ( std::vector<char>& data, fmx::Data& results );
void SetResult ( const std::vector<unsigned char>& data, fmx::Data& results );
void SetResult ( const std::string& filename, const std::vector<char>& data, fmx::Data& results, const std::string data_type );
void SetResult ( const std::string& filename, const std::vector<unsigned char>& data, fmx::Data& results, const std::string data_type );

void SetResult ( const std::string& filename, const std::vector<char>& data, const std::string& data_type, const short width, const short height, fmx::Data& results );

void SetResult ( const std::string& filename, BEImage& image, fmx::Data& results );
void SetResult ( const std::string& filename, const std::vector<char>& data, fmx::Data& results );
void SetResult ( const std::string& filename, const std::vector<unsigned char>& data, fmx::Data& results );

const bool ParameterAsBoolean ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const bool default_value = true );
const double ParameterAsDouble ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const bool default_value = 0.0 );
const long ParameterAsLong ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const unsigned long default_value = 0 );
const long ParameterAsIndex ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const unsigned long default_value = 0 );
const std::string ParameterAsUTF8String ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const std::string default_value = "" );
const std::wstring ParameterAsWideString ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const std::wstring default_value = L"" );
const std::vector<char> ParameterAsVectorChar ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );
const std::vector<unsigned char> ParameterAsVectorUnsignedChar ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );
const std::vector<double> ParameterAsVectorDouble ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );
const boost::filesystem::path ParameterAsPath ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const boost::filesystem::path default_path = "" );
const std::string ParameterFileName ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );
const std::string ParameterPathOrContainerAsUTF8 ( const fmx::DataVect& parameters, const fmx::uint32 which = 0 );
std::unique_ptr<PoDoFo::PdfMemDocument> ParameterAsPDF ( const fmx::DataVect& parameters, const fmx::uint32 which = 0 );


const int PreferredContainerType ( const fmx::BinaryData& data );
const bool BinaryDataAvailable ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );

const fmx::int32 StreamIndex ( const fmx::BinaryData& data, const std::string stream_type );
const fmx::int32 IndexForStream ( const fmx::BinaryData& data, const std::string stream_type );
const std::vector<char> DataAsVectorChar ( const fmx::BinaryData& data, const FMX_UInt32 which );
const bool StreamIsCompressed ( const fmx::BinaryData& data );

const std::vector<char> ReadFileAsBinary ( const boost::filesystem::path path );
std::string ReadFileAsUTF8 ( const boost::filesystem::path path );
std::vector<char> ConvertTextEncoding ( char * in, const size_t length, const std::string& to, const std::string& from = g_text_encoding );
std::string ConvertTextEncoding ( std::string& in, const std::string& to, const std::string& from = g_text_encoding );
std::string ConvertTextToUTF8 ( char * in, const size_t length, const std::string& from = g_text_encoding );

void SetTextEncoding ( const std::string& encoding = UTF8 );
std::string TextAsUTF8String ( const fmx::Text& fmx_text );
std::string TextAsNumberString ( const fmx::Text& fmx_text );
std::string DataAsUTF8String ( const fmx::Data& data );
long DataAsLong ( const fmx::Data& data );
double DataAsDouble ( const fmx::Data& data );


fmx::errcode ExecuteScript ( const fmx::Text& script_name, const fmx::Text& file_name, const fmx::Data& parameter, const fmx::ExprEnv& environment );


fmx::errcode NoError ( void );
fmx::errcode MapError ( const fmx::errcode error = kNoError, const bool map = false );

void set_name_value_pair ( const fmx::DataVect& parameters, std::map<std::string, std::string>& pairs );

bool AllowUserAbort ( const fmx::ExprEnv& environment );
std::string GetFileMakerTemporaryDirectory ( const fmx::ExprEnv& environment );

// extract localised strings from the plug-in resources

void Do_GetString ( unsigned long whichString, FMX_PtrType winLangID, FMX_PtrType resultsize, FMX_Unichar* string );
void Do_GetString ( const unsigned long whichStringID, fmx::TextUniquePtr& function_information, const int which_string = kFunctionName );


#endif // BEPLUGINUTILITIES_H
