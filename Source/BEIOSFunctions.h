/*
 BEIOSFunctions.h
 BaseElements Plug-In
 
 Copyright 2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEIOSFUNCTIONS_H)
	#define BEIOSFUNCTIONS_H



#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"

#include <string>



void InitialiseForPlatform ( );

const std::wstring ClipboardFormats ( void );
const std::string ClipboardData ( std::wstring& atype );
const bool SetClipboardData ( std::string& data, std::wstring& atype );

const std::wstring SelectFile ( const std::wstring& prompt, const std::wstring& in_folder );
const std::wstring SelectFolder ( const std::wstring& prompt, const std::wstring& in_folder );
const std::wstring SaveFileDialog ( std::wstring& prompt, std::wstring& fileName, std::wstring& inFolder );

const int DisplayDialog ( std::wstring& title, std::wstring& message, std::wstring& ok_button, std::wstring& cancel_button, std::wstring& alternate_button );


// Progress Dialog

const fmx::errcode DisplayProgressDialog ( const std::wstring& title, const std::wstring& description, const long maximum, const bool can_cancel );
const fmx::errcode UpdateProgressDialog ( const long value, const std::wstring& description );


// user preferences

const bool SetPreference ( std::wstring& key, std::wstring& value, std::wstring& domain );
const std::wstring GetPreference ( std::wstring& key, std::wstring& domain );


// other

const bool OpenURL ( std::wstring& url );
const bool OpenFile ( std::wstring& path );

const std::wstring get_machine_name ( );


#endif // BEIOSFUNCTIONS_H

