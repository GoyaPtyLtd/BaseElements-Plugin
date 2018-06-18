/*
 BELinuxFunctions.h
 BaseElements Plug-In

 Copyright 2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#if !defined(BELINUXFUNCTIONS_H)
	#define BELINUXFUNCTIONS_H

#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"

#include <string>



void InitialiseForPlatform ( );

std::wstring ClipboardFormats ( void );
std::string ClipboardData ( std::wstring& atype );
bool SetClipboardData ( std::string& data, std::wstring& atype );

std::wstring SelectFile ( std::wstring& prompt, std::wstring& in_folder );
std::wstring SelectFolder ( std::wstring& prompt, std::wstring& in_folder );
std::wstring SaveFileDialog ( std::wstring& prompt, std::wstring& fileName, std::wstring& inFolder );

int DisplayDialog ( std::wstring& title, std::wstring& message, std::wstring& ok_button, std::wstring& cancel_button, std::wstring& alternate_button );


// Progress Dialog

fmx::errcode DisplayProgressDialog ( const std::wstring& title, const std::wstring& description, const long maximum, const bool can_cancel );
fmx::errcode UpdateProgressDialog ( const long value, const std::wstring& description );


// user preferences

bool SetPreference ( std::wstring& key, std::wstring& value, std::wstring& domain );
std::wstring GetPreference ( std::wstring& key, std::wstring& domain );


// other

bool OpenURL ( std::wstring& url );
bool OpenFile ( std::wstring& path );


const std::wstring get_machine_name ( );


// used by Do_GetString
unsigned long Sub_LinuxLoadString ( const unsigned long string_id, FMX_Unichar * string, const long intoHereMax );


#endif // BELINUXFUNCTIONS_H

