/*
 BEPlugin.cpp
 BaseElements Plug-in

 Copyright 2010-2023 Goya. All rights reserved.
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


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <Magick++.h>
#pragma GCC diagnostic pop


using namespace std;
using namespace fmx;


// the "main" file for the plug-in

#pragma mark -
#pragma mark Globals
#pragma mark -

BEP_EXPORT FMX_ExternCallPtr gFMX_ExternCallPtr; // required by the FMX API
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

	QuadCharUniquePtr plugin_id ( PLUGIN_ID );
	g_be_plugin = new BEFileMakerPlugin ( *plugin_id );
	g_be_plugin->set_fmx_application ( plugin_call->parm1 );

	InitialiseForPlatform ( );
	InitialiseLibXSLT();

#ifndef FMX_IOS_TARGET
    Magick::InitializeMagick ( NULL );
#endif

	fmx::TextUniquePtr fmx_application_version;
	fmx_application_version->AssignUnicode ( (unichar16 *)plugin_call->parm2 );
	const std::string application_version ( TextAsUTF8String ( *fmx_application_version ) );
	g_be_plugin->set_fmx_application_version ( application_version );

	g_be_plugin->RegisterFunction ( kBE_Version, BE_Version );
	g_be_plugin->RegisterFunction ( kBE_VersionAutoUpdate, BE_Version );
	g_be_plugin->RegisterFunction ( kBE_VersionPro, BE_Version );


	g_be_plugin->RegisterFunction ( kBE_GetLastError, BE_GetLastError );
	g_be_plugin->RegisterFunction ( kBE_GetLastDDLError, BE_GetLastError );

#if ( FMX_MAC_TARGET || FMX_IOS_TARGET || FMX_WIN_TARGET )
	g_be_plugin->RegisterFunction ( kBE_ClipboardGetText, BE_ClipboardGetText, 1 );
	g_be_plugin->RegisterFunction ( kBE_ClipboardSetText, BE_ClipboardSetText, 2 );
	g_be_plugin->RegisterFunction ( kBE_ClipboardGetFile, BE_ClipboardGetFile, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ClipboardSetFile, BE_ClipboardSetFile, 2 );
	g_be_plugin->RegisterFunction ( kBE_OpenURL, BE_OpenURL, 1 );
	g_be_plugin->RegisterFunction ( kBE_PreferenceSet, BE_PreferenceSet, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_PreferenceGet, BE_Preference, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_PreferenceDelete, BE_Preference, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ScriptExecute, BE_ScriptExecute, 1, 4 );
#else
	g_be_plugin->RegisterHiddenFunction ( kBE_ClipboardGetText, BE_NotImplemented, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_ClipboardSetText, BE_NotImplemented, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_ClipboardGetFile, BE_NotImplemented, 1, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_ClipboardSetFile, BE_NotImplemented, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_OpenURL, BE_NotImplemented, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_PreferenceSet, BE_NotImplemented, 2, 3 );
	g_be_plugin->RegisterHiddenFunction ( kBE_PreferenceGet, BE_NotImplemented, 1, 2 );
	g_be_plugin->RegisterHiddenFunction ( kBE_PreferenceDelete, BE_NotImplemented, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ScriptExecute, BE_NotImplemented, 1, 4 );
#endif

	g_be_plugin->RegisterFunction ( kBE_EvaluateJavaScript, BE_EvaluateJavaScript, 1 );

#if ( FMX_MAC_TARGET || FMX_WIN_TARGET )
	g_be_plugin->RegisterFunction ( kBE_GetSystemDrive, BE_GetSystemDrive );
	g_be_plugin->RegisterFunction ( kBE_ClipboardFormats, BE_ClipboardFormats );
	g_be_plugin->RegisterFunction ( kBE_FileSelect, BE_FileSelect, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_FolderSelect, BE_FolderSelect, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_FileSaveDialog, BE_FileSaveDialog, 1, 3 );
	g_be_plugin->RegisterFunction ( kBE_DialogDisplay, BE_DialogDisplay, 3, 5 );
	g_be_plugin->RegisterFunction ( kBE_FileOpen, BE_FileOpen, 1, 1 );
	g_be_plugin->RegisterFunction ( kBE_DialogProgress, BE_DialogProgress, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_DialogProgressUpdate, BE_DialogProgressUpdate, 1, 2 );
#else
	g_be_plugin->RegisterFunction ( kBE_GetSystemDrive, BE_NotImplemented );
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
	g_be_plugin->RegisterFunction ( kBE_FileReadText, BE_FileReadText, 1, 4 );
	g_be_plugin->RegisterFunction ( kBE_FileWriteText, BE_FileWriteText, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_XMLStripInvalidCharacters, BE_XMLStripInvalidCharacters, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ExportFieldContents, BE_ExportFieldContents, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_FileImport, BE_FileImport, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_FilePatternCount, BE_FilePatternCount, 2 );
	g_be_plugin->RegisterFunction ( kBE_FileReplaceText, BE_FileReplaceText, 3, 4 );

	g_be_plugin->RegisterFunction ( kBE_FileMove, BE_FileMove, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_FileCopy, BE_FileCopy, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_FileListFolder, BE_FileListFolder, 1, 5 );

	g_be_plugin->RegisterFunction ( kBE_FileTypeAll + kBE_FileTypeOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_FileTypeFile + kBE_FileTypeOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_FileTypeFolder + kBE_FileTypeOffset, BE_NumericConstants );

#ifdef BEP_PRO_VERSION
	g_be_plugin->RegisterFunction ( kBE_XSLT_Apply, BE_XSLT_Apply, 3, -1 );
#else
	g_be_plugin->RegisterFunction ( kBE_XSLT_Apply, BE_XSLT_Apply, 3 );
#endif

	g_be_plugin->RegisterFunction ( kBE_XSLT_ApplyInMemory, BE_XSLT_Apply, 2 );

	g_be_plugin->RegisterFunction ( kBE_XPath, BE_XPath, 2, 4 );
	g_be_plugin->RegisterFunction ( kBE_XPathAll, BE_XPath, 2, 3 );
	g_be_plugin->RegisterFunction ( kBE_XMLStripNodes, BE_XMLStripNodes, 3 );
	g_be_plugin->RegisterFunction ( kBE_XMLParse, BE_XMLParse, 1 );
	g_be_plugin->RegisterHiddenFunction ( kBE_SplitBEFileNodes, BE_SplitBEFileNodes, 1 );
	g_be_plugin->RegisterFunction ( kBE_XMLValidate, BE_XMLValidate, 2 );
	g_be_plugin->RegisterFunction ( kBE_XMLCanonical, BE_XMLCanonical, 1 );
	g_be_plugin->RegisterFunction ( kBE_XMLTidy, BE_XMLTidy, 1 );

	g_be_plugin->RegisterFunction ( kBE_TextExtractWords, BE_TextExtractWords, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_ButtonOK + kBE_ButtonOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_ButtonCancel + kBE_ButtonOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_ButtonAlternate + kBE_ButtonOffset, BE_NumericConstants );

#if ( FMX_MAC_TARGET || FMX_WIN_TARGET || FMX_LINUX_TARGET )
	g_be_plugin->RegisterFunction ( kBE_ExecuteSystemCommand, BE_ExecuteSystemCommand, 1, 3 );
#else
	g_be_plugin->RegisterHiddenFunction ( kBE_ExecuteSystemCommand, BE_NotImplemented, 1, 3 );
#endif

	g_be_plugin->RegisterFunction ( kBE_FileMakerSQL, BE_FileMakerSQL, 1, 6 );

	g_be_plugin->RegisterFunction ( kBE_ContainerIsCompressed, BE_ContainerIsCompressed, 1 );
	g_be_plugin->RegisterFunction ( kBE_ContainerCompress, BE_ContainerCompress, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_ContainerUncompress, BE_ContainerUncompress, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_ContainerListTypes, BE_ContainerListTypes, 1 );
	g_be_plugin->RegisterFunction ( kBE_ContainerGetType, BE_ContainerGetType, 2 );

	g_be_plugin->RegisterFunction ( kBE_Gzip, BE_Gzip, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_UnGzip, BE_UnGzip, 1, 2 );

	g_be_plugin->RegisterFunction ( kBE_JSON_ArraySize, BE_JSON_ArraySize, 1, 2 );
#if ( FMX_WIN_TARGET || FMX_IOS_TARGET )
	g_be_plugin->RegisterFunction ( kBE_JSON_jq, BE_NotImplemented, 2, 3 );
#else
	g_be_plugin->RegisterFunction ( kBE_JSON_jq, BE_JSON_jq, 2, 3 );
#endif


	g_be_plugin->RegisterFunction ( kBE_HTTP_GETFile, BE_HTTP_GET, 1, 4 );
	g_be_plugin->RegisterFunction ( kBE_SaveURLToFile, BE_HTTP_GETFile, 2, 4 );

	g_be_plugin->RegisterFunction ( kBE_Unzip_Deprecated, BE_Unzip, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_Unzip, BE_Unzip, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_Zip, BE_Zip, 1, 2 );
	g_be_plugin->RegisterFunction ( kBE_SetTextEncoding, BE_SetTextEncoding, 0, 1 );

	g_be_plugin->RegisterFunction ( kBE_JPEGRecompress, BE_JPEGRecompress, 1, 3 );

#if BEP_PRO_VERSION && !FMX_IOS_TARGET
	g_be_plugin->RegisterFunction ( kBE_ContainerConvertImage, BE_ContainerConvertImage, 1, 2 );
#endif

	g_be_plugin->RegisterFunction ( kBE_VariableSet, BE_Variable, 2 );
	g_be_plugin->RegisterFunction ( kBE_VariableGet, BE_Variable, 1 );
	g_be_plugin->RegisterFunction ( kBE_StackPush, BE_Stack, 2 );
	g_be_plugin->RegisterFunction ( kBE_StackPop, BE_Stack, 1 );
	g_be_plugin->RegisterFunction ( kBE_StackCount, BE_Stack, 1 );
	g_be_plugin->RegisterFunction ( kBE_StackDelete, BE_Stack, 1 );

	g_be_plugin->RegisterFunction ( kBE_EncryptAES, BE_EncryptAES, 2 );
	g_be_plugin->RegisterFunction ( kBE_DecryptAES, BE_DecryptAES, 2 );
	g_be_plugin->RegisterFunction ( kBE_SignatureGenerateRSA, BE_SignatureGenerateRSA, 2, 5 );
	g_be_plugin->RegisterFunction ( kBE_SignatureVerifyRSA, BE_SignatureVerifyRSA, 3, 4 );
	g_be_plugin->RegisterFunction ( kBE_CipherEncrypt, BE_CipherEncrypt, 3, 6 );
	g_be_plugin->RegisterFunction ( kBE_CipherDecrypt, BE_CipherDecrypt, 3, 6 );
	
	g_be_plugin->RegisterFunction ( kBE_EncryptWithKey, BE_EncryptWithKey, 2 );
	g_be_plugin->RegisterFunction ( kBE_DecryptWithKey, BE_DecryptWithKey, 2 );
	g_be_plugin->RegisterFunction ( kBE_CreateKeyPair, BE_CreateKeyPair );
	g_be_plugin->RegisterFunction ( kBE_GetPublicKey, BE_GetPublicKey, 1 );

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

	g_be_plugin->RegisterFunction ( kBE_MessageDigest, BE_MessageDigest, 1, 3 );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestAlgorithmMD5 + kBE_MessageDigestAlgorithmOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestAlgorithmSHA256 + kBE_MessageDigestAlgorithmOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestAlgorithmMDC2 + kBE_MessageDigestAlgorithmOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestAlgorithmSHA + kBE_MessageDigestAlgorithmOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestAlgorithmSHA1 + kBE_MessageDigestAlgorithmOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestAlgorithmSHA224 + kBE_MessageDigestAlgorithmOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestAlgorithmSHA384 + kBE_MessageDigestAlgorithmOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_MessageDigestAlgorithmSHA512 + kBE_MessageDigestAlgorithmOffset, BE_NumericConstants );

	g_be_plugin->RegisterFunction ( kBE_DebugInformation, BE_DebugInformation );

	g_be_plugin->RegisterFunction ( kBE_EncodingHex + kBE_EncodingOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kBE_EncodingBase64 + kBE_EncodingOffset, BE_NumericConstants );

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
	g_be_plugin->RegisterFunction ( kBE_FTP_UploadFile, BE_FTP_Upload, 2, 4 );
	g_be_plugin->RegisterFunction ( kBE_CurlGetInfo, BE_CurlGetInfo, 1 );

#ifdef BEP_PRO_VERSION
		
	g_be_plugin->RegisterFunction ( kBE_BackgroundTaskAdd, BE_BackgroundTaskAdd, 7, 9 );
	g_be_plugin->RegisterFunction ( kBE_BackgroundTaskList, BE_BackgroundTaskList );

#endif

#ifdef BEP_PRO_VERSION
	g_be_plugin->RegisterFunction ( kBE_SMTPServer, BE_SMTPServer, 1, 5 );
#else
	g_be_plugin->RegisterFunction ( kBE_SMTPServer, BE_SMTPServer, 1, 4 );
#endif

	g_be_plugin->RegisterFunction ( kBE_SMTPServer, BE_SMTPServer, 1, 5 );
	g_be_plugin->RegisterFunction ( kBE_SMTPSend, BE_SMTPSend, 4, 9 );
	g_be_plugin->RegisterFunction ( kBE_SMTPAddAttachment, BE_SMTPAddAttachment, 0, 2 );
	g_be_plugin->RegisterFunction ( kBE_SMTPSetHeader, BE_Net_Set_Header, 0, 2 );

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


	g_be_plugin->RegisterFunction ( kFMXT_Halt + kBE_ScriptControlOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kFMXT_Exit + kBE_ScriptControlOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kFMXT_Resume + kBE_ScriptControlOffset, BE_NumericConstants );
	g_be_plugin->RegisterFunction ( kFMXT_Pause + kBE_ScriptControlOffset, BE_NumericConstants );


	return kCurrentExtnVersion;	// enable the plug-in

} // LoadPlugin



static void UnloadPlugin ( void ) {


#ifndef FMX_IOS_TARGET
    Magick::TerminateMagick();
#endif

	CleanupLibXSLT();

	delete g_be_plugin;	// un-register the plugin functions

}


// main entry point
// calls for plug-in functions go direct to the registered function

void FMX_ENTRYPT BEP_EXPORT FMExternCallProc ( FMX_ExternCallPtr plugin_call ) {

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

