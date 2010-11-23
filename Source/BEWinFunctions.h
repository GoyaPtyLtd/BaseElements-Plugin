/*
 BEWinFunctions.h
 BaseElements Plug-in

 Copyright 2010 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin

 */


#if !defined(BEWINFUNCTIONS_H)  		// Only include once.
	#define BEWINFUNCTIONS_H

#include "BEPluginUtilities.h"
#include "BEPluginGlobalDefines.h"

#include <string>

using namespace std;

StringAutoPtr ClipboardFormats ( void );
StringAutoPtr ClipboardData ( StringAutoPtr atype );
bool SetClipboardData ( StringAutoPtr data, StringAutoPtr atype );

StringAutoPtr SelectFile ( StringAutoPtr prompt );
StringAutoPtr SelectFolder ( StringAutoPtr prompt );

int DisplayDialog ( StringAutoPtr title, StringAutoPtr message, StringAutoPtr button1, StringAutoPtr button2, StringAutoPtr button3 );

bool OpenURL ( StringAutoPtr url );

#endif // BEWINFUNCTIONS_H
