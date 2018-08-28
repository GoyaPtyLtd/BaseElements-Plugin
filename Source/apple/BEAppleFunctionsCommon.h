/*
 BEAppleFunctionsCommon.h
 BaseElements Plug-In
 
 Copyright 2017-2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEAppleFunctionsCommon_h)
	#define BEAppleFunctionsCommon_h



#include "BEPluginGlobalDefines.h"

#include <string>


// used by Do_GetString
const unsigned long  Sub_LoadString ( unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax, const std::string bundleId = "" );


#endif // BEAppleFunctionsCommon_h

