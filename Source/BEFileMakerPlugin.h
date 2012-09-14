/*
 BEFileMakerPlugin.h
 BaseElements Plug-In
 
 Copyright 2010-2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEFILEMAKERPLUGIN_H)
	#define BEFILEMAKERPLUGIN_H

#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXCalcEngine.h"

#include <vector>


using namespace std;
using namespace fmx;


// the plug-in object... register & un-register functions

class BEFileMakerPlugin {
	
	QuadCharAutoPtr id;
	vector<short> registered_functions;
	
public:
	
    BEFileMakerPlugin ( QuadCharAutoPtr plugin_id );
	~BEFileMakerPlugin();
	
    errcode RegisterHiddenFunction ( short function_id, ExtPluginType funcion_pointer, bool server_compatible, int minumum_parameters = 0, int maximum_parameters = NULL );
    errcode RegisterFunction ( short function_id, ExtPluginType funcion_pointer, bool server_compatible, int minumum_parameters = 0, int maximum_parameters = NULL );
    errcode RegisterPluginFunction ( FMX_UInt32 function_flags, short function_id, ExtPluginType funcion_pointer, bool server_compatible, int minumum_parameters = 0, int maximum_parameters = NULL );
    errcode RegisterFunction ( short function_id, ExtPluginType funcion_pointer, int minumum_parameters = 0 );

};

#endif // BEFILEMAKERPLUGIN_H
