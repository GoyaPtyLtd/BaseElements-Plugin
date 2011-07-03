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


using namespace boost::filesystem;


bool recursive_directory_copy ( const path & from, const path & to  );



#endif // BEFILESYSTEM_H
