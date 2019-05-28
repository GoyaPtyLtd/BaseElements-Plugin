/*
 BEMacFunctions.h
 BaseElements Plug-In
 
 Copyright 2010-2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEMACFUNCTIONS_H)
	#define BEMACFUNCTIONS_H

#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"

#include <string>
#include <vector>


void InitialiseForPlatform ( );

const std::wstring ClipboardFormats ( void );
const std::string ClipboardText ( const std::wstring& atype );
const bool SetClipboardText ( const std::string& data, const std::wstring& atype );
const std::vector<unsigned char> ClipboardFile ( const std::wstring& atype );
const bool SetClipboardFile ( const std::vector<unsigned char>& data, const std::wstring& atype );

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
const std::string get_system_drive ( );


#endif // BEMACFUNCTIONS_H
