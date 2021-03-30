/*
 BEZlib.h
 BaseElements Plug-In
 
 Copyright 2011-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEZLIB_H)
#define BEZLIB_H


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"
#include "BEValueList.h"

#include <vector>


const long UnZipFile ( const std::string& archive, const std::string& output_directory );
const long UnZipMemory ( const std::vector<char>& archive, const std::string& output_directory );
const long ZipFiles ( const BEValueList<std::string> * filename, const std::string& archive );
const long ZipMemory ( const std::vector<char>& data, const std::string& filename, const std::string& archive );

const std::vector<char> CompressContainerStream ( const std::vector<char> data );
const std::vector<char> UncompressContainerStream ( const std::vector<char> data );


#endif // BEZLIB_H
