/*
 BEXMLReader.h
 BaseElements Plug-In
 
 Copyright 2012-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEXMLREADER_H)
#define BEXMLREADER_H


#include "BEPluginUtilities.h"


int StripXMLNodes ( const std::string input_file, const std::filesystem::path output_file, const std::vector<std::string> node_names );
int SplitBEXMLFiles ( const std::filesystem::path input_file );



#endif // BEXMLREADER_H
