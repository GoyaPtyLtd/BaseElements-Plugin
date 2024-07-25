/*
 BEPluginFunctions.h
 BaseElements Plug-In
 
 Copyright 2010-2023 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEPLUGINFUNCTIONS_H)
	#define BEPLUGINFUNCTIONS_H


#include "BEPluginGlobalDefines.h"


// The following functions are registered with FileMaker as the external functions made available via the plug-in.

fmx::errcode BE_Version ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_GetLastError ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& /* parameters */, fmx::Data& results );

fmx::errcode BE_ClipboardFormats ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ClipboardGetText ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ClipboardSetText ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ClipboardGetFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ClipboardSetFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_FolderCreate ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileDelete ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileExists ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileSize ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileModificationTimestamp ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileReadText ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileWriteText ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_XMLStripInvalidCharacters ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ExportFieldContents ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileImport ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FilePatternCount ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileReplaceText ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_FileMove ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileCopy ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileListFolder ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_FileSelect ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FolderSelect ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileSaveDialog ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_DialogDisplay ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_DialogProgress ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_DialogProgressUpdate ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_XSLT_Apply ( short function_id, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_XPath ( short function_id, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_XMLStripNodes ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_XMLParse ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_SplitBEFileNodes ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_XMLValidate ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_XMLCanonical ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_XMLTidy ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_ContainerIsCompressed ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ContainerCompress ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ContainerUncompress ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_ContainerListTypes ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ContainerGetType ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_Gzip ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_UnGzip ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_EvaluateJavaScript ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_ArraySetFromValueList ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ArrayGetSize ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ArrayGetValue ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ArrayDelete ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ArrayFind ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ArrayChangeValue ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_PreferenceSet ( short function_id, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Preference ( short function_id, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_Unzip ( const short fucnction_id, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Zip ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_SetTextEncoding ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_HTTP_GET ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_GETFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_POST_PUT_PATCH ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_DELETE ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_ResponseCode ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_ResponseHeaders ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Net_Set_Header ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_SetProxy ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_CurlSetOption ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FTP_Upload ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_CurlTrace ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FTP_Delete ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_CurlGetInfo ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_BackgroundTaskAdd ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_BackgroundTaskList ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_TextExtractWords ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_NotImplemented ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_NumericConstants ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_TimeFunctions ( const short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& /* parameters */, fmx::Data& results );
fmx::errcode BE_ExecuteSystemCommand ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_OpenURL ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileOpen ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ScriptExecute ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileMakerSQL ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_JPEGRecompress ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ContainerConvertImage ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ConvertContainer ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_SMTPServer ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_SMTPSend ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_SMTPAddAttachment ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_Variable ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Stack ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_ValuesUnique ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ValuesFilterOut ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ValuesContainsDuplicates ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ValuesSort ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ValuesTimesDuplicated ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ValuesTrim ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_XOR ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_VectorDotProduct ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Vector_EuclideanDistance ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_RegularExpression ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Pause ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_GetMachineName ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_GetSystemDrive ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_PDFAppend ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_PDFPageCount ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_PDFGetPages ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_ScriptStepInstall ( short /* function_id */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply );
fmx::errcode BE_ScriptStepRemove ( short /* function_id */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& parameters, fmx::Data& reply );
fmx::errcode BE_ScriptStepPerform ( short function_id, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& reply );

fmx::errcode BE_SignatureGenerateRSA ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_SignatureVerifyRSA ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_CipherEncrypt ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_CipherDecrypt ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_EncryptWithKey ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_DecryptWithKey ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_CreateKeyPair ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_GetPublicKey ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_DebugInformation ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_MessageDigest ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_EncryptAES ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_DecryptAES ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_JSON_ArraySize ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
#if ( !FMX_WIN_TARGET && !FMX_IOS_TARGET )
	fmx::errcode BE_JSON_jq ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
#endif


#endif // BEPLUGINFUNCTIONS_H
