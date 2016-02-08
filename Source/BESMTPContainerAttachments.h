/*
 BESMTPContainerAttachments.h
 BaseElements Plug-In
 
 Copyright 2016 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef BESMTPContainerAttachments_hpp
#define BESMTPContainerAttachments_hpp


#include "BEValueList.h"


#include <boost/filesystem.hpp>


class BESMTPContainerAttachments {
	
	
public:
	
	BESMTPContainerAttachments ( void ) {};
	~BESMTPContainerAttachments();
	
	void add ( std::string file_name, std::vector<char>& data );
	BEValueList<boost::filesystem::path>  get_file_list ( void ) { return file_paths; };
	void clear ( void );
	
private:
	
	BEValueList<boost::filesystem::path> file_paths;
	
};


#endif /* BESMTPContainerAttachment_hpp */
