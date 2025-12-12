/*
 BEPluginUtilities.h
 BaseElements Plug-In
	
 Copyright 2010-2025 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#if !defined(BEPLUGINUTILITIES_H)
	#define BEPLUGINUTILITIES_H


#include "BEPluginGlobalDefines.h"
#include "Images/BEImage.h"
#include "BEValueList.h"

#include <filesystem>
#include <map>
#include <vector>
#include <stdint.h>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wcomma"
#include <podofo/podofo.h>
#pragma GCC diagnostic pop


enum {
	kFunctionName,
	kFunctionPrototype,
	kFunctionDescription
};


extern thread_local std::string g_text_encoding;


// convert "text literal" into L"text literal"
#define CAT(A, B)   A##B
#define WSTRING(A)  CAT(L, A)


const fmx::errcode TextConstantFunction ( const std::wstring& text, fmx::Data& results );

void SetResult(const double number, fmx::Data& results);
void SetResultAsDoubleAsText ( const double number, fmx::Data& results );
void SetResult ( const fmx::Text& text, fmx::Data& results );

void SetResult ( const std::string& text, fmx::Data& results );
void SetResult ( const std::wstring& text, fmx::Data& results );
void SetResult ( std::vector<char>& data, fmx::Data& results );
void SetResult ( const std::vector<unsigned char>& data, fmx::Data& results );
void SetResult ( const std::string& filename, const std::vector<unsigned char>& data, fmx::Data& results, const std::string data_type );

void SetResult ( const std::string& filename, const std::vector<char>& data, const std::string& type, fmx::Data& results );

void SetResult ( const std::string& filename, BEImage& image, fmx::Data& results );
void SetResult ( const std::string& filename, const std::vector<char>& data, fmx::Data& results );
void SetResult ( const std::string& filename, const std::vector<unsigned char>& data, fmx::Data& results );
void SetResult ( const std::string& filename, PoDoFo::PdfMemDocument& pdf, fmx::Data& results );

void SetResult ( const BEValueList<std::string>& value_list, fmx::Data& results );

const bool ParameterAsBoolean ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const bool default_value = true );
const double ParameterAsDouble ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const bool default_value = 0.0 );
const long ParameterAsLong ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const unsigned long default_value = 0 );
const long ParameterAsIndex ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const unsigned long default_value = 0 );
const std::string ParameterAsUTF8String ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const std::string default_value = "" );
const std::wstring ParameterAsWideString ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const std::wstring default_value = L"" );
const BEValueListStringSharedPtr ParameterAsStringValueList ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const bool is_case_sensitive = true, const bool retain_empty_values = true );
const std::vector<char> ParameterAsVectorChar ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );
const std::vector<unsigned char> ParameterAsVectorUnsignedChar ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );
const std::vector<double> ParameterAsVectorDouble ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );
const std::filesystem::path ParameterAsPath ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0, const std::filesystem::path default_path = "" );
const std::string ParameterFileName ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );
const std::string ParameterPathOrContainerAsUTF8 ( const fmx::DataVect& parameters, const fmx::uint32 which = 0 );

std::unique_ptr<PoDoFo::PdfMemDocument> ParameterAsPDF ( const fmx::DataVect& parameters, const fmx::uint32 which = 0 );
const time_t ParameterAsEpochTime ( const fmx::DataVect& parameters, const fmx::uint32 which );

const int PreferredContainerType ( const fmx::BinaryData& data );
const bool BinaryDataAvailable ( const fmx::DataVect& parameters, const FMX_UInt32 which = 0 );

const fmx::int32 IndexForStream ( const fmx::BinaryData& data, const std::string stream_type, const bool resolve_main_stream = true );
const std::vector<char> BinaryDataAsVectorChar ( const fmx::BinaryData& data, const FMX_UInt32 which = 0 );
const std::vector<char> DataAsVectorChar ( const fmx::Data& data, const FMX_UInt32 which = 0 );
const bool StreamIsCompressed ( const fmx::BinaryData& data );

const std::vector<char> ReadFileAsBinary ( const std::filesystem::path path );
std::string ReadFileAsUTF8 ( const std::filesystem::path path );

std::vector<char> ConvertTextEncoding ( char * in, const size_t length, const std::string& to, const std::string& from = g_text_encoding );
std::string ConvertTextEncoding ( std::string& in, const std::string& to, const std::string& from = g_text_encoding );
const bool IsValidUTF8 ( const std::string& utf8 );
std::string ConvertTextToUTF8 ( char * in, const size_t length, const std::string& from = g_text_encoding );

std::string TextAsUTF8String ( const fmx::Text& fmx_text );
std::string TextAsNumberString ( const fmx::Text& fmx_text );
std::string DataAsUTF8String ( const fmx::Data& data );
long DataAsLong ( const fmx::Data& data );
double DataAsDouble ( const fmx::Data& data );


fmx::errcode ExecuteScript ( const fmx::Text& script_name, const fmx::Text& file_name, const fmx::Data& parameter, const FMX_ScriptControl script_control );
fmx::errcode ExecuteScript ( const std::string& script_name, const std::string& file_name, const std::string& parameter, const FMX_ScriptControl script_control );


fmx::errcode NoError ( void );
fmx::errcode MapError ( const fmx::errcode error = kNoError, const bool map = false );

std::string ConvertFileMakerEOLs ( std::string& in );

void set_name_value_pair ( const fmx::DataVect& parameters, std::map<std::string, std::string>& pairs );

bool AllowUserAbort ( const fmx::ExprEnv& environment );
std::string GetFileMakerTemporaryDirectory ( const fmx::ExprEnv& environment );

// extract localised strings from the plug-in resources

void Do_GetString ( unsigned long whichString, FMX_PtrType winLangID, FMX_PtrType resultsize, FMX_Unichar* string );
void Do_GetString ( const unsigned long whichStringID, fmx::TextUniquePtr& function_information, const int which_string = kFunctionName );


#endif // BEPLUGINUTILITIES_H
