/*
 BEPlugin.cpp
 BaseElements Plug-in

 Copyright 2010-2018 Goya. All rights reserved.
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
#include "BEXSLT.h"
#include "Crypto/BEMessageDigest.h"

#ifdef FMX_MAC_TARGET
	#include "apple/BEMacFunctions.h"
#elif FMX_IOS_TARGET
	#include "apple/BEIOSFunctions.h"
#elif FMX_WIN_TARGET
	#include "win/BEWinFunctions.h"
#elif FMX_LINUX_TARGET
	#include "linux/BELinuxFunctions.h"
#endif


using namespace std;
using namespace fmx;


// the "main" file for the plug-in

#pragma mark -
#pragma mark Globals
#pragma mark -

FMX_ExternCallPtr gFMX_ExternCallPtr;				// required by the FMX API
BEFileMakerPlugin * g_be_plugin;

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
	InitialiseLibXSLT();


	QuadCharUniquePtr plugin_id ( PLUGIN_ID );
	g_be_plugin = new BEFileMakerPlugin ( *plugin_id );
	g_be_plugin->set_fmx_application ( plugin_call->parm1 );

	g_be_plugin->RegisterFunction ( kBE_Version, BE_Version );
	g_be_plugin->RegisterFunction ( kBE_VersionAutoUpdate, BE_VersionAutoUpdate );

	g_be_plugin->RegisterFunction ( kBE_GetLastError, BE_GetLastError );
	g_be_plugin->RegisterFunction ( kBE_GetLastDDLError, BE_GetLastError );

#if ( FMX_MAC_TARGET || FMX_IOS_TARGET || FMX_WIN_TARGET )
	g_be_plugin->RegisterFunction ( kBE_ClipboardGetText, BE_ClipboardGetText, 1 );
	g_be_plugin->RegisterFunction ( kBE_ClipboardSetText, BE_ClipboardSetText, 2 );
	g_be_plugin->RegisterFunction ( kBE_OpenURL, BE_OpenURL, 1 );
	g_be_plugin->RegisterFunction ( kBE_PreferenceSet, BE_PreferenceSet, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_PreferenceGet, BE_PreferenceGet, 1, 2 );
#else
	g_be_plugin->RegisterHiddenFunction ( kBE_ClipboardGetText, BE_NotImplemented, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_ClipboardSetText, BE_NotImplemented, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_OpenURL, BE_NotImplemented, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_PreferenceSet, BE_NotImplemented, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_PreferenceGet, BE_NotImplemented, 1, 2 );
#endif
	
#if ( FMX_MAC_TARGET || FMX_WIN_TARGET )
	g_be_plugin->RegisterFunction ( kBE_ClipboardFormats, BE_ClipboardFormats );
	g_be_plugin->RegisterFunction ( kBE_FileSelect, BE_FileSelect, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_FolderSelect, BE_FolderSelect, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_FileSaveDialog, BE_FileSaveDialog, 1, 3 );
	g_be_plugin->RegisterFunction ( kBE_DialogDisplay, BE_DialogDisplay, 3, 5 );
	g_be_plugin->RegisterFunction ( kBE_FileOpen, BE_FileOpen, 1, 1 );
	g_be_plugin->RegisterFunction ( kBE_DialogProgress, BE_DialogProgress, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_DialogProgressUpdate, BE_DialogProgressUpdate, 1, 2 );
#else
	g_be_plugin->RegisterHiddenFunction ( kBE_ClipboardFormats, BE_NotImplemented );
	g_be_plugin->RegisterHiddenFunction ( kBE_FileSelect, BE_NotImplemented, 1, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FolderSelect, BE_NotImplemented, 1, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FileSaveDialog, BE_NotImplemented, 1, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_DialogDisplay, BE_NotImplemented, 3, 5 );
	g_be_plugin->RegisterHiddenFunction ( kBE_FileOpen, BE_NotImplemented, 1, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_DialogProgress, BE_NotImplemented, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_DialogProgressUpdate, BE_NotImplemented, 1, 2 );
#endif

	g_be_plugin->RegisterFunction ( kBE_FolderCreate, BE_FolderCreate, 1 );
	g_be_plugin->RegisterFunction ( kBE_FileDelete, BE_FileDelete, 1 );
	g_be_plugin->RegisterFunction ( kBE_FileExists, BE_FileExists, 1 );
	g_be_plugin->RegisterFunction ( kBE_FileSize, BE_FileSize, 1 );
    g_be_plugin->RegisterFunction ( kBE_FileModificationTimestamp, BE_FileModificationTimestamp, 1 );
	g_be_plugin->RegisterFunction ( kBE_FileReadText, BE_FileReadText, 1 );
	g_be_plugin->RegisterFunction ( kBE_FileWriteText, BE_FileWriteText, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_WriteTextFileToContainer_Deprecated, BE_WriteTextFileToContainer_Deprecated, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_StripInvalidUTF16CharactersFromXMLFile, BE_StripInvalidUTF16CharactersFromXMLFile, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ExportFieldContents, BE_ExportFieldContents, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_FileImport, BE_FileImport, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_FileMove, BE_FileMove, 2 );
	g_be_plugin->RegisterFunction ( kBE_FileCopy, BE_FileCopy, 2 );
	g_be_plugin->RegisterFunction ( kBE_FileListFolder, BE_FileListFolder, 1, 5 );

	g_be_plugin->RegisterFunction ( kBE_FileTypeAll + kBE_FileTypeOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_FileTypeFile + kBE_FileTypeOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_FileTypeFolder + kBE_FileTypeOffset, BE_NumericConstants );

	g_be_plugin->RegisterFunction ( kBE_XSLTApply, BE_XSLTApply, 3 );

	g_be_plugin->RegisterFunction ( kBE_XSLT_ApplyInMemory, BE_XSLT_ApplyInMemory, 2 );
	g_be_plugin->RegisterFunction ( kBE_XPath, BE_XPath, 2, 4 );
	g_be_plugin->RegisterFunction ( kBE_XPathAll, BE_XPathAll, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_XMLStripNodes, BE_XMLStripNodes, 3 );
	g_be_plugin->RegisterFunction ( kBE_XMLParse, BE_XMLParse, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_SplitBEFileNodes, BE_SplitBEFileNodes, 1 );
	g_be_plugin->RegisterFunction ( kBE_XMLValidate, BE_XMLValidate, 2 );


	g_be_plugin->RegisterHiddenFunction ( kBE_ExtractScriptVariables, BE_ExtractScriptVariables, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_OKButton + kBE_ButtonOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_CancelButton + kBE_ButtonOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_AlternateButton + kBE_ButtonOffset, BE_NumericConstants );

#if ( FMX_MAC_TARGET || FMX_WIN_TARGET || FMX_LINUX_TARGET )
	g_be_plugin->RegisterFunction ( kBE_ExecuteSystemCommand, BE_ExecuteSystemCommand, 1, 2 );
#else
	g_be_plugin->RegisterHiddenFunction ( kBE_ExecuteSystemCommand, BE_NotImplemented, 1, 2 );
#endif
	
	g_be_plugin->RegisterFunction ( kBE_ScriptExecute, BE_ScriptExecute, 1, 3 );
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

	g_be_plugin->RegisterFunction ( kBE_Unzip, BE_Unzip, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_Zip, BE_Zip, 1, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_Base64_Decode_Deprecated, BE_Base64_Decode_Deprecated, 1, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_Base64_Encode_Deprecated, BE_Base64_Encode_Deprecated, 1 );
	g_be_plugin->RegisterFunction ( kBE_SetTextEncoding, BE_SetTextEncoding, 0, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_Base64_URL_Encode_Deprecated, BE_Base64_Encode_Deprecated, 1 );


	g_be_plugin->RegisterHiddenFunction ( kBE_JPEG_Recompress_Deprecated, BE_JPEGRecompress, 1, 4 ); // deprecated
	g_be_plugin->RegisterFunction ( kBE_JPEGRecompress, BE_JPEGRecompress, 1, 3 );


	g_be_plugin->RegisterHiddenFunction ( kBE_Encrypt_AES_Deprecated, BE_Encrypt_AES_Deprecated, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_Decrypt_AES_Deprecated, BE_Decrypt_AES_Deprecated, 2 );
	g_be_plugin->RegisterFunction ( kBE_SignatureGenerateRSA, BE_SignatureGenerateRSA, 2, 5 );
	g_be_plugin->RegisterFunction ( kBE_SignatureVerifyRSA, BE_SignatureVerifyRSA, 3, 4 );
	g_be_plugin->RegisterFunction ( kBE_CipherEncrypt, BE_CipherEncrypt, 3, 6 );
	g_be_plugin->RegisterFunction ( kBE_CipherDecrypt, BE_CipherDecrypt, 3, 6 );

	g_be_plugin->RegisterFunction ( kBE_HTTP_POST, BE_HTTP_POST_PUT_PATCH, 2, 5 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_DELETE, BE_HTTP_DELETE, 1, 3 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_PUTFile, BE_HTTP_POST_PUT_PATCH, 2, 4 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_PUTData, BE_HTTP_POST_PUT_PATCH, 2, 4 );

	g_be_plugin->RegisterFunction ( kBE_HTTP_ResponseCode, BE_HTTP_ResponseCode );
	g_be_plugin->RegisterFunction ( kBE_HTTP_ResponseHeaders, BE_HTTP_ResponseHeaders, 0, 1 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_SetCustomHeader, BE_Net_Set_Header, 0, 2 );
	g_be_plugin->RegisterFunction ( kBE_HTTP_SetProxy, BE_HTTP_SetProxy, 1, 4 );
	g_be_plugin->RegisterFunction ( kBE_CurlSetOption, BE_CurlSetOption, 0, 2 );
	g_be_plugin->RegisterFunction ( kBE_CurlTrace, BE_CurlTrace );
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

	g_be_plugin->RegisterFunction ( kBE_TimeCurrentMilliseconds, BE_TimeFunctions );
	g_be_plugin->RegisterFunction ( kBE_TimeUTCMilliseconds, BE_TimeFunctions );
	g_be_plugin->RegisterFunction ( kBE_TimeZoneOffset, BE_TimeFunctions );


	g_be_plugin->RegisterFunction ( kBE_ValuesUnique, BE_ValuesUnique, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ValuesFilterOut, BE_ValuesFilterOut, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_ValuesContainsDuplicates, BE_ValuesContainsDuplicates, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ValuesSort, BE_ValuesSort, 1, 3 );
	g_be_plugin->RegisterFunction ( kBE_ValuesTimesDuplicated, BE_ValuesTimesDuplicated, 2 );
	g_be_plugin->RegisterFunction ( kBE_ValuesTrim, BE_ValuesTrim, 1 );


	g_be_plugin->RegisterFunction ( kBE_FTP_Upload, BE_FTP_Upload, 2, 4 );

	g_be_plugin->RegisterFunction ( kBE_SMTPServer, BE_SMTPServer, 1, 4 );
	g_be_plugin->RegisterFunction ( kBE_SMTPSend, BE_SMTPSend, 4, 9 );
	g_be_plugin->RegisterFunction ( kBE_SMTPAddAttachment, BE_SMTPAddAttachment, 0, 2 );
	g_be_plugin->RegisterFunction ( kBE_SMTPSetHeader, BE_Net_Set_Header, 0, 2 );


	g_be_plugin->RegisterFunction ( kBE_XeroSetTokens, BE_XeroSetTokens, 2 );
	g_be_plugin->RegisterFunction ( kBE_XeroGenerateKeys, BE_XeroGenerateKeys, 1, 7 );

	g_be_plugin->RegisterFunction ( kBE_EvaluateJavaScript, BE_EvaluateJavaScript, 1 );


	g_be_plugin->RegisterFunction ( kBE_ArraySetFromValueList, BE_ArraySetFromValueList, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ArrayGetSize, BE_ArrayGetSize, 1 );
	g_be_plugin->RegisterFunction ( kBE_ArrayGetValue, BE_ArrayGetValue, 2 );
	g_be_plugin->RegisterFunction ( kBE_ArrayDelete, BE_ArrayDelete, 1 );
	g_be_plugin->RegisterFunction ( kBE_ArrayFind, BE_ArrayFind, 2 );
	g_be_plugin->RegisterFunction ( kBE_ArrayChangeValue, BE_ArrayChangeValue, 3 );


	g_be_plugin->RegisterFunction ( kBE_ConvertContainer, BE_ConvertContainer, 1, 4 );

	g_be_plugin->RegisterFunction ( kBE_PDFAppend, BE_PDFAppend, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_PDFPageCount, BE_PDFPageCount, 1 );
	g_be_plugin->RegisterFunction ( kBE_PDFGetPages, BE_PDFGetPages, 3, 4 );


	if ( (gFMX_ExternCallPtr->extnVersion >= k160ExtnVersion) ) { // crash in versions prior to 16

		g_be_plugin->RegisterFunction ( kBE_ScriptStepInstall, BE_ScriptStepInstall, 5 );
		g_be_plugin->RegisterFunction ( kBE_ScriptStepRemove, BE_ScriptStepRemove, 1 );
		g_be_plugin->RegisterFunction ( kBE_ScriptStepPerform, BE_ScriptStepPerform, 1 );

	} else {

		g_be_plugin->RegisterHiddenFunction ( kBE_ScriptStepInstall, BE_NotImplemented, 5 );
		g_be_plugin->RegisterHiddenFunction ( kBE_ScriptStepRemove, BE_NotImplemented, 1 );
		g_be_plugin->RegisterHiddenFunction ( kBE_ScriptStepPerform, BE_NotImplemented, 1 );

	}


	g_be_plugin->RegisterFunction ( kBE_XOR, BE_XOR, 2, 2 );

	g_be_plugin->RegisterFunction ( kBE_RegularExpression, BE_RegularExpression, 2, 4 );
	g_be_plugin->RegisterFunction ( kBE_Pause, BE_Pause, 1 );

	g_be_plugin->RegisterFunction ( kBE_VectorDotProduct, BE_VectorDotProduct, 2 );
	g_be_plugin->RegisterFunction ( kBE_Vector_EuclideanDistance, BE_Vector_EuclideanDistance, 2 );

	g_be_plugin->RegisterFunction ( kBE_GetMachineName, BE_GetMachineName );


	// still alpha

//	g_be_plugin->RegisterFunction ( kBE_OAuth_RequestToken, BE_OAuthRequestAccessToken, true, 3 );
//	g_be_plugin->RegisterFunction ( kBE_OAuthRequestAccessToken, BE_OAuthRequestAccessToken, true, 3, 5 );
	g_be_plugin->RegisterHiddenFunction ( kBE_OAuthRequestAccessToken, BE_OAuthRequestAccessToken, 3, 5 );
//	g_be_plugin->RegisterFunction ( kBE_OAuth_ClearToken, BE_OAuthRequestAccessToken, true );

	return kCurrentExtnVersion;	// enable the plug-in

} // LoadPlugin



static void UnloadPlugin ( void ) {

	CleanupLibXSLT();

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

