/*
 BEPlugin.cpp
 BaseElements Plug-in
  
 Copyright 2010-2011 Goya. All rights reserved.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without 
 modification, are permitted provided that the following conditions 
 are met:
 
 1. Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
 3. Neither the name of Growl nor the names of its contributors
	may be used to endorse or promote products derived from this software
	without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 http://www.goya.com.au/baseelements/plugin
 
 */


#if defined(FMX_WIN_TARGET)
	#include "afxwin.h"
#endif

#include "BEPluginGlobalDefines.h"
#include "BEPluginFunctions.h"
#include "BEPluginUtilities.h"
#include "BEFileMakerPlugin.h"
#include "BESQLCommand.h"

#include "FMWrapper/FMXData.h"

#include "BESQLCommand.h"

using namespace fmx;

// the "main" file for the plug-in

#pragma mark -
#pragma mark Globals
#pragma mark -

FMX_ExternCallPtr gFMX_ExternCallPtr;	// required by the FMX API
BEFileMakerPlugin * g_be_plugin;		// the plug-in instance

extern BESQLCommandAutoPtr g_ddl_command;


#pragma mark -
#pragma mark Functions
#pragma mark -

// register the plug-in functions
// the functions are defined in BEPluginFunctions.cpp

static FMX_Long LoadPlugin ( FMX_ExternCallPtr plugin_call ) 
{
#pragma unused ( plugin_call )
	
	SetTextEncoding ( "" );
	
	QuadCharAutoPtr plugin_id ( PLUGIN_ID );
	g_be_plugin = new BEFileMakerPlugin ( plugin_id );

	g_be_plugin->RegisterFunction ( kBE_Version, BE_Version );
	g_be_plugin->RegisterFunction ( kBE_VersionAutoUpdate, BE_VersionAutoUpdate );

	g_be_plugin->RegisterFunction ( kBE_GetLastError, BE_GetLastError );
	g_be_plugin->RegisterFunction ( kBE_GetLastDDLError, BE_GetLastError );

	g_be_plugin->RegisterFunction ( kBE_ClipboardFormats, BE_ClipboardFormats, false );
	g_be_plugin->RegisterFunction ( kBE_ClipboardData, BE_ClipboardData, false, 1 );
	g_be_plugin->RegisterFunction ( kBE_SetClipboardData, BE_SetClipboardData, false, 2 );

	g_be_plugin->RegisterFunction ( kBE_CreateFolder, BE_CreateFolder, 1 );
	g_be_plugin->RegisterFunction ( kBE_DeleteFile, BE_DeleteFile, 1 );
	g_be_plugin->RegisterFunction ( kBE_FileExists, BE_FileExists, 1 );
	g_be_plugin->RegisterFunction ( kBE_ReadTextFromFile, BE_ReadTextFromFile, 1 );
	g_be_plugin->RegisterFunction ( kBE_WriteTextToFile, BE_WriteTextToFile, true, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_StripInvalidUTF16CharactersFromXMLFile, BE_StripInvalidUTF16CharactersFromXMLFile, 1 );

	g_be_plugin->RegisterFunction ( kBE_MoveFile, BE_MoveFile, 2 );
	g_be_plugin->RegisterFunction ( kBE_CopyFile, BE_CopyFile, 2 );
	g_be_plugin->RegisterFunction ( kBE_ListFilesInFolder, BE_ListFilesInFolder, true, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_FileType_ALL + kBE_FileTypeOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_FileType_File + kBE_FileTypeOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_FileType_Folder + kBE_FileTypeOffset, BE_NumericConstants );

	g_be_plugin->RegisterFunction ( kBE_SelectFile, BE_SelectFile, false, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_SelectFolder, BE_SelectFolder, false, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_DisplayDialog, BE_DisplayDialog, false, 3, 5 );
	g_be_plugin->RegisterFunction ( kBE_OpenFile, BE_OpenFile, false, 1, 1 );
	
	
	g_be_plugin->RegisterFunction ( kBE_ApplyXSLT, BE_ApplyXSLT, 3 );
	
	g_be_plugin->RegisterFunction ( kBE_ApplyXSLTInMemory, BE_ApplyXSLTInMemory, 2 );
	g_be_plugin->RegisterFunction ( kBE_XPath, BE_XPath, false, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_XPathAll, BE_XPathAll, false, 2, 3 );

	g_be_plugin->RegisterFunction ( kBE_ExtractScriptVariables, BE_ExtractScriptVariables, 1 );
	
	g_be_plugin->RegisterFunction ( kBE_OKButton + kBE_ButtonOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_CancelButton + kBE_ButtonOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_AlternateButton + kBE_ButtonOffset, BE_NumericConstants );
	
	g_be_plugin->RegisterFunction ( kBE_ExecuteShellCommand, BE_ExecuteShellCommand, false, 1, 2 );
	
	g_be_plugin->RegisterFunction ( kBE_FileMaker_Tables, BE_FileMaker_TablesOrFields );
	g_be_plugin->RegisterFunction ( kBE_FileMaker_Fields, BE_FileMaker_TablesOrFields );

	g_be_plugin->RegisterFunction ( kBE_OpenURL, BE_OpenURL, false, 1 );

	g_be_plugin->RegisterFunction ( kBE_ExecuteScript, BE_ExecuteScript, true, 1, 3 );
	
	g_be_plugin->RegisterFunction ( kBE_FileMakerSQL, BE_FileMakerSQL, true, 1, 4 );

	g_be_plugin->RegisterFunction ( kBE_GetURL, BE_GetURL, true, 1, 4 );
	g_be_plugin->RegisterFunction ( kBE_SaveURLToFile, BE_SaveURLToFile, true, 2, 4 );

	g_be_plugin->RegisterFunction ( kBE_SetPreference, BE_SetPreference, true, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_GetPreference, BE_GetPreference, true, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_Unzip, BE_Unzip, 1 );
	g_be_plugin->RegisterFunction ( kBE_Zip, BE_Zip, 1 );
	g_be_plugin->RegisterFunction ( kBE_Base64_Decode, BE_Base64_Decode, 2 );
	g_be_plugin->RegisterFunction ( kBE_Base64_Encode, BE_Base64_Encode, 1 );
	g_be_plugin->RegisterFunction ( kBE_SetTextEncoding, BE_SetTextEncoding, true, 0, 1 );

	
	g_be_plugin->RegisterFunction ( kBE_HTTP_POST, BE_HTTP_POST_OR_PUT, true, 2, 4 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_DELETE, BE_HTTP_DELETE, true, 1, 3 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_PUT, BE_HTTP_POST_OR_PUT, true, 2, 4 );

	g_be_plugin->RegisterFunction ( kBE_HTTP_Response_Code, BE_HTTP_Response_Code );
	g_be_plugin->RegisterFunction ( kBE_HTTP_Response_Headers, BE_HTTP_Response_Headers );
	g_be_plugin->RegisterFunction ( kBE_HTTP_Set_Custom_Header, BE_HTTP_Set_Custom_Header, 2 );

	g_be_plugin->RegisterFunction ( kBE_MessageDigest, BE_MessageDigest, true, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestType_MD5 + kBE_MessageDigestTypeOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestType_SHA256 + kBE_MessageDigestTypeOffset, BE_NumericConstants );

	return kCurrentExtnVersion;	// enable the plug-in
	
} // LoadPlugin



static void UnloadPlugin ( void ) {

	delete g_be_plugin;	// un-register the plugin functions	

}


#include <iostream>

// main entry point
// calls for plug-in functions go direct to the registered function

void FMX_ENTRYPT FMExternCallProc ( FMX_ExternCallPtr plugin_call ) {

	gFMX_ExternCallPtr = plugin_call;

	switch ( plugin_call->whichCall ) 
	{
		case kFMXT_GetString:
			Do_GetString (	plugin_call->parm1, 
							plugin_call->parm2,
							plugin_call->parm3, 
							reinterpret_cast<FMX_Unichar*> ( plugin_call->result )
						  );
			break;

		case kFMXT_Init:
			plugin_call->result = LoadPlugin ( plugin_call );
			break;
			
		case kFMXT_Idle:
		{
			bool safe_idle = plugin_call->parm1 != kFMXT_Unsafe;
			
			if ( safe_idle && g_ddl_command.get() != 0 ) {
				
				g_ddl_command->execute ( );
				g_ddl_command.reset();
			
			}
			
			break;
		}
			
		case kFMXT_Shutdown:
			UnloadPlugin();
			break;
	}

} // FMExternCallProc
	
