/*
 BEPlugin.cpp
 BaseElements Plug-in

 Copyright 2010-2017 Goya. All rights reserved.
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


#include "BEPluginGlobalDefines.h"
#include "BEPluginFunctions.h"
#include "BEPluginUtilities.h"
#include "BEFileMakerPlugin.h"
#include "BESQLCommand.h"
#include "Crypto/BEMessageDigest.h"

#ifdef FMX_MAC_TARGET
	#include "BEMacFunctions.h"
#elif FMX_IOS_TARGET
	#include "BEIOSFunctions.h"
#elif FMX_WIN_TARGET
	#include "BEMacFunctions.h"
#elif FMX_LINUX_TARGET
	#include "BELinuxFunctions.h"
#endif

using namespace std;
using namespace fmx;


// the "main" file for the plug-in

#pragma mark -
#pragma mark Globals
#pragma mark -

FMX_ExternCallPtr gFMX_ExternCallPtr;	// required by the FMX API
BEFileMakerPlugin * g_be_plugin;		// the plug-in instance

extern BESQLCommandUniquePtr g_ddl_command;


#pragma mark -
#pragma mark Functions
#pragma mark -

// register the plug-in functions
// the functions are defined in BEPluginFunctions.cpp

static FMX_Int32 LoadPlugin ( FMX_ExternCallPtr plugin_call )
{
#pragma unused ( plugin_call )

	SetTextEncoding ( );

	InitialiseForPlatform ( );


	QuadCharUniquePtr plugin_id ( PLUGIN_ID );
	g_be_plugin = new BEFileMakerPlugin ( *plugin_id );
	g_be_plugin->set_fmx_application ( plugin_call->parm1 );

	g_be_plugin->RegisterFunction ( kBE_Version, BE_Version );
	g_be_plugin->RegisterFunction ( kBE_VersionAutoUpdate, BE_VersionAutoUpdate );

	g_be_plugin->RegisterFunction ( kBE_GetLastError, BE_GetLastError );
	g_be_plugin->RegisterFunction ( kBE_GetLastDDLError, BE_GetLastError );

	g_be_plugin->RegisterFunction ( kBE_ClipboardFormats, BE_ClipboardFormats );
	g_be_plugin->RegisterFunction ( kBE_ClipboardText, BE_ClipboardData, 1 );
	g_be_plugin->RegisterFunction ( kBE_SetClipboardText, BE_SetClipboardData, 2 );

	g_be_plugin->RegisterFunction ( kBE_CreateFolder, BE_CreateFolder, 1 );
	g_be_plugin->RegisterFunction ( kBE_DeleteFile, BE_DeleteFile, 1 );
	g_be_plugin->RegisterFunction ( kBE_FileExists, BE_FileExists, 1 );
	g_be_plugin->RegisterFunction ( kBE_FileSize, BE_FileSize, 1 );
    g_be_plugin->RegisterFunction ( kBE_File_Modification_Timestamp, BE_File_Modification_Timestamp, 1 );
	g_be_plugin->RegisterFunction ( kBE_ReadTextFromFile, BE_ReadTextFromFile, 1 );
	g_be_plugin->RegisterFunction ( kBE_WriteTextToFile, BE_WriteTextToFile, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_StripInvalidUTF16CharactersFromXMLFile, BE_StripInvalidUTF16CharactersFromXMLFile, 1 );
	g_be_plugin->RegisterFunction ( kBE_ExportFieldContents, BE_ExportFieldContents, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ImportFile, BE_ImportFile, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_MoveFile, BE_MoveFile, 2 );
	g_be_plugin->RegisterFunction ( kBE_CopyFile, BE_CopyFile, 2 );
	g_be_plugin->RegisterFunction ( kBE_ListFilesInFolder, BE_ListFilesInFolder, 1, 4 );

	g_be_plugin->RegisterFunction ( kBE_FileType_All + kBE_FileTypeOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_FileType_File + kBE_FileTypeOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_FileType_Folder + kBE_FileTypeOffset, BE_NumericConstants );

	g_be_plugin->RegisterFunction ( kBE_SelectFile, BE_SelectFile, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_SelectFolder, BE_SelectFolder, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_SaveFileDialog, BE_SaveFileDialog, 1, 3 );

	g_be_plugin->RegisterFunction ( kBE_DisplayDialog, BE_DisplayDialog, 3, 5 );
	g_be_plugin->RegisterFunction ( kBE_OpenFile, BE_OpenFile, 1, 1 );
	g_be_plugin->RegisterFunction ( kBE_ProgressDialog, BE_ProgressDialog, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_ProgressDialog_Update, BE_ProgressDialog_Update, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_Pause, BE_Pause, 1 );


	g_be_plugin->RegisterFunction ( kBE_ApplyXSLT, BE_ApplyXSLT, 3 );

	g_be_plugin->RegisterFunction ( kBE_ApplyXSLTInMemory, BE_ApplyXSLTInMemory, 2 );
	g_be_plugin->RegisterFunction ( kBE_XPath, BE_XPath, 2, 4 );
	g_be_plugin->RegisterFunction ( kBE_XPathAll, BE_XPathAll, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_StripXMLNodes, BE_StripXMLNodes, 3 );
	g_be_plugin->RegisterFunction ( kBE_XML_Parse, BE_XML_Parse, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_SplitBEFileNodes, BE_SplitBEFileNodes, 1 );
	g_be_plugin->RegisterFunction ( kBE_XML_Validate, BE_XML_Validate, 2 );


	g_be_plugin->RegisterHiddenFunction ( kBE_ExtractScriptVariables, BE_ExtractScriptVariables, 1 );

	g_be_plugin->RegisterFunction ( kBE_OKButton + kBE_ButtonOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_CancelButton + kBE_ButtonOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_AlternateButton + kBE_ButtonOffset, BE_NumericConstants );

	g_be_plugin->RegisterFunction ( kBE_ExecuteSystemCommand, BE_ExecuteSystemCommand, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_OpenURL, BE_OpenURL, 1 );

	g_be_plugin->RegisterFunction ( kBE_ExecuteScript, BE_ExecuteScript, 1, 3 );

	g_be_plugin->RegisterFunction ( kBE_FileMakerSQL, BE_FileMakerSQL, 1, 4 );

	g_be_plugin->RegisterFunction ( kBE_ContainerIsCompressed, BE_ContainerIsCompressed, 1 );
	g_be_plugin->RegisterFunction ( kBE_ContainerCompress, BE_ContainerCompress, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ContainerUncompress, BE_ContainerUncompress, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_Gzip, BE_Gzip, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_UnGzip, BE_UnGzip, 1, 2 );

	g_be_plugin->RegisterHiddenFunction ( kBE_JSONPath_Deprecated, BE_JSONPath_Deprecated, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_JSON_Error_Description_Deprecated, BE_JSON_Error_Description_Deprecated );
	g_be_plugin->RegisterHiddenFunction ( kBE_JSON_ArraySize_Deprecated, BE_JSON_ArraySize_Deprecated, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_JSON_Encode_Deprecated, BE_JSON_Encode_Deprecated, 1, 3 );


	g_be_plugin->RegisterFunction ( kBE_HTTP_GET_File, BE_HTTP_GET, 1, 4 );
	g_be_plugin->RegisterFunction ( kBE_SaveURLToFile, BE_HTTP_GET_File, 2, 4 );

	g_be_plugin->RegisterFunction ( kBE_SetPreference, BE_SetPreference, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_GetPreference, BE_GetPreference, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_Unzip, BE_Unzip, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_Zip, BE_Zip, 1, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_Base64_Decode_Deprecated, BE_Base64_Decode_Deprecated, 1, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_Base64_Encode_Deprecated, BE_Base64_Encode_Deprecated, 1 );
	g_be_plugin->RegisterFunction ( kBE_SetTextEncoding, BE_SetTextEncoding, 0, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_Base64_URL_Encode_Deprecated, BE_Base64_Encode_Deprecated, 1 );


	g_be_plugin->RegisterHiddenFunction ( kBE_JPEG_Recompress_Deprecated, BE_JPEG_Recompress, 1, 4 ); // deprecated
	g_be_plugin->RegisterFunction ( kBE_JPEG_Recompress, BE_JPEG_Recompress, 1, 3 );


	g_be_plugin->RegisterHiddenFunction ( kBE_Encrypt_AES_Deprecated, BE_Encrypt_AES_Deprecated, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_Decrypt_AES_Deprecated, BE_Decrypt_AES_Deprecated, 2 );


	g_be_plugin->RegisterFunction ( kBE_HTTP_POST, BE_HTTP_POST_PUT_PATCH, 2, 5 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_DELETE, BE_HTTP_DELETE, 1, 3 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_PUT_File, BE_HTTP_POST_PUT_PATCH, 2, 4 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_PUT_Data, BE_HTTP_POST_PUT_PATCH, 2, 4 );

	g_be_plugin->RegisterFunction ( kBE_HTTP_Response_Code, BE_HTTP_Response_Code );
	g_be_plugin->RegisterFunction ( kBE_HTTP_Response_Headers, BE_HTTP_Response_Headers, 0, 1 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_Set_Custom_Header, BE_Net_Set_Header, 2 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_Set_Proxy, BE_HTTP_Set_Proxy, 1, 4 );
	g_be_plugin->RegisterFunction ( kBE_Curl_Set_Option, BE_Curl_Set_Option, 0, 2 );
	g_be_plugin->RegisterFunction ( kBE_Curl_Trace, BE_Curl_Trace );
	g_be_plugin->RegisterFunction ( kBE_FTP_Delete, BE_FTP_Delete, 1, 3 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_PATCH, BE_HTTP_POST_PUT_PATCH, 2, 4 );


	g_be_plugin->RegisterHiddenFunction ( kBE_MessageDigest_Deprecated, BE_MessageDigest_Deprecated, 1, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_MessageDigestAlgorithm_MD5 + kBE_MessageDigestAlgorithmOffset_Deprecated, BE_NumericConstants );
	g_be_plugin->RegisterHiddenFunction ( kBE_MessageDigestAlgorithm_SHA256 + kBE_MessageDigestAlgorithmOffset_Deprecated, BE_NumericConstants );
	g_be_plugin->RegisterHiddenFunction ( kBE_MessageDigestAlgorithm_MDC2 + kBE_MessageDigestAlgorithmOffset_Deprecated, BE_NumericConstants );
	g_be_plugin->RegisterHiddenFunction ( kBE_MessageDigestAlgorithm_SHA + kBE_MessageDigestAlgorithmOffset_Deprecated, BE_NumericConstants );
	g_be_plugin->RegisterHiddenFunction ( kBE_MessageDigestAlgorithm_SHA1 + kBE_MessageDigestAlgorithmOffset_Deprecated, BE_NumericConstants );
	g_be_plugin->RegisterHiddenFunction ( kBE_MessageDigestAlgorithm_SHA224 + kBE_MessageDigestAlgorithmOffset_Deprecated, BE_NumericConstants );
	g_be_plugin->RegisterHiddenFunction ( kBE_MessageDigestAlgorithm_SHA384 + kBE_MessageDigestAlgorithmOffset_Deprecated, BE_NumericConstants );
	g_be_plugin->RegisterHiddenFunction ( kBE_MessageDigestAlgorithm_SHA512 + kBE_MessageDigestAlgorithmOffset_Deprecated, BE_NumericConstants );
	g_be_plugin->RegisterHiddenFunction ( kBE_HMAC_Deprecated, BE_HMAC_Deprecated, 2, 5 );
	
	g_be_plugin->RegisterHiddenFunction ( kBE_Encoding_Hex + kBE_EncodingOffset, BE_NumericConstants );
	g_be_plugin->RegisterHiddenFunction ( kBE_Encoding_Base64 + kBE_EncodingOffset, BE_NumericConstants );

	g_be_plugin->RegisterFunction ( kBE_CurrentTimeMilliseconds, BE_TimeFunctions );
	g_be_plugin->RegisterFunction ( kBE_UTCMilliseconds, BE_TimeFunctions );
	g_be_plugin->RegisterFunction ( kBE_TimeZoneOffset, BE_TimeFunctions );


	g_be_plugin->RegisterFunction ( kBE_Values_Unique, BE_Values_Unique, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_Values_FilterOut, BE_Values_FilterOut, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_Values_ContainsDuplicates, BE_Values_ContainsDuplicates, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_Values_Sort, BE_Values_Sort, 1, 3 );
	g_be_plugin->RegisterFunction ( kBE_Values_TimesDuplicated, BE_Values_TimesDuplicated, 2 );
	g_be_plugin->RegisterFunction ( kBE_Values_Trim, BE_Values_Trim, 1 );


	g_be_plugin->RegisterFunction ( kBE_FTP_Upload, BE_FTP_Upload, 2, 4 );

	g_be_plugin->RegisterFunction ( kBE_SMTP_Server, BE_SMTP_Server, 1, 4 );
	g_be_plugin->RegisterFunction ( kBE_SMTP_Send, BE_SMTP_Send, 4, 9 );
	g_be_plugin->RegisterFunction ( kBE_SMTP_AddAttachment, BE_SMTP_AddAttachment, 0, 2 );
	g_be_plugin->RegisterFunction ( kBE_SMTP_Set_Header, BE_Net_Set_Header, 2 );


	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_Upload_Database, BE_ExportFieldContents, 2, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_Download_Database, BE_ImportFile, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_List_Files, BE_FMS_Command, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_Pause_Files, BE_FMS_Command, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_Run_Schedule, BE_FMS_Command, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_List_Schedules, BE_FMS_Command, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_List_Clients, BE_FMS_Command, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_Resume_Files, BE_FMS_Command, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_Close_Files, BE_FMS_Command, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_Remove_Files, BE_FMS_Command, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FMS_Open_Files, BE_FMS_Command, 2, 3 );


	g_be_plugin->RegisterFunction ( kBE_Xero_SetTokens, BE_Xero_SetTokens, 2 );
	g_be_plugin->RegisterFunction ( kBE_Xero_GenerateKeys, BE_Xero_GenerateKeys, 1, 7 );

	g_be_plugin->RegisterFunction ( kBE_EvaluateJavaScript, BE_EvaluateJavaScript, 1 );


	g_be_plugin->RegisterFunction ( kBE_ArraySetFromValueList, BE_ArraySetFromValueList, 1 );
	g_be_plugin->RegisterFunction ( kBE_ArrayGetSize, BE_ArrayGetSize, 1 );
	g_be_plugin->RegisterFunction ( kBE_ArrayGetValue, BE_ArrayGetValue, 2 );
	g_be_plugin->RegisterFunction ( kBE_Array_Delete, BE_Array_Delete, 1 );
	g_be_plugin->RegisterFunction ( kBE_Array_Find, BE_Array_Find, 2 );
	g_be_plugin->RegisterFunction ( kBE_Array_Change_Value, BE_Array_Change_Value, 3 );


	g_be_plugin->RegisterFunction ( kBE_ConvertContainer, BE_ConvertContainer, 1, 4 );

	g_be_plugin->RegisterFunction ( kBE_PDF_Append, BE_PDF_Append, 2, 3 );

	g_be_plugin->RegisterFunction ( kBE_XOR, BE_XOR, 2, 2 );

	g_be_plugin->RegisterFunction ( kBE_RegularExpression, BE_RegularExpression, 2, 4 );

	g_be_plugin->RegisterFunction ( kBE_Vector_DotProduct, BE_Vector_DotProduct, 2 );
	g_be_plugin->RegisterFunction ( kBE_Vector_EuclideanDistance, BE_Vector_EuclideanDistance, 2 );

	g_be_plugin->RegisterFunction ( kBE_Get_Machine_Name, BE_Get_Machine_Name );


	// still alpha

//	g_be_plugin->RegisterFunction ( kBE_OAuth_RequestToken, BE_OAuth_RequestAccessToken, true, 3 );
//	g_be_plugin->RegisterFunction ( kBE_OAuth_RequestAccessToken, BE_OAuth_RequestAccessToken, true, 3, 5 );
	g_be_plugin->RegisterHiddenFunction ( kBE_OAuth_RequestAccessToken, BE_OAuth_RequestAccessToken, 3, 5 );
//	g_be_plugin->RegisterFunction ( kBE_OAuth_ClearToken, BE_OAuth_RequestAccessToken, true );

	return kCurrentExtnVersion;	// enable the plug-in

} // LoadPlugin



static void UnloadPlugin ( void ) {

	delete g_be_plugin;	// un-register the plugin functions

}


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

