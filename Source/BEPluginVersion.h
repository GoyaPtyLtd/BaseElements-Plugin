/*
 BEPluginVersion.h
 BaseElements Plug-In
 
 Copyright 2015-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEPLUGINVERSION_H)
#define BEPLUGINVERSION_H


#define PLUGIN_DESCRIPTION_STRING_ID 65000
#define PLUGIN_DESCRIPTION_STRING_ID_STRING "PLUGIN_DESCRIPTION_STRING_ID"

// this is the only place the version information is set
// building the macOS version will automatically set the version information for win/linux

#define	AUTO_UPDATE_VERSION "04019908"
#define VERSION_NUMBER_STRING "4.2.0" // e.g. 4.0.1
#define VERSION_STRING "4.2.0b2"	// e.g. 4.0.1b2

#define USER_AGENT_STRING "libcurl-agent-base-elements-plugin/" VERSION_STRING


#endif /* BEPLUGINVERSION_H */
