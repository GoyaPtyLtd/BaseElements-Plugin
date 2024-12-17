/*
 BEFileSystem.h
 BaseElements Plug-In
 
 Copyright 2011-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEFILESYSTEM_H)
#define BEFILESYSTEM_H


#include "BEValueList.h"

#include <filesystem>


BEValueListWideStringUniquePtr list_files_in_directory ( const std::filesystem::path & directory, const long file_type_wanted, const bool recurse, const bool include_hidden );
const fmx::errcode write_to_file ( const std::filesystem::path& new_file, const std::vector<char>& contents, const std::ios_base::openmode flags = std::ios_base::binary | std::ios_base::ate );
const uintmax_t file_or_directory_size ( const std::filesystem::path& path );


#endif // BEFILESYSTEM_H
