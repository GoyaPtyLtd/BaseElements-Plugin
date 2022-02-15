/*
 BEPluginGlobalDefines.h
 BaseElements Plug-In
 
 Copyright 2010-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEPLUGINGLOBALDEFINES_H)
	#define BEPLUGINGLOBALDEFINES_H


#if defined __APPLE__

	#include <TargetConditionals.h>

	#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR

		#define FMX_IOS_TARGET  1

	#elif TARGET_OS_MAC

		#define FMX_MAC_TARGET  1

	#endif

	#define USER_PREFERENCES_DOMAIN BE_PLUGIN_BUNDLE_IDENTIFIER".plugin-user"
	#define FOPEN fopen

	// tchar
	#define _TEXT(x) x

#elif defined _WIN64

	#define NOMINMAX // boost 1.53 and later break without this defined before including windows.h
	#include <windows.h> // life comes to an end if this is not included before anything else

	#define FMX_WIN_TARGET	1
	#define USER_PREFERENCES_DOMAIN "Software\\Goya\\BaseElements\\PluginUser"
	#define FOPEN _wfopen

	#define PATH_MAX MAX_PATH

#elif defined __linux__

    #define FMX_LINUX_TARGET 1

    // tchar
    #define _TEXT(x) x
    #define FOPEN fopen
	#define BE_PLUGIN_BUNDLE_IDENTIFIER "au.com.goya.baseelements"
	#define USER_PREFERENCES_DOMAIN BE_PLUGIN_BUNDLE_IDENTIFIER".plugin-user"

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

extern thread_local fmx::errcode g_last_error;

#endif // __cplusplus


#define PLUGIN_ID		'G', 'y', 'B', 'E'
#define BUNDLE_STRINGS_ID "BaseElements"


#define	TEMPORARY_FILE_SUFFIX L".be4.tmp"

#define FILEMAKER_END_OF_LINE "\r"
#define FILEMAKER_END_OF_LINE_CHAR '\r'
#define LINE_FEED "\n"
#define NETWORK_ENDL "\r\n"

#define UTF8 "UTF-8"
#define UTF16 "UTF-16"


enum {
	kBE_OptionsStringID = 1
};


enum functions {
	kBE_Version = 102,
	kBE_VersionAutoUpdate = 103,
	kBE_ClipboardFormats = 104,
	kBE_ClipboardGetText = 105,
	kBE_ClipboardSetText = 106,
	kBE_FolderCreate = 107,
	kBE_FileDelete = 108,
	kBE_FileExists = 109,
	kBE_FileReadText = 110,
	kBE_FileWriteText = 111,
	kBE_FileSelect = 112,
	kBE_FolderSelect = 113,
	kBE_DialogDisplay = 114,
	kBE_XSLTApply = 115,
	kBE_TextExtractWords = 116,
	kBE_XMLStripInvalidCharacters = 117,
	kBE_FileMove = 118,
	kBE_FileCopy = 119,
//	kBE_ExecuteShellCommand = 120, // removed, do not use
	kBE_FileListFolder = 121,
//	kBE_FileMaker_Tables = 122, // removed, do not use
//	kBE_FileMaker_Fields = 123, // removed, do not use
	kBE_OpenURL = 124,
	kBE_FileOpen = 125,
	kBE_FileSize = 126,
    kBE_FileModificationTimestamp = 127,
	kBE_DebugInformation = 134,
	kBE_ClipboardGetFile = 135,
	kBE_ClipboardSetFile = 136,
	kBE_GetSystemDrive = 137,
	kBE_WriteTextFileToContainer_Deprecated = 140,
	kBE_ScriptExecute = 150,
	kBE_FileMakerSQL = 151,
	kBE_HTTP_GETFile = 152,
	kBE_GetLastError = 153,
	kBE_MessageDigest = 154,
	kBE_GetLastDDLError = 155,
	kBE_SaveURLToFile = 156,
	kBE_DialogProgress = 157,
	kBE_DialogProgressUpdate = 158,
	kBE_Pause = 159,
	kBE_PreferenceSet = 160,
	kBE_PreferenceGet = 161,
	kBE_PreferenceDelete = 162,
	kBE_Unzip = 170,
	kBE_Zip = 171,
//	kBE_Base64_Decode_Deprecated = 172, // Removed in 4.2
//	kBE_Base64_Encode_Deprecated = 173, // Removed in 4.2
	kBE_SetTextEncoding = 174,
//	kBE_Base64_URL_Encode_Deprecated = 175, // Removed in 4.2
	kBE_ExportFieldContents = 176,
	kBE_FileImport = 177,
	kBE_FilePatternCount = 178,
	kBE_FileReplaceText = 179,
	kBE_HTTP_POST = 180,
	kBE_HTTP_ResponseCode = 181,
	kBE_HTTP_ResponseHeaders = 182,
	kBE_HTTP_SetCustomHeader = 183,
	kBE_HTTP_DELETE = 184,
	kBE_HTTP_PUTFile = 185,
	kBE_HTTP_SetProxy = 186,
	kBE_CurlSetOption = 187,
	kBE_HTTP_PUTData = 188,
	kBE_FTP_Upload = 189,
	kBE_CurlTrace = 190,
	kBE_FTP_Delete = 191,
	kBE_HTTP_PATCH = 192,
	kBE_FTP_UploadFile = 193,
	kBE_CurlGetInfo = 194,
	kBE_BackgroundTaskAdd = 195,
	kBE_BackgroundTaskList = 196,
	kBE_XSLT_ApplyInMemory = 200,
	kBE_XPath = 201,
	kBE_XPathAll = 202,
	kBE_XMLStripNodes = 210,
	kBE_XMLParse = 211,
	kBE_SplitBEFileNodes = 212,
	kBE_XMLValidate = 213,
	kBE_XMLCanonical = 214,
	kBE_XMLTidy = 215,
	kBE_TimeCurrentMilliseconds = 220,
	kBE_TimeUTCMilliseconds = 221,
	kBE_TimeZoneOffset = 222,
	kBE_FileSaveDialog = 230,
	kBE_RegularExpression = 240,
	kBE_ContainerIsCompressed = 250,
	kBE_ContainerCompress = 251,
	kBE_ContainerUncompress = 252,
	kBE_ContainerListTypes = 253,
	kBE_ContainerGetType = 254,
	kBE_Gzip = 260,
	kBE_UnGzip = 261,
//	kBE_JPEG_Recompress_Deprecated = 270, // deprecated => Removed in 4.2
	kBE_JPEGRecompress= 271,
	kBE_JSONPath_Deprecated = 300,
	kBE_JSON_Error_Description_Deprecated = 301,
	kBE_JSON_ArraySize = 302,
//	kBE_JSON_Encode_Deprecated = 303, // Removed in 4.2
//	kBE_OAuth_RequestToken = 320, // Never Implemented
	kBE_OAuthRequestAccessToken_Deprecated = 321, // Deprecated in 4.1.4 => Removed in 4.2
//	kBE_OAuth_SetToken = 322, // Never Implemented
	kBE_XeroSetTokens_Deprecated = 330, // Deprecated in 4.1.4 => Removed in 4.2
	kBE_XeroGenerateKeys_Deprecated = 331, // Deprecated in 4.1.4 => Removed in 4.2
	kBE_ValuesUnique = 350,
	kBE_ValuesFilterOut = 351,
	kBE_ValuesContainsDuplicates = 352,
	kBE_ValuesSort = 353,
	kBE_ValuesTimesDuplicated = 354,
	kBE_ValuesTrim = 355,
	kBE_SMTPServer = 400,
	kBE_SMTPSend = 401,
	kBE_SMTPAddAttachment = 402,
	kBE_SMTPSetHeader = 403,
//	kBE_FMS_Upload_Database = 450, // removed
//	kBE_FMS_Download_Database = 451, // removed
//	kBE_FMS_List_Files = 452, // removed
//	kBE_FMS_Pause_Files = 453, // removed
//	kBE_FMS_Run_Schedule = 454, // removed
//	kBE_FMS_List_Schedules = 455, // removed
//	kBE_FMS_List_Clients = 456, // removed
//	kBE_FMS_Resume_Files = 457, // removed
//	kBE_FMS_Close_Files = 458, // removed
//	kBE_FMS_Remove_Files = 459, // removed
//	kBE_FMS_Open_Files = 460, // removed
	kBE_VariableSet = 470,
	kBE_VariableGet = 471,
	kBE_StackPush = 472,
	kBE_StackPop = 473,
	kBE_StackCount = 474,
	kBE_StackDelete = 475,
	kBE_EncryptAES = 500,
	kBE_DecryptAES = 501,
	kBE_SignatureGenerateRSA = 502,
	kBE_SignatureVerifyRSA = 503,
	kBE_CipherEncrypt = 504,
	kBE_CipherDecrypt = 505,
//	kBE_HMAC_Deprecated = 550,, // removed in version 4.2
//	kBE_Encoding_TextToHex = 570, // removed
//	kBE_Encoding_HexToText = 571, // removed
	kBE_EvaluateJavaScript = 600,
	kBE_ArraySetFromValueList = 650,
	kBE_ArrayGetSize = 651,
	kBE_ArrayGetValue = 652,
	kBE_ArrayDelete = 653,
	kBE_ArrayFind = 654,
	kBE_ArrayChangeValue = 655,
	kBE_ConvertContainer = 700,
	kBE_PDFAppend = 750,
	kBE_PDFPageCount = 751,
	kBE_PDFGetPages = 752,
	kBE_XOR = 800,
	kBE_ScriptStepInstall = 850,
	kBE_ScriptStepRemove = 851,
	kBE_ScriptStepPerform = 852,
	kBE_VectorDotProduct = 900,
	kBE_Vector_EuclideanDistance = 901,
	kBE_GetMachineName = 950,
	kBE_ExecuteSystemCommand = 1120
};


enum dialog_buttons {
	kBE_ButtonOK = 1,
	kBE_ButtonCancel = 2,
	kBE_ButtonAlternate = 3
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
	kNameAlreadyExists = 12,
	kFileOrObjectIsInUse = 13,
	kFileExistsError = 17,
	kErrorParameterMissing = 102,
	kWindowIsMissingError = 112,
	kInvalidAccountOrPassword = 212,
	kNotFoundError = 401,
	kInvalidFieldType = 413,
	kXMLParseError = 718,
	kCannotInsertAsImage = 735,
	kSMTPConnectionFailure = 1507,
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
	kHexDecodingFailed = 16003,
	kRSALoadBioFailed = 17000,
	kRSAReadPrivateKeyFailed = 17001,
	kRSAReadPublicKeyFailed = 17002,
	kRSASignInitFailed = 17003,
	kRSASignUpdateFailed = 17003,
	kRSASignFinalFailed = 17004,
	kRSAVerifyInitFailed = 17005,
	kRSAVerifyUpdateFailed = 17006,
	kRSATranslateDigestFailed = 17007,
	kCipherTranslateCipherFailed = 17010,
	kCipherEncryptInitFailed = 17011,
	kCipherEncryptUpdateFailed = 17012,
	kCipherEncryptFinalFailed = 17013,
	kCipherMissingIV = 17014,
	kCipherDecryptInitFailed = 17015,
	kCipherDecryptUpdateFailed = 17016,
	kCipherDecryptFinalFailed = 17017,
	kCipherDataTooLarge = 17018,
	kCipherInvalidKeySize = 17019,
	kCipherInvalidIvSize = 17020
};

enum {
	kBE_NumericConstantOffset = 1000,
	kBE_ButtonOffset = 1000,
	kBE_MessageDigestAlgorithmOffset = 2000,
	kBE_FileTypeOffset = 3000,
	kBE_EncodingOffset = 4000,
	kBE_ScriptControlOffset = 5000
};

enum file_type {
	kBE_FileTypeAll = 0,
	kBE_FileTypeFile = 1,
	kBE_FileTypeFolder = 2
};

enum data_type {
	kBE_DataType_Not_Found = -1, 
	kBE_DataType_String = 0,
	kBE_DataType_Long = 1
};

enum input_output {
    kBufferSize = 16 * 1024 // 16 * 1024 16384
} ;

enum timeout {
	kBE_Never = -1,
	kBE_Immediate = 0
};


#endif // BEPLUGINGLOBALDEFINES_H
