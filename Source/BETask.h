/*
 BETask.h
 BaseElements Plug-In

 Copyright 2021-2022 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

*/


#ifndef BETask_h
#define BETask_h


#include "BEPluginGlobalDefines.h"

#include <string>
#include <vector>

#include <boost/filesystem.hpp>


typedef std::pair<fmx::errcode,std::string> BETaskResult;


const BETaskResult xslt_task ( std::vector<boost::filesystem::path>& csv_paths, std::vector<std::string>& xslt, const std::string& xml, const boost::filesystem::path& xml_path, const std::string& script_name, const std::string& database_name );





#endif /* BETask_h */
