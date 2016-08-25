/*
 BEZlib.h
 BaseElements Plug-In
 
 Copyright 2011-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEZLIB_H)
#define BEZLIB_H


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"
#include "BEValueList.h"

#include <vector>


const long UnZip ( const std::string& archive, const std::string& output_directory );
const long Zip ( const BEValueList<std::string> * filename, const std::string& archive );

const std::vector<char> CompressContainerStream ( const std::vector<char> data );
const std::vector<char> UncompressContainerStream ( const std::vector<char> data );


#endif // BEZLIB_H
