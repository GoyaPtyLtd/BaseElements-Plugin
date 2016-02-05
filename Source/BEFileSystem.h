/*
 BEFileSystem.h
 BaseElements Plug-In
 
 Copyright 2011-2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEFILESYSTEM_H)
#define BEFILESYSTEM_H


#include "BEValueList.h"

#include "boost/filesystem.hpp"


bool recursive_directory_copy ( const boost::filesystem::path & from, const boost::filesystem::path & to  );
BEValueListWideStringAutoPtr list_files_in_directory ( const boost::filesystem::path & directory, const long file_type_wanted, const bool recurse );
fmx::errcode write_to_file ( const boost::filesystem::path& new_file, const std::vector<char>& contents, const std::ios_base::openmode flags = std::ios_base::binary | std::ios_base::ate );


#endif // BEFILESYSTEM_H
