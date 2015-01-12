/*
 BEPluginGlobalDefines.h
 BaseElements Plug-In
 
 Copyright 2010-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEPLUGINGLOBALDEFINES_H)
	#define BEPLUGINGLOBALDEFINES_H


#if defined( __APPLE__ )

	#define FMX_MAC_TARGET	1

	/*
	 Unloadable modules can not use the new style constant strings that Apple introduced
	 in 10.4. So we have to turn if off in the headers and the project file with
	 */
	#undef __CONSTANT_CFSTRINGS__

	#define USER_PREFERENCES_DOMAIN L"au.com.goya.baseelements.plugin-user"
	#define FOPEN fopen

	// tchar
	#define _TEXT(x) x

#elif defined( _MSC_VER )

	#define NOMINMAX // boost 1.53 and later break without this defined before including windows.h
	#include <windows.h> // life comes to an end if this is not included before anything else

	#define FMX_WIN_TARGET	1
	#define USER_PREFERENCES_DOMAIN L"Software\\Goya\\BaseElements\\PluginUser"
	#define FOPEN _wfopen

	#define PATH_MAX MAX_PATH

#endif


#define FILEMAKER_EPOCH_OFFSET 62135596800


#ifdef __cplusplus

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"

#include "FMWrapper/FMXBinaryData.h"
#include "FMWrapper/FMXCalcEngine.h"
#include "FMWrapper/FMXClient.h"
#include "FMWrapper/FMXData.h"
#include "FMWrapper/FMXDateTime.h"
#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXFixPt.h"
#include "FMWrapper/FMXText.h"
#include "FMWrapper/FMXTextStyle.h"
#include "FMWrapper/FMXTypes.h"

#pragma clang diagnostic pop


#include "BECppUtilities.h"

extern fmx::errcode g_last_error;


#endif // __cplusplus


#define PLUGIN_ID		'G', 'y', 'B', 'E'
#define BUNDLE_STRINGS_ID "BaseElements"

#define	VERSION_NUMBER_STRING L"3.1.0"
#define	AUTO_UPDATE_VERSION L"03009000"
#define VERSION_STRING "3.1"

#define USER_AGENT_STRING "libcurl-agent-base-elements-plugin/"VERSION_STRING


#define	TEMPORARY_FILE_SUFFIX L".be4.tmp"

#define FILEMAKER_END_OF_LINE "\r"
#define FILEMAKER_END_OF_LINE_CHAR '\r'

#define UTF8 "UTF-8"
#define UTF16 "UTF-16"


enum {
	kBE_OptionsStringID = 1
};


enum functions {
	kBE_Version = 102,
	kBE_VersionAutoUpdate = 103,
	kBE_ClipboardFormats = 104,
	kBE_ClipboardData = 105,
	kBE_SetClipboardData = 106,
	kBE_CreateFolder = 107,
	kBE_DeleteFile = 108,
	kBE_FileExists = 109,
	kBE_ReadTextFromFile = 110,
	kBE_WriteTextToFile = 111,
	kBE_SelectFile = 112,
	kBE_SelectFolder = 113,
	kBE_DisplayDialog = 114,
	kBE_ApplyXSLT = 115,
	kBE_ExtractScriptVariables = 116,
	kBE_StripInvalidUTF16CharactersFromXMLFile = 117,
	kBE_MoveFile = 118,
	kBE_CopyFile = 119,
//	kBE_ExecuteShellCommand = 120, // removed, do not use
	kBE_ListFilesInFolder = 121,
//	kBE_FileMaker_Tables = 122, // removed, do not use
//	kBE_FileMaker_Fields = 123, // removed, do not use
	kBE_OpenURL = 124,
	kBE_OpenFile = 125,
	kBE_FileSize = 126,
	kBE_ExecuteScript = 150,
	kBE_FileMakerSQL = 151,
	kBE_GetURL = 152,
	kBE_GetLastError = 153,
	kBE_MessageDigest = 154,
	kBE_GetLastDDLError = 155,
	kBE_SaveURLToFile = 156,
	kBE_ProgressDialog = 157,
	kBE_ProgressDialog_Update = 158,
	kBE_SetPreference = 160,
	kBE_GetPreference = 161,
	kBE_Unzip = 170,
	kBE_Zip = 171,
	kBE_Base64_Decode = 172,
	kBE_Base64_Encode = 173,
	kBE_SetTextEncoding = 174,
	kBE_Base64_URL_Encode = 175,
	kBE_ExportFieldContents = 176,
	kBE_HTTP_POST = 180,
	kBE_HTTP_Response_Code = 181,
	kBE_HTTP_Response_Headers = 182,
	kBE_HTTP_Set_Custom_Header = 183,
	kBE_HTTP_DELETE = 184,
	kBE_HTTP_PUT_File = 185,
	kBE_HTTP_Set_Proxy = 186,
	kBE_Curl_Set_Option = 187,
	kBE_HTTP_PUT_Data = 188,
	kBE_FTP_Upload = 189,
	kBE_ApplyXSLTInMemory = 200,
	kBE_XPath = 201,
	kBE_XPathAll = 202,
	kBE_StripXMLNodes = 210,
	kBE_XML_Parse = 211,
	kBE_SplitBEFileNodes = 212,
	kBE_CurrentTimeMilliseconds = 220,
	kBE_UTCMilliseconds = 221,
	kBE_TimeZoneOffset = 222,
	kBE_SaveFileDialog = 230,
	kBE_JSONPath = 300,
	kBE_JSON_Error_Description = 301,
	kBE_JSON_ArraySize = 302,
	kBE_JSON_Encode = 303,
//	kBE_OAuth_RequestToken = 320,
	kBE_OAuth_RequestAccessToken = 321,
//	kBE_OAuth_SetToken = 322,
	kBE_Xero_SetTokens = 330,
	kBE_Values_Unique = 350,
	kBE_Values_FilterOut = 351,
	kBE_Values_Sort = 353,
	kBE_SMTP_Server = 400,
	kBE_SMTP_Send = 401,
	kBE_Encrypt_AES = 500,
	kBE_Decrypt_AES = 501,
	kBE_HMAC = 550,
	kBE_EvaluateJavaScript = 600,
	kBE_ExecuteSystemCommand = 1120
};


enum dialog_buttons {
	kBE_OKButton = 1,
	kBE_CancelButton = 2,
	kBE_AlternateButton = 3
};


enum errors {
	kErrorUnknown = -1,
	kNoError = 0,
	kUserCancelledError = 1,
	kNoSuchFileOrDirectoryError = 2,
	kLowMemoryError = 7,
	kRequestedDataIsMissingError = 10,
	kNameIsNotValid = 11,
	kFileOrObjectIsInUse = 13,
	kFileExistsError = 17,
	kErrorParameterMissing = 102,
	kWindowIsMissingError = 112,
	kNotFoundError = 401,
	kFileSystemError = 10000,
	kDDLCommandQueued = 11000,
	kCommandTimeout = 12000,
	kNotImplemented = 13000,
	kKeyEncodingError = 14000,
	kEncryptionInitialisationFailed = 14001,
	kEncryptionUpdateFailed = 14002,
	kDecryptionUpdateFailed = 14003,
	kDecryptionInputVectorNotFound = 14004
};


enum {
	kBE_NumericConstantOffset = 1000,
	kBE_ButtonOffset = 1000,
	kBE_MessageDigestAlgorithmOffset = 2000,
	kBE_FileTypeOffset = 3000,
	kBE_EncodingOffset = 4000,
	kBE_GetStringMaxBufferSize = 4096
};


enum file_type {
	kBE_FileType_ALL = 0,
	kBE_FileType_File = 1,
	kBE_FileType_Folder = 2
};


enum data_type {
	kBE_DataType_Not_Found = -1
};


enum timeout {
	kBE_Never = -1,
	kBE_Immediate = 0
};


#endif // BEPLUGINGLOBALDEFINES_H
