/*
 BEWinFunctions.h
 BaseElements Plug-in

 Copyright 2010-2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#if !defined(BEWINFUNCTIONS_H)  		// Only include once.
	#define BEWINFUNCTIONS_H

#include "BEPluginUtilities.h"
#include "BEPluginGlobalDefines.h"

#include <string>


WStringAutoPtr ClipboardFormats ( void );
StringAutoPtr ClipboardData ( WStringAutoPtr atype );
bool SetClipboardData ( StringAutoPtr data, WStringAutoPtr atype );

WStringAutoPtr SelectFile ( WStringAutoPtr prompt, WStringAutoPtr in_folder );
WStringAutoPtr SelectFolder ( WStringAutoPtr prompt, WStringAutoPtr in_folder );

int DisplayDialog ( WStringAutoPtr title, WStringAutoPtr message, WStringAutoPtr button1, WStringAutoPtr button2, WStringAutoPtr button3 );


// Progress Dialog

fmx::errcode DisplayProgressDialog ( const WStringAutoPtr title, const WStringAutoPtr description, const long maximum, const bool can_cancel );
fmx::errcode UpdateProgressDialog ( const long value, const WStringAutoPtr description );


// user preferences

bool SetPreference ( WStringAutoPtr key, WStringAutoPtr value, WStringAutoPtr domain );
WStringAutoPtr GetPreference ( WStringAutoPtr key, WStringAutoPtr domain );


// other

bool OpenURL ( WStringAutoPtr url );
bool OpenFile ( WStringAutoPtr file );

// utilities

std::wstring utf8toutf16 ( const std::string& instr );
std::string utf16ToUTF8 ( const std::wstring& s );


#endif // BEWINFUNCTIONS_H
