/*
 BEPluginGlobalDefines.h
 BaseElements Plug-In
 
 Copyright 2010-2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEPLUGINGLOBALDEFINES_H)
	#define BEPLUGINGLOBALDEFINES_H


#if defined( __GNUC__ )

	#define FMX_MAC_TARGET	1

	/*
	 Unloadable modules can not use the new style constant strings that Apple introduced
	 in 10.4. So we have to turn if off in the headers and the project file with
	 */
	#undef __CONSTANT_CFSTRINGS__

	#define USER_PREFERENCES_DOMAIN L"au.com.goya.baseelements.plugin-user"

#elif defined( _MSC_VER )

	#define FMX_WIN_TARGET	1
	#define _WIN32_WINNT _WIN32_WINNT_MAXVER
	#define USER_PREFERENCES_DOMAIN L"Software\\Goya\\BaseElements\\PluginUser"

	#define PATH_MAX MAX_PATH

#endif


#define FILEMAKER_EPOCH_OFFSET 62135596800

#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXText.h"

extern fmx::errcode g_last_error;


#define PLUGIN_NAME		"BaseElements"
#define PLUGIN_ID		'G', 'y', 'B', 'E'

#define	VERSION_NUMBER_STRING L"1.9.5"
#define	AUTO_UPDATE_VERSION L"00010905"
#define VERSION_STRING "1.9.5"


#define USER_AGENT_STRING "libcurl-agent-base-elements-plugin/"VERSION_STRING


#define	TEMPORARY_FILE_SUFFIX L".be3.tmp"


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
	kBE_ExecuteShellCommand = 120, // depreciated
	kBE_ListFilesInFolder = 121,
	kBE_FileMaker_Tables = 122, // depreciated
	kBE_FileMaker_Fields = 123, // depreciated
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
	kBE_SetPreference = 160,
	kBE_GetPreference = 161,
	kBE_Unzip = 170,
	kBE_Zip = 171,
	kBE_Base64_Decode = 172,
	kBE_Base64_Encode = 173,
	kBE_SetTextEncoding = 174,
	kBE_HTTP_POST = 180,
	kBE_HTTP_Response_Code = 181,
	kBE_HTTP_Response_Headers = 182,
	kBE_HTTP_Set_Custom_Header = 183,
	kBE_HTTP_DELETE = 184,
	kBE_HTTP_PUT = 185,
	kBE_HTTP_Set_Proxy = 186,
	kBE_ApplyXSLTInMemory = 200,
	kBE_XPath = 201,
	kBE_XPathAll = 202,
	kBE_StripXMLNodes = 210,
	kBE_CurrentTimeMilliseconds = 220,
	kBE_UTCMilliseconds = 221,
	kBE_TimeZoneOffset = 222,
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
	kNoSuchFileOrDirectoryError = 2,
	kLowMemoryError = 7,
	kRequestedDataIsMissingError = 10,
	kFileExistsError = 17,
	kErrorParameterMissing = 102,
	kFileSystemError = 10000,
	kDDLCommandQueued = 11000,
	kCommandTimeout = 12000
};


enum {
	kBE_NumericConstantOffset = 1000,
	kBE_ButtonOffset = 1000,
	kBE_MessageDigestTypeOffset = 2000,
	kBE_FileTypeOffset = 3000,
	kBE_GetStringMaxBufferSize = 4096
};


enum message_digest_type {
	kBE_MessageDigestType_MD5 = 1,
	kBE_MessageDigestType_SHA256 = 2
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
	kBE_Never = -1
};


#endif // BEPLUGINGLOBALDEFINES_H
