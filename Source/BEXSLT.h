/*
 BEXSLT.h
 BaseElements Plug-In
 
 Copyright 2010-2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEXSLT_H)
	#define BEXSLT_H


#include "BEPluginUtilities.h"


using namespace fmx;


TextAutoPtr ApplyXSLT ( StringAutoPtr xml_path, StringAutoPtr xslt, StringAutoPtr csv_path );

TextAutoPtr ApplyXSLTInMemory (StringAutoPtr xml, StringAutoPtr xslt );
TextAutoPtr ApplyXPath ( StringAutoPtr xml, StringAutoPtr xpath, StringAutoPtr nsList );
TextAutoPtr ApplyXPathAll ( StringAutoPtr xml, StringAutoPtr xpath, StringAutoPtr nsList );

#endif // BEXSLT_H
