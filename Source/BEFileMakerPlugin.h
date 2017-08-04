/*
 BEFileMakerPlugin.h
 BaseElements Plug-In
 
 Copyright 2010-2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEFILEMAKERPLUGIN_H)
	#define BEFILEMAKERPLUGIN_H


#include "BEPluginGlobalDefines.h"


#include <vector>


// the plug-in object... register & un-register functions

class BEFileMakerPlugin {
	
	fmx::QuadCharUniquePtr plugin_id;
	std::vector<short> registered_functions;
	
public:
	
    BEFileMakerPlugin ( fmx::QuadChar& plugin_id );
	~BEFileMakerPlugin();
	
    const fmx::errcode RegisterHiddenFunction ( short function_id, fmx::ExtPluginType funcion_pointer, int minumum_parameters = 0, int maximum_parameters = 0 );
    const fmx::errcode RegisterFunction ( short function_id, fmx::ExtPluginType funcion_pointer, int minumum_parameters = 0, int maximum_parameters = 0 );
	const fmx::errcode RegisterPluginFunction ( fmx::uint32 function_flags, short function_id, fmx::ExtPluginType funcion_pointer, int minumum_parameters = 0, int maximum_parameters = 0 );
	
	const fmx::QuadCharUniquePtr id ( void ) const { fmx::QuadCharUniquePtr id_copy ( *plugin_id ); return id_copy; };

	void set_fmx_application ( const FMX_Application fmx_application_value );
	const bool running_on_server ( ) const;

protected:
	
	FMX_Application fmx_application;

};

#endif // BEFILEMAKERPLUGIN_H
