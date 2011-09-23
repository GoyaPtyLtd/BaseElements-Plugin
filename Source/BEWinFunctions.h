/*
 BEWinFunctions.h
 BaseElements Plug-in

 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#if !defined(BEWINFUNCTIONS_H)  		// Only include once.
	#define BEWINFUNCTIONS_H

#include "BEPluginUtilities.h"
#include "BEPluginGlobalDefines.h"

#include <string>

using namespace std;

WStringAutoPtr ClipboardFormats ( void );
StringAutoPtr ClipboardData ( WStringAutoPtr atype );
bool SetClipboardData ( StringAutoPtr data, WStringAutoPtr atype );

WStringAutoPtr SelectFile ( WStringAutoPtr prompt );
WStringAutoPtr SelectFolder ( WStringAutoPtr prompt );

int DisplayDialog ( WStringAutoPtr title, WStringAutoPtr message, WStringAutoPtr button1, WStringAutoPtr button2, WStringAutoPtr button3 );

// user preferences

bool SetPreference ( WStringAutoPtr key, WStringAutoPtr value, WStringAutoPtr domain );
WStringAutoPtr GetPreference ( WStringAutoPtr key, WStringAutoPtr domain );


// other

bool OpenURL ( WStringAutoPtr url );


#endif // BEWINFUNCTIONS_H
