/*
 BEMessageDigest.h
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEMESSAGEDIGEST_H)
#define BEMESSAGEDIGEST_H


#include "BEPluginUtilities.h"




StringAutoPtr MD5 ( StringAutoPtr message );
StringAutoPtr SHA256 ( StringAutoPtr message );



#endif // BEMESSAGEDIGEST_H

