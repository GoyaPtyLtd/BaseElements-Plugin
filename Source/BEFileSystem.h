/*
 BEFileSystem.h
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEFILESYSTEM_H)
#define BEFILESYSTEM_H


#include "boost/filesystem.hpp"


bool recursive_directory_copy ( const boost::filesystem::path & from, const boost::filesystem::path & to  );


#endif // BEFILESYSTEM_H
