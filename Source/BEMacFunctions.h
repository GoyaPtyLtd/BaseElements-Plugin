/*
 BEMacFunctions.h
 BaseElements Plug-In
 
 Copyright 2010 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEMACFUNCTIONS_H)
	#define BEMACFUNCTIONS_H

#include "BEPluginUtilities.h"

#include <string>


using namespace std;


StringAutoPtr ClipboardFormats ( void );
StringAutoPtr ClipboardData ( StringAutoPtr atype );
bool SetClipboardData ( StringAutoPtr data, StringAutoPtr atype );

StringAutoPtr SelectFile ( StringAutoPtr prompt );
StringAutoPtr SelectFolder ( StringAutoPtr prompt );

int DisplayDialog ( StringAutoPtr title, StringAutoPtr message, StringAutoPtr ok_button, StringAutoPtr cancel_button, StringAutoPtr alternate_button );


bool OpenURL ( StringAutoPtr url );


#endif // BEMACFUNCTIONS_H
