/*
 BELinuxFunctions.cpp
 BaseElements Plug-In

 Copyright 2016-2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEPluginGlobalDefines.h"
#include "BELinuxFunctions.h"
#include "BEPluginUtilities.h"
#include "BEPluginException.h"

#include <map>

#include <unistd.h>
#include <limits.h>


using namespace std;


void InitialiseForPlatform ( )
{
}


std::wstring ClipboardFormats ( void )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return L"";
}


std::string ClipboardData ( std::wstring& atype )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return "";
}


bool SetClipboardData ( std::string& data, std::wstring& atype )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return false;
}


std::wstring SelectFile ( std::wstring& prompt, std::wstring& in_folder )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return L"";
}


std::wstring SelectFolder ( std::wstring& prompt, std::wstring& in_folder )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return L"";
}


std::wstring SaveFileDialog ( std::wstring& prompt, std::wstring& fileName, std::wstring& inFolder )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return L"";
}


int DisplayDialog ( std::wstring& title, std::wstring& message, std::wstring& ok_button, std::wstring& cancel_button, std::wstring& alternate_button )
{
	return kCommandIsUnavailableError;
}


// Progress Dialog

fmx::errcode DisplayProgressDialog ( const std::wstring& title, const std::wstring& description, const long maximum, const bool can_cancel )
{
	return kCommandIsUnavailableError;
}


fmx::errcode UpdateProgressDialog ( const long value, const std::wstring& description )
{
	return kCommandIsUnavailableError;
}



// user preferences

bool SetPreference ( std::wstring& key, std::wstring& value, std::wstring& domain )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return false;
}


std::wstring GetPreference ( std::wstring& key, std::wstring& domain )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return L"";
}


// other

bool OpenURL ( std::wstring& url )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return false;
}


bool OpenFile ( std::wstring& path )
{
	throw BEPlugin_Exception ( kCommandIsUnavailableError );

	return false;
}


const std::wstring get_machine_name ( )
{

	char host_name [ HOST_NAME_MAX ];
	const size_t error = gethostname ( host_name, HOST_NAME_MAX );

	wchar_t w_host_name [ HOST_NAME_MAX * 4 ];
	if ( error != kErrorUnknown ) {
		const size_t length = mbstowcs ( w_host_name, host_name, HOST_NAME_MAX * 4 );
		if ( length == kErrorUnknown ) {
			throw BEPlugin_Exception ( errno );
		}
	} else {
		throw BEPlugin_Exception ( errno );
	}

	return w_host_name;

}


unsigned long Sub_LinuxLoadString ( const unsigned long string_id, FMX_Unichar * intoHere, const long intoHereMax )
{
	unsigned long returnResult = 0;

    std::map<unsigned long, std::string> strings;

    strings [ kBE_OptionsStringID ] = "GyBE1nnYYnn";
	strings [ kBE_Version ] = "BE_Version";
	strings [ kBE_VersionAutoUpdate ] = "BE_VersionAutoUpdate";
	strings [ kBE_ClipboardFormats ] = "BE_ClipboardFormats";
	strings [ kBE_ClipboardText ] = "BE_ClipboardText ( format )";
	strings [ kBE_SetClipboardText ] = "BE_SetClipboardText ( text ; format )";
	strings [ kBE_CreateFolder ] = "BE_CreateFolder ( path )";
	strings [ kBE_DeleteFile ] = "BE_DeleteFile ( path )";
	strings [ kBE_FileExists ] = "BE_FileExists ( path )";
	strings [ kBE_ReadTextFromFile ] = "BE_ReadTextFromFile ( path )";
	strings [ kBE_WriteTextToFile ] = "BE_WriteTextToFile ( path ; text {; append} )";
	strings [ kBE_SelectFile ] = "BE_SelectFile ( prompt {; inFolder } )";
	strings [ kBE_SelectFolder ] = "BE_SelectFolder ( prompt {; inFolder } )";
	strings [ kBE_DisplayDialog ] = "BE_DisplayDialog ( title ; message ; defaultButton {; cancelButton ; alternateButton } )";
	strings [ kBE_ApplyXSLT ] = "BE_ApplyXSLT ( xmlPath ; xslt ; outputPath )";
	strings [ kBE_ExtractScriptVariables ] = "BE_ExtractScriptVariables ( calculationText { ; variablePrefix } )";
	strings [ kBE_StripInvalidUTF16CharactersFromXMLFile ] = "BE_StripInvalidUTF16CharactersFromXMLFile ( path ; { resultPath } )";
	strings [ kBE_MoveFile ] = "BE_MoveFile ( from ; to )";
	strings [ kBE_CopyFile ] = "BE_CopyFile ( from ; to )";
	strings [ kBE_ListFilesInFolder ] = "BE_ListFilesInFolder ( path {; type ; includeSubdirectories ; useFullPath ; includeHidden } )";
	strings [ kBE_OpenURL ] = "BE_OpenURL ( url )";
	strings [ kBE_OpenFile ] = "BE_OpenFile ( path )";
	strings [ kBE_FileSize ] = "BE_FileSize ( path )";
	strings [ kBE_File_Modification_Timestamp ] = "BE_File_Modification_Timestamp ( path )";
	strings [ kFMXT_NameStr ] = "BaseElements";
	strings [ kFMXT_OptionsStr ] = "GyBE1nnYYnn";
	strings [ kFMXT_HelpURLStr ] = "https://baseelementsplugin.zendesk.com/";
	strings [ kBE_WriteTextFileToContainer ] = "BE_WriteTextFileToContainer ( path ; text {; append} )";
	strings [ kBE_ExecuteScript ] = "BE_ExecuteScript ( scriptName {; fileName ; parameter } )";
	strings [ kBE_FileMakerSQL ] = "BE_FileMakerSQL ( sqlStatement {; columnSeparator ; rowSeparator ; database } )";
	strings [ kBE_HTTP_GET_File ] = "BE_HTTP_GET ( url {; filename ; username ; password } )";
	strings [ kBE_GetLastError ] = "BE_GetLastError";
	strings [ kBE_MessageDigest_Deprecated ] = "BE_MessageDigest_Deprecated ( text {; algorithm ; encoding } )";
	strings [ kBE_GetLastDDLError ] = "BE_GetLastDDLError";
	strings [ kBE_HTTP_GET_File ] = "BE_HTTP_GET_File ( url ; path {; username ; password } )";
	strings [ kBE_ProgressDialog ] = "BE_ProgressDialog ( title ; description {; maximum } )";
	strings [ kBE_ProgressDialog_Update ] = "BE_ProgressDialog_Update ( number {; description } )";
	strings [ kBE_Pause ] = "BE_Pause ( milliseconds )";
	strings [ kBE_SetPreference ] = "BE_SetPreference ( key ; value {; domain } )";
	strings [ kBE_GetPreference ] = "BE_GetPreference ( key {; domain } )";
	strings [ kBE_Unzip ] = "BE_Unzip ( archive {; output_directory} )";
	strings [ kBE_Zip ] = "BE_Zip ( fileList {; archive} )";
	strings [ kBE_Base64_Decode_Deprecated ] = "BE_Base64_Decode_Deprecated ( text {; name } )";
	strings [ kBE_Base64_Encode_Deprecated ] = "BE_Base64_Encode_Deprecated ( data )";
	strings [ kBE_SetTextEncoding ] = "BE_SetTextEncoding ( { encoding } )";
	strings [ kBE_Base64_URL_Encode_Deprecated ] = "BE_Base64_URL_Encode_Deprecated ( data )";
	strings [ kBE_ExportFieldContents ] = "BE_ExportFieldContents ( field {; outputPath } )";
	strings [ kBE_ImportFile ] = "BE_ImportFile ( path {; compress } )";
	strings [ kBE_HTTP_POST ] = "BE_HTTP_POST ( url ; parameters {; username ; password ; filename } )";
	strings [ kBE_HTTP_Response_Code ] = "BE_HTTP_Response_Code";
	strings [ kBE_HTTP_Response_Headers ] = "BE_HTTP_Response_Headers ( { header } )";
	strings [ kBE_HTTP_Set_Custom_Header ] = "BE_HTTP_Set_Custom_Header ( { header ; value } )";
	strings [ kBE_HTTP_DELETE ] = "BE_HTTP_DELETE ( url {; username ; password } )";
	strings [ kBE_HTTP_PUT_File ] = "BE_HTTP_PUT_File ( url ; path {; username ; password } )";
	strings [ kBE_HTTP_Set_Proxy ] = "BE_HTTP_Set_Proxy ( proxy {; port ; username ; password } )";
	strings [ kBE_Curl_Set_Option ] = "BE_Curl_Set_Option ( { option ; value } )";
	strings [ kBE_HTTP_PUT_Data ] = "BE_HTTP_PUT_Data ( url ; data {; username ; password } )";
	strings [ kBE_FTP_Upload ] = "BE_FTP_Upload ( url ; data {; username ; password } )";
	strings [ kBE_Curl_Trace ] = "BE_Curl_Trace";
	strings [ kBE_FTP_Delete ] = "BE_FTP_Delete ( url {; username ; password } )";
	strings [ kBE_HTTP_PATCH ] = "BE_HTTP_PATCH ( url ; parameters {; username ; password } )";
	strings [ kBE_ApplyXSLTInMemory ] = "BE_ApplyXSLTInMemory ( xml ; xslt )";
	strings [ kBE_XPath ] = "BE_XPath ( xml ; xpath {; namespaceList ; asText } )";
	strings [ kBE_XPathAll ] = "BE_XPathAll ( xml ; xpath {; namespaceList } )";
	strings [ kBE_StripXMLNodes ] = "BE_StripXMLNodes ( inputPath ; outputPath ; nodeNames )";
	strings [ kBE_XML_Parse ] = "BE_XML_Parse ( path )";
	strings [ kBE_SplitBEFileNodes ] = "BE_SplitBEFileNodes ( path )";
	strings [ kBE_XML_Validate ] = "BE_XML_Validate ( xml ; schema )";
	strings [ kBE_CurrentTimeMilliseconds ] = "BE_CurrentTimeMilliseconds";
	strings [ kBE_UTCMilliseconds ] = "BE_UTCMilliseconds";
	strings [ kBE_TimeZoneOffset ] = "BE_TimeZoneOffset";
	strings [ kBE_SaveFileDialog ] = "BE_SaveFileDialog ( prompt {; fileName ; inFolder } )";
	strings [ kBE_RegularExpression ] = "BE_RegularExpression ( text ; expression {; options ; replaceString } )";
	strings [ kBE_ContainerIsCompressed ] = "BE_ContainerIsCompressed ( containerField )";
	strings [ kBE_ContainerCompress ] = "BE_ContainerCompress ( data {; filename } )";
	strings [ kBE_ContainerUncompress ] = "BE_ContainerUncompress ( gzip_data {; filename } )";
	strings [ kBE_Gzip ] = "BE_Gzip ( data {; filename } )";
	strings [ kBE_UnGzip ] = "BE_UnGzip ( gzip_data {; filename } )";
	strings [ kBE_JPEG_Recompress_Deprecated ] = "BE_JPEG_Recompress_Deprecated ( jpeg {; compressionLevel ; width ; height } )";
	strings [ kBE_JPEG_Recompress ] = "BE_JPEG_Recompress ( jpeg {; compressionLevel ; scale } )";
	strings [ kBE_JSONPath_Deprecated ] = "BE_JSONPath_Deprecated ( json ; path )";
	strings [ kBE_JSON_Error_Description_Deprecated ] = "BE_JSON_Error_Description_Deprecated";
	strings [ kBE_JSON_ArraySize_Deprecated ] = "BE_JSON_ArraySize_Deprecated ( json )";
	strings [ kBE_JSON_Encode_Deprecated ] = "BE_JSON_Encode_Deprecated ( key {; value ; type } )";
	strings [ kBE_OAuth_RequestAccessToken ] = "BE_OAuth_RequestAccessToken ( url ; consumer_key ; consumer_secret ; request_key ; request_secret )";
	strings [ kBE_Xero_SetTokens ] = "BE_Xero_SetTokens ( consumer_key ; private_key )";
	strings [ kBE_Xero_GenerateKeys ] = "BE_Xero_GenerateKeys ( organisation {; organisationalUnit ; country ; state ; suburb ; domain ; emailAddress } )";
	strings [ kBE_Values_Unique ] = "BE_Values_Unique ( listOfValues {; caseSensitive } )";
	strings [ kBE_Values_FilterOut ] = "BE_Values_FilterOut ( textToFilter ; filterValues {; caseSensitive } )";
	strings [ kBE_Values_ContainsDuplicates ] = "BE_Values_ContainsDuplicates ( listOfValues  {; caseSensitive } )";
	strings [ kBE_Values_Sort ] = "BE_Values_Sort ( listOfValues {; ascending ; type } )";
	strings [ kBE_Values_TimesDuplicated ] = "BE_Values_TimesDuplicated ( listOfValues ; numberOfTimes )";
	strings [ kBE_Values_Trim ] = "BE_Values_Trim ( listOfValues )";
	strings [ kBE_SMTP_Server ] = "BE_SMTP_Server ( server { ; port ; username ; password } )";
	strings [ kBE_SMTP_Send ] = "BE_SMTP_Send ( from ; to ; subject ; text { ; cc ; bcc ; replyTo ; html ; attachments } )";
	strings [ kBE_SMTP_AddAttachment ] = "BE_SMTP_AddAttachment ( { attachment ; contentType } )";
	strings [ kBE_SMTP_Set_Header ] = "BE_SMTP_Set_Header ( { header ; value } )";
	strings [ kBE_FMS_Upload_Database ] = "BE_FMS_Upload_Database ( container ; path )";
	strings [ kBE_FMS_Download_Database ] = "BE_FMS_Download_Database ( path )";
	strings [ kBE_FMS_List_Files ] = "BE_FMS_List_Files ( username ; password {; showStatistics } )";
	strings [ kBE_FMS_Pause_Files ] = "BE_FMS_Pause_Files ( username ; password {; files } )";
	strings [ kBE_FMS_Run_Schedule ] = "BE_FMS_Run_Schedule ( username ; password ; scheduleNumber )";
	strings [ kBE_FMS_List_Schedules ] = "BE_FMS_List_Schedules ( username ; password )";
	strings [ kBE_FMS_List_Clients ] = "BE_FMS_List_Clients ( username ; password {; showStatistics } )";
	strings [ kBE_FMS_Resume_Files ] = "BE_FMS_Resume_Files ( username ; password {; files } )";
	strings [ kBE_FMS_Close_Files ] = "BE_FMS_Close_Files ( username ; password {; files } )";
	strings [ kBE_FMS_Remove_Files ] = "BE_FMS_Remove_Files ( username ; password {; files } )";
	strings [ kBE_FMS_Open_Files ] = "BE_FMS_Open_Files ( username ; password {; files } )";
	strings [ kBE_Encrypt_AES_Deprecated ] = "BE_Encrypt_AES_Deprecated ( key ; text )";
	strings [ kBE_Decrypt_AES_Deprecated ] = "BE_Decrypt_AES_Deprecated ( key ; text )";
	strings [ kBE_SignatureGenerate_RSA ] = "BE_SignatureGenerate_RSA ( data ; privateKey {; privateKeyPassword ; algorithm; fileNameWithExtension } )";
	strings [ kBE_SignatureVerify_RSA ] = "BE_SignatureVerify_RSA ( data ; publicKey ; signature {; algorithm } )";
    strings [ kBE_CipherEncrypt ] = "BE_CipherEncrypt ( cipher ; data ; key ; iv {; padding ; fileNameWithExtension } )";
    strings [ kBE_CipherDecrypt ] = "BE_CipherDecrypt ( cipher ; encryptedData ; key ; iv {; padding ; fileNameWithExtension } )";
	strings [ kBE_HMAC_Deprecated ] = "BE_HMAC_Deprecated ( text ; key {; algorithm ; outputEncoding ; inputEncoding } )";
	strings [ kBE_EvaluateJavaScript ] = "BE_EvaluateJavaScript ( javaScript )";
	strings [ kBE_ArraySetFromValueList ] = "BE_ArraySetFromValueList ( valueList )";
	strings [ kBE_ArrayGetSize ] = "BE_ArrayGetSize ( array )";
	strings [ kBE_ArrayGetValue ] = "BE_ArrayGetValue ( array ; valueNumber )";
	strings [ kBE_Array_Delete ] = "BE_Array_Delete ( array )";
	strings [ kBE_Array_Find ] = "BE_Array_Find ( array ; value )";
	strings [ kBE_Array_Change_Value ] = "BE_Array_Change_Value ( array ; valueNumber ; newValue )";
	strings [ kBE_ConvertContainer ] = "BE_ConvertContainer ( field {; type ; width ; height } )";
	strings [ kBE_PDF_Append ] = "BE_PDF_Append ( pdf ; pdfToAppend {; destination } )";
	strings [ kBE_PDF_PageCount ] = "BE_PDF_PageCount ( pdf )";
	strings [ kBE_PDF_GetPages ] = "BE_PDF_GetPages ( pdf ; newPDF ; fromPage {; toPage } )";
	strings [ kBE_XOR ] = "BE_XOR ( text ; xorWith )";
	strings [ kBE_InstallScriptStep ] = "BE_InstallScriptStep ( name ; definition ; id ; description ; calculation )";
    strings [ kBE_RemoveScriptStep ] = "BE_RemoveScriptStep ( scriptStepId )";
    strings [ kBE_PerformScriptStep ] = "BE_PerformScriptStep ( scriptStepId )";
	strings [ kBE_Vector_DotProduct ] = "BE_Vector_DotProduct ( a ; b )";
	strings [ kBE_Vector_EuclideanDistance ] = "BE_Vector_EuclideanDistance ( a ; b )";
	strings [ kBE_Get_Machine_Name ] = "BE_Get_Machine_Name";
	strings [ kBE_ExecuteSystemCommand ] = "BE_ExecuteSystemCommand ( command { ; timeout } )";
	strings [ 1001 ] = "BE_OKButton";
	strings [ 1002 ] = "BE_CancelButton";
	strings [ 1003 ] = "BE_AlternateButton";
	strings [ 2001 ] = "BE_MessageDigestAlgorithm_MD5_Deprecated";
	strings [ 2002 ] = "BE_MessageDigestAlgorithm_SHA256_Deprecated";
	strings [ 2003 ] = "BE_MessageDigestAlgorithm_MDC2_Deprecated";
	strings [ 2004 ] = "BE_MessageDigestAlgorithm_SHA_Deprecated";
	strings [ 2005 ] = "BE_MessageDigestAlgorithm_SHA1_Deprecated";
	strings [ 2006 ] = "BE_MessageDigestAlgorithm_SHA224_Deprecated";
	strings [ 2007 ] = "BE_MessageDigestAlgorithm_SHA384_Deprecated";
	strings [ 2008 ] = "BE_MessageDigestAlgorithm_SHA512_Deprecated";
	strings [ 3001 ] = "BE_FileType_All";
	strings [ 3002 ] = "BE_FileType_File";
	strings [ 3003 ] = "BE_FileType_Folder";
	strings [ 4001 ] = "BE_Encoding_Hex_Deprecated";
	strings [ 4002 ] = "BE_Encoding_Base64_Deprecated";
	strings [ PLUGIN_DESCRIPTION_STRING_ID ] = "Version: 4.0b2\n\nThis plug-in provides additional functionality for BaseElements from Goya";

	std::string wanted;
	try {
		wanted = strings [ string_id ];
	} catch ( exception& /* e */ ) {
		returnResult = kRequestedDataIsMissingError;
	}

	if( (intoHere != NULL) && (intoHereMax > 1) ) {

		fmx::TextUniquePtr text;
		text->Assign ( wanted.c_str() );
		text->GetUnicode ( intoHere, 0, fmx::Text::kSize_End );
		intoHere[wanted.size()] = 0x0000;

	} else {
		returnResult = -123;
	}


	return returnResult;

} // Sub_LinuxLoadString

