/*
 BEPluginGlobalDefines.h
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
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

#elif defined( _MSC_VER )

	#define FMX_WIN_TARGET	1
	#define _WIN32_WINNT _WIN32_WINNT_MAXVER

#endif


#include "FMWrapper/FMXExtern.h"

extern fmx::errcode g_last_error;



#define PLUGIN_NAME		"BaseElements"
#define PLUGIN_ID		'G', 'y', 'B', 'E'

#define	VERSION_NUMBER_STRING L"1.2.1"
#define	AUTO_UPDATE_VERSION L"00010201"
#define VERSION_STRING "1.2.1"

//#define VERSION_NAME_STRING	PLUGIN_NAME " (" VERSION_STRING ")"
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
	kBE_ExecuteShellCommand = 120,
	kBE_ListFilesInFolder = 121,
	kBE_FileMaker_Tables = 122,
	kBE_FileMaker_Fields = 123,
	kBE_OpenURL = 124,
	kBE_ExecuteScript = 150,
	kBE_FileMakerSQL = 151,
	kBE_GetURL = 152,
	kBE_GetLastError = 153,
	kBE_MessageDigest = 154,
	kBE_SaveURLToFile = 156,
	kBE_ApplyXSLTInMemory = 200,
	kBE_XPath = 201,
	kBE_XPathAll = 202
};


enum dialog_buttons {
	kBE_OKButton = 1,
	kBE_CancelButton = 2,
	kBE_AlternateButton = 3
};


enum errors {
	kErrorUnknown = -1,
	kNoError = 0,
	kLowMemoryError = 7,
	kErrorParameterMissing = 102,
	kFileSystemError = 10000
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


#endif // BEPLUGINGLOBALDEFINES_H
