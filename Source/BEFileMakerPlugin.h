/*
 BEFileMakerPlugin.h
 BaseElements Plug-In
 
 Copyright 2010-2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEFILEMAKERPLUGIN_H)
	#define BEFILEMAKERPLUGIN_H


#include "BEPluginGlobalDefines.h"


#include <vector>


// the plug-in object... register & un-register functions

class BEFileMakerPlugin {
	
	fmx::QuadCharAutoPtr id;
	std::vector<short> registered_functions;
	
public:
	
    BEFileMakerPlugin ( fmx::QuadCharAutoPtr plugin_id );
	~BEFileMakerPlugin();
	
    fmx::errcode RegisterHiddenFunction ( short function_id, fmx::ExtPluginType funcion_pointer, bool server_compatible, int minumum_parameters = 0, int maximum_parameters = NULL );
    fmx::errcode RegisterFunction ( short function_id, fmx::ExtPluginType funcion_pointer, bool server_compatible, int minumum_parameters = 0, int maximum_parameters = NULL );
    fmx::errcode RegisterPluginFunction ( FMX_UInt32 function_flags, short function_id, fmx::ExtPluginType funcion_pointer, bool server_compatible, int minumum_parameters = 0, int maximum_parameters = NULL );
    fmx::errcode RegisterFunction ( short function_id, fmx::ExtPluginType funcion_pointer, int minumum_parameters = 0 );

};

#endif // BEFILEMAKERPLUGIN_H
