/*
 BEMacFunctions.h
 BaseElements Plug-In
 
 Copyright 2010-2013 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEMACFUNCTIONS_H)
	#define BEMACFUNCTIONS_H

#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"

#include <string>


WStringAutoPtr ClipboardFormats ( void );
StringAutoPtr ClipboardData ( WStringAutoPtr atype );
bool SetClipboardData ( StringAutoPtr data, WStringAutoPtr atype );

WStringAutoPtr SelectFile ( WStringAutoPtr prompt, WStringAutoPtr in_folder );
WStringAutoPtr SelectFolder ( WStringAutoPtr prompt, WStringAutoPtr in_folder );
WStringAutoPtr SaveFileDialog ( WStringAutoPtr prompt, WStringAutoPtr fileName, WStringAutoPtr inFolder );

int DisplayDialog ( WStringAutoPtr title, WStringAutoPtr message, WStringAutoPtr ok_button, WStringAutoPtr cancel_button, WStringAutoPtr alternate_button );


// Progress Dialog

fmx::errcode DisplayProgressDialog ( const WStringAutoPtr title, const WStringAutoPtr description, const long maximum, const bool can_cancel );
fmx::errcode UpdateProgressDialog ( const long value, const WStringAutoPtr description );


// user preferences

bool SetPreference ( WStringAutoPtr key, WStringAutoPtr value, WStringAutoPtr domain );
WStringAutoPtr GetPreference ( WStringAutoPtr key, WStringAutoPtr domain );


// other

bool OpenURL ( WStringAutoPtr url );
bool OpenFile ( WStringAutoPtr path );


#endif // BEMACFUNCTIONS_H
