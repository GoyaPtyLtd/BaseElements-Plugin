/*
 BEXMLSchema.h
 BaseElements Plug-In
 
 Copyright (c) 2020-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef BEXMLSchema_h
#define BEXMLSchema_h


#include <string>


const std::string validate_xml ( const std::string xml, const std::string schema );
const std::string canonical_xml ( const std::string xml );
const std::string pretty_print_xml ( const std::string xml );


#endif /* BEXMLSchema_h */
