/*
 BEValueList.h
 BaseElements Plug-In
 
 Copyright 2013-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEValueList__
#define __BaseElements__BEValueList__

#include <iostream>
#include <string>
#include <vector>


class BEValueList {
	
	
public:
	
	BEValueList ( void );
	BEValueList ( const std::string value_list );
	
	bool not_empty ( );
	void append ( BEValueList to_append );
	void append ( const std::string to_append );
	
	std::string unique ( );
	std::string filter_out ( std::auto_ptr<BEValueList> filter_out );
	std::string sort ( );
	
	std::vector<std::string> get_values ( void ) { return values; }
	std::string get_as_comma_separated ( void );
	std::string get_as_filemaker_string ( void );
	
protected:
	
	std::vector<std::string> values;
	
};


#endif /* defined(__BaseElements__BEValueList__) */
