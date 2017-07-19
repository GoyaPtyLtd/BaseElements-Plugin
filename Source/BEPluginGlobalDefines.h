/*
 BEPluginGlobalDefines.h
 BaseElements Plug-In
 
 Copyright 2010-2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEPLUGINGLOBALDEFINES_H)
	#define BEPLUGINGLOBALDEFINES_H


#if defined __APPLE__

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

#elif defined _WIN64

	#define NOMINMAX // boost 1.53 and later break without this defined before including windows.h
	#include <windows.h> // life comes to an end if this is not included before anything else

	#define FMX_WIN_TARGET	1
	#define USER_PREFERENCES_DOMAIN L"Software\\Goya\\BaseElements\\PluginUser"
	#define FOPEN _wfopen

	#define PATH_MAX MAX_PATH

#elif defined __linux__

    #define FMX_LINUX_TARGET 1

    // tchar
    #define _TEXT(x) x
    #define FOPEN fopen
    #define USER_PREFERENCES_DOMAIN L"au.com.goya.baseelements.plugin-user"

#else

   error "Unknown compiler"

#endif

#define FMX_USE_UNIQUE_PTR 1

#ifdef __cplusplus

#include <FMWrapper/FMXBinaryData.h>
#include <FMWrapper/FMXCalcEngine.h>
#include <FMWrapper/FMXClient.h>
#include <FMWrapper/FMXData.h>
#include <FMWrapper/FMXDateTime.h>
#include <FMWrapper/FMXExtern.h>
#include <FMWrapper/FMXFixPt.h>
#include <FMWrapper/FMXText.h>
#include <FMWrapper/FMXTextStyle.h>
#include <FMWrapper/FMXTypes.h>

#include "BEPluginVersion.h"
#include "BECppUtilities.h"

extern fmx::errcode g_last_error;


#endif // __cplusplus


#define PLUGIN_ID		'G', 'y', 'B', 'E'
#define BUNDLE_STRINGS_ID "BaseElements"


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
	kBE_ClipboardText = 105,
	kBE_SetClipboardText = 106,
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
    kBE_File_Modification_Timestamp = 127,
	kBE_ExecuteScript = 150,
	kBE_FileMakerSQL = 151,
	kBE_HTTP_GET_File = 152,
	kBE_GetLastError = 153,
	kBE_MessageDigest_Deprecated = 154,
	kBE_GetLastDDLError = 155,
	kBE_SaveURLToFile = 156,
	kBE_ProgressDialog = 157,
	kBE_ProgressDialog_Update = 158,
	kBE_Pause = 159,
	kBE_SetPreference = 160,
	kBE_GetPreference = 161,
	kBE_Unzip = 170,
	kBE_Zip = 171,
	kBE_Base64_Decode_Deprecated = 172,
	kBE_Base64_Encode_Deprecated = 173,
	kBE_SetTextEncoding = 174,
	kBE_Base64_URL_Encode_Deprecated = 175,
	kBE_ExportFieldContents = 176,
	kBE_ImportFile = 177,
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
	kBE_Curl_Trace = 190,
	kBE_FTP_Delete = 191,
	kBE_HTTP_PATCH = 192,
	kBE_ApplyXSLTInMemory = 200,
	kBE_XPath = 201,
	kBE_XPathAll = 202,
	kBE_StripXMLNodes = 210,
	kBE_XML_Parse = 211,
	kBE_SplitBEFileNodes = 212,
	kBE_XML_Validate = 213,
	kBE_CurrentTimeMilliseconds = 220,
	kBE_UTCMilliseconds = 221,
	kBE_TimeZoneOffset = 222,
	kBE_SaveFileDialog = 230,
	kBE_RegularExpression = 240,
	kBE_ContainerIsCompressed = 250,
	kBE_ContainerCompress = 251,
	kBE_ContainerUncompress = 252,
	kBE_Gzip = 260,
	kBE_UnGzip = 261,
	kBE_JPEG_Recompress_Deprecated = 270, // deprecated
	kBE_JPEG_Recompress = 271,
	kBE_JSONPath_Deprecated = 300,
	kBE_JSON_Error_Description_Deprecated = 301,
	kBE_JSON_ArraySize_Deprecated = 302,
	kBE_JSON_Encode_Deprecated = 303,
//	kBE_OAuth_RequestToken = 320,
	kBE_OAuth_RequestAccessToken = 321,
//	kBE_OAuth_SetToken = 322,
	kBE_Xero_SetTokens = 330,
	kBE_Xero_GenerateKeys = 331,
	kBE_Values_Unique = 350,
	kBE_Values_FilterOut = 351,
	kBE_Values_ContainsDuplicates = 352,
	kBE_Values_Sort = 353,
	kBE_Values_TimesDuplicated = 354,
	kBE_Values_Trim = 355,
	kBE_SMTP_Server = 400,
	kBE_SMTP_Send = 401,
	kBE_SMTP_AddAttachment = 402,
	kBE_SMTP_Set_Header = 403,
	kBE_FMS_Upload_Database = 450,
	kBE_FMS_Download_Database = 451,
	kBE_FMS_List_Files = 452,
	kBE_FMS_Pause_Files = 453,
	kBE_FMS_Run_Schedule = 454,
	kBE_FMS_List_Schedules = 455,
	kBE_FMS_List_Clients = 456,
	kBE_FMS_Resume_Files = 457,
	kBE_FMS_Close_Files = 458,
	kBE_FMS_Remove_Files = 459,
	kBE_FMS_Open_Files = 460,
	kBE_Encrypt_AES_Deprecated = 500,
	kBE_Decrypt_AES_Deprecated = 501,
	kBE_HMAC_Deprecated = 550,
//	kBE_Encoding_TextToHex = 570, // removed
//	kBE_Encoding_HexToText = 571, // removed
	kBE_EvaluateJavaScript = 600,
	kBE_ArraySetFromValueList = 650,
	kBE_ArrayGetSize = 651,
	kBE_ArrayGetValue = 652,
	kBE_Array_Delete = 653,
	kBE_ConvertContainer = 700,
	kBE_PDF_Append = 750,
	kBE_XOR = 800,
	kBE_Vector_DotProduct = 900,
	kBE_Vector_EuclideanDistance = 901,
	kBE_Get_Machine_Name = 950,
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
	kCommandIsUnavailableError = 3,
	kLowMemoryError = 7,
	kRequestedDataIsMissingError = 10,
	kNameIsNotValid = 11,
	kFileOrObjectIsInUse = 13,
	kFileExistsError = 17,
	kErrorParameterMissing = 102,
	kWindowIsMissingError = 112,
	kNotFoundError = 401,
	kInvalidFieldType = 413,
	kCannotInsertAsImage = 735,
	kFileSystemError = 10000,
	kInvalidUTF8 = 10100,
	kZipError = 10200,
	kDDLCommandQueued = 11000,
	kCommandTimeout = 12000,
	kNotImplemented = 13000,
	kKeyEncodingError = 14000,
	kEncryptionInitialisationFailed = 14001,
	kEncryptionUpdateFailed = 14002,
	kDecryptionUpdateFailed = 14003,
	kDecryptionInputVectorNotFound = 14004,
	kJPEGReadHeaderError = 15000,
	kJPEGInitDecompressorError = 15001,
	kJPEGDecompressionError = 15002,
	kJPEGCompressionError = 15003,
	kJPEGInitCcompressorError = 15004,
    kMessageDigestDecodeKeyError = 16000,
    kMessageDigestDecodeMessageError = 16001,
	kHexEncodingFailed = 16002,
	kHexDecodingFailed = 16003
};


enum {
	kBE_NumericConstantOffset = 1000,
	kBE_ButtonOffset = 1000,
	kBE_MessageDigestAlgorithmOffset_Deprecated = 2000,
	kBE_FileTypeOffset = 3000,
	kBE_EncodingOffset = 4000,
	kBE_GetStringMaxBufferSize = 4096
};


enum file_type {
	kBE_FileType_All = 0,
	kBE_FileType_File = 1,
	kBE_FileType_Folder = 2
};


enum data_type {
	kBE_DataType_Not_Found = -1, 
	kBE_DataType_String = 0,
	kBE_DataType_Long = 0
};


enum timeout {
	kBE_Never = -1,
	kBE_Immediate = 0
};


#endif // BEPLUGINGLOBALDEFINES_H
