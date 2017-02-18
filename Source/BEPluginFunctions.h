/*
 BEPluginFunctions.h
 BaseElements Plug-In
 
 Copyright 2010-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEPLUGINFUNCTIONS_H)
	#define BEPLUGINFUNCTIONS_H


#include "BEPluginGlobalDefines.h"


// The following functions are registered with FileMaker as the external functions made available via the plug-in.

fmx::errcode BE_Version ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_VersionAutoUpdate ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );

fmx::errcode BE_GetLastError ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& /* data_vect */, fmx::Data& results );

fmx::errcode BE_ClipboardFormats ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_ClipboardData ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_SetClipboardData ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );

fmx::errcode BE_CreateFolder ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_DeleteFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_FileExists ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_FileSize ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_File_Modification_Timestamp ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_ReadTextFromFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_WriteTextToFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_StripInvalidUTF16CharactersFromXMLFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_ExportFieldContents ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_ImportFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );

fmx::errcode BE_MoveFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_CopyFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_ListFilesInFolder ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );


fmx::errcode BE_SelectFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_SelectFolder ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_SaveFileDialog ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );

fmx::errcode BE_DisplayDialog ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_ProgressDialog ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ProgressDialog_Update ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_ApplyXSLT ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );

fmx::errcode BE_ApplyXSLTInMemory ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_XPath ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_XPathAll ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_StripXMLNodes ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_XML_Parse ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_SplitBEFileNodes ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_ContainerIsCompressed ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ContainerCompress ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ContainerUncompress ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Gzip ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_UnGzip ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_JSONPath ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_JSON_Error_Description ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_JSON_ArraySize ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_JSON_Encode ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_EvaluateJavaScript ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_ArraySetFromValueList ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ArrayGetSize ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ArrayGetValue ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Array_Delete ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_SetPreference ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_GetPreference ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_Unzip ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Zip ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Base64_Decode ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Base64_Encode ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_SetTextEncoding ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_Encrypt_AES ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Decrypt_AES ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_HTTP_GET ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_GET_File ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_POST_PUT_PATCH ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_DELETE ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_Response_Code ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_Response_Headers ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_Set_Custom_Header ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HTTP_Set_Proxy ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Curl_Set_Option ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FTP_Upload ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Curl_Trace ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FTP_Delete ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_ExtractScriptVariables ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_NumericConstants ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& data_vect, fmx::Data& results );
fmx::errcode BE_TimeFunctions ( const short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& /* parameters */, fmx::Data& results );
fmx::errcode BE_ExecuteSystemCommand ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_OpenURL ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_OpenFile ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ExecuteScript ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_FileMakerSQL ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_MessageDigest ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_HMAC ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_PBKDF2_HMAC ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_JPEG_Recompress ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_ConvertContainer ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_OAuth_RequestAccessToken ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Xero_GenerateKeys ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_Xero_SetTokens ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_SMTP_Server ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_SMTP_Send ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_SMTP_AddAttachment ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_FMS_Command ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_Values_Unique ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Values_FilterOut ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Values_ContainsDuplicates ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Values_Sort ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Values_TimesDuplicated ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Values_Trim ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_XOR ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_Vector_DotProduct ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Vector_EuclideanDistance ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_RegularExpression ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );
fmx::errcode BE_Pause ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


fmx::errcode BE_Get_Machine_Name ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );

fmx::errcode BE_PDF_Append ( short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& parameters, fmx::Data& results );


#endif // BEPLUGINFUNCTIONS_H
