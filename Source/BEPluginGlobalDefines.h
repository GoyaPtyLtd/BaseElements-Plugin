/*
 BEPluginGlobalDefines.h
 BaseElements Plug-In
 
 Copyright 2010 Goya. All rights reserved.
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


#define PLUGIN_NAME		"BaseElements"
#define PLUGIN_ID		'G', 'y', 'B', 'E'

#define	VERSION_NUMBER_STRING "1.0.0"
#define	AUTO_UPDATE_VERSION "00010000"


enum { 
	kXMpl_OptionsStringID = 1
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
	kBE_StripInvalidUTF16CharactersFromXMLFile = 117
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
	kXMpl_ButtonOffset = 1000,
	kXMpl_GetStringMaxBufferSize = 4096
};


#endif // BEPLUGINGLOBALDEFINES_H
