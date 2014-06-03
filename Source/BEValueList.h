/*
 BEValueList.h
 BaseElements Plug-In
 
 Copyright 2013-2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEValueList__
#define __BaseElements__BEValueList__


#include "BEPluginGlobalDefines.h"

#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>



template <typename T>

class BEValueList {
	
	
public:
	
	BEValueList ( void );
	BEValueList ( const T value_list );
	BEValueList ( const std::vector<T> value_list );
	
	bool not_empty ( );
	size_t size ( ) const;

	void append ( BEValueList to_append );
	void append ( const T to_append );
	
	T first ( ) const;
	T at ( const size_t which ) const;
	
	T unique ( );
	T filter_out ( std::auto_ptr<BEValueList> filter_out );
	T sort ( );
	void remove_prefix ( const T prefix );
	
	std::vector<T> get_values ( void ) { return values; }
	T get_as_comma_separated ( void );
	T get_as_filemaker_string ( void );
	
protected:
	
	std::vector<T> values;
	
};



typedef std::auto_ptr<BEValueList<std::string> > BEValueListStringAutoPtr;
typedef std::auto_ptr<BEValueList<std::wstring> > BEValueListWideStringAutoPtr;



template <typename T>
BEValueList<T>::BEValueList ( void )
{
	values.clear();
}


template <typename T>
BEValueList<T>::BEValueList ( const T value_list )
{
	boost::split ( values, value_list, boost::is_any_of ( FILEMAKER_END_OF_LINE ), boost::token_compress_on );
}


template <typename T>
BEValueList<T>::BEValueList ( const std::vector<T> value_list )
{
	values = value_list;
}


template <typename T>
bool BEValueList<T>::not_empty ( )
{
	return !values.empty();
}


template <typename T>
size_t BEValueList<T>::size ( ) const
{
	return values.size();
}


template <typename T>
void BEValueList<T>::append ( BEValueList<T> to_append )
{
	const std::vector<T> new_values = to_append.get_values();
	values.insert ( values.end(), new_values.begin(), new_values.end() );
}


template <typename T>
void BEValueList<T>::append ( const T to_append )
{
	values.push_back ( to_append );
}


template <typename T>
T BEValueList<T>::first ( ) const
{
	return values.front();
};


template <typename T>
T BEValueList<T>::at ( const size_t which ) const
{
	return values.at ( which );
}


template <typename T>
T BEValueList<T>::unique ( )
{
	
	std::set<T> unique;
	std::stringstream text;
	
	for ( typename std::vector<T>::iterator it = values.begin() ; it != values.end(); ++it ) {
		
		std::pair<typename std::set<T>::iterator, bool> inserted = unique.insert ( *it );
		
		if ( inserted.second == true ) {
			
			if ( text.rdbuf()->in_avail() > 0 ) {
				text << FILEMAKER_END_OF_LINE;
			}
			text << *it;
		}
		
	}
	
	return text.str();
	
} // unique


template <typename T>
T BEValueList<T>::filter_out ( std::auto_ptr<BEValueList> filter_out )
{
	
	const std::vector<T> to_filter = filter_out->get_values();
	std::set<T> filter_set ( to_filter.begin(), to_filter.end() );
	
	std::stringstream text;
	
	for ( typename std::vector<T>::iterator it = values.begin() ; it != values.end(); ++it ) {
		
		std::pair<typename std::set<T>::iterator, bool> inserted = filter_set.insert ( *it );
		
		if ( inserted.second == true ) {
			
			if ( text.rdbuf()->in_avail() > 0 ) {
				text << FILEMAKER_END_OF_LINE;
			}
			text << *it;
			filter_set.erase ( *it );
		}
		
	}
	
	return text.str();
	
} // filter_out


template <typename T>
T BEValueList<T>::sort ( ) {
	
	std::vector<T> sorted ( values.begin(), values.end() );
	std::sort ( sorted.begin(), sorted.end() );
	
	std::stringstream text;
	
	for ( typename std::vector<T>::iterator it = sorted.begin() ; it != sorted.end(); ++it ) {
		
		if ( text.rdbuf()->in_avail() > 0 ) {
			text << FILEMAKER_END_OF_LINE;
		}
		text << *it;
		
	}
	
	return text.str();
	
} // sort


template <typename T>
void BEValueList<T>::remove_prefix ( const T prefix ) {
	
	for ( typename std::vector<T>::iterator it = values.begin() ; it != values.end(); ++it ) {
		
		size_t where = it->find ( prefix );
		if ( where == 0 ) {
			it->erase ( 0, prefix.size() );
		}
	}
	
} // remove_prefix


template <typename T>
T BEValueList<T>::get_as_comma_separated ( void )
{
	return boost::algorithm::join ( values, "," );
}


template <typename T>
T BEValueList<T>::get_as_filemaker_string ( void )
{
	return boost::algorithm::join ( values, FILEMAKER_END_OF_LINE );
}



#endif /* defined(__BaseElements__BEValueList__) */
