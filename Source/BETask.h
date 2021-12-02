/*
 BETask.h
 BaseElements Plug-In

 Copyright 2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

*/


#ifndef BETask_h
#define BETask_h


#include "BEPluginGlobalDefines.h"


#include <string>
#include <vector>


typedef std::pair<fmx::errcode,std::string> BETaskResult;


const BETaskResult xslt_task ( const std::vector<std::wstring>& parameters, const fmx::uint32 first_parameter, const std::string& xml, const std::string& script_name, const std::string& database_name );





#endif /* BETask_h */
