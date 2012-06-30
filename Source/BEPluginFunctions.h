/*
 BEPluginFunctions.h
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEPLUGINFUNCTIONS_H)
	#define BEPLUGINFUNCTIONS_H


#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXText.h"


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"
#include "BESQLCommand.h"


using namespace fmx;



// The following functions are registered with FileMaker as the external functions made available via the plug-in.

FMX_PROC(errcode) BE_Version ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_VersionAutoUpdate ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );

FMX_PROC(errcode) BE_GetLastError ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& /* data_vect */, Data& results);

FMX_PROC(errcode) BE_ClipboardFormats ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_ClipboardData ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_SetClipboardData ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );

FMX_PROC(errcode) BE_CreateFolder ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_DeleteFile ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_FileExists ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_ReadTextFromFile ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_WriteTextToFile ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_StripInvalidUTF16CharactersFromXMLFile ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );

FMX_PROC(errcode) BE_MoveFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results);
FMX_PROC(errcode) BE_CopyFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results);
FMX_PROC(errcode) BE_ListFilesInFolder ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results);


FMX_PROC(errcode) BE_SelectFile ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_SelectFolder ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_DisplayDialog ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );

FMX_PROC(errcode) BE_ApplyXSLT ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );

FMX_PROC(errcode) BE_ApplyXSLTInMemory ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results);
FMX_PROC(errcode) BE_XPath ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_XPathAll ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results );


FMX_PROC(errcode) BE_SetPreference ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results);
FMX_PROC(errcode) BE_GetPreference ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results);


FMX_PROC(errcode) BE_Unzip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_Zip ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_Base64_Decode ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_Base64_Encode ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_SetTextEncoding ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );


FMX_PROC(errcode) BE_GetURL ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_SaveURLToFile ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_HTTP_POST ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_HTTP_DELETE ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_HTTP_Response_Code ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_HTTP_Response_Headers ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_HTTP_Set_Custom_Header ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );


FMX_PROC(errcode) BE_ExtractScriptVariables ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_NumericConstants ( short funcId, const ExprEnv& environment, const DataVect& data_vect, Data& results );
FMX_PROC(errcode) BE_ExecuteShellCommand ( short /* funcId */, const ExprEnv& /* environment */, const DataVect& data_vect, Data& results);
FMX_PROC(errcode) BE_FileMaker_TablesOrFields ( short function_id, const ExprEnv& environment, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_OpenURL ( short funcId, const ExprEnv& environment, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_OpenFile ( short /*funcId*/, const ExprEnv& /* environment */, const DataVect& parameters, Data& results);
FMX_PROC(errcode) BE_ExecuteScript ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_FileMakerSQL ( short /* funcId */, const ExprEnv& environment, const DataVect& parameters, Data& results );
FMX_PROC(errcode) BE_MessageDigest ( short funcId, const ExprEnv& /* environment */, const DataVect& parameters, Data& results );


#endif // BEPLUGINFUNCTIONS_H
