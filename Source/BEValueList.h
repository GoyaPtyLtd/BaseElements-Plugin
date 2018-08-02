/*
 BEValueList.h
 BaseElements Plug-In
 
 Copyright 2013-2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef __BaseElements__BEValueList__
#define __BaseElements__BEValueList__


#include "BEPluginGlobalDefines.h"
#include "BERegularExpression.h"

#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <map>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>


//template <typename T>
//bool numeric_sort ( const T i, const T j );


template<typename T>
inline long double convert_to_double ( const T& s )
{
	std::istringstream i ( s );
	long double x;
	if ( !(i >> x) ) {
		x = 0;
	}
	
	return x;
}


template<typename T>
bool numeric_sort ( const T i, const T j )
{
	const long double ii = convert_to_double ( i );
	const long double jj = convert_to_double ( j );
	
	return ( ii < jj );
}




template <typename T>

class BEValueList {
	
	
public:
	
	BEValueList ( void );
	BEValueList ( const T& value_list, bool is_case_sensitive = true, bool retain_empty_values = false );
	BEValueList ( const std::vector<T>& value_list, bool is_case_sensitive = true );
	BEValueList ( const T& value_list, const T& delimiter, const bool is_case_sensitive = true, bool retain_empty_values = false );

	bool not_empty ( );
	size_t size ( ) const;
	
	void clear ( void );
	
	void append ( BEValueList& to_append );
	void append ( const T& to_append );
	
	T first ( ) const;
	T at ( const size_t which ) const;

	T starts_with ( const T prefix ) const;
	BEValueList<T> find ( const T find_this ) const;
	bool change_value ( const size_t which, const T new_value );
	
	T unique ( );
	T filter_out ( BEValueList& filter_out );
	const bool contains_duplicates ( );
	BEValueList<T> times_duplicated ( const long numberOfDuplicates ) const;
	T sort ( const bool ascending = true, const bool numeric = false ) const;
	void remove_prefix ( const T& prefix );
	
	std::vector<T> get_values ( void ) const { return values; }
	T get_as_comma_separated ( void ) const;
	T get_as_filemaker_string ( void ) const;
	std::vector<double> get_as_vector_double ( void ) const;

	BEValueList<T> apply_regular_expression ( const T expression, const std::string options, const T replace_with, const bool replace = false ) const;

	void trim_values ( );

protected:
	
	std::vector<T> values;
	bool case_sensitive;
	
	bool inserted ( T value, typename std::set<T>& values_wanted );
	
};



typedef std::unique_ptr<BEValueList<std::string> > BEValueListStringUniquePtr;
typedef std::unique_ptr<BEValueList<std::wstring> > BEValueListWideStringUniquePtr;
typedef std::shared_ptr<BEValueList<std::string> > BEValueListStringSharedPtr;


template <typename T>
BEValueList<T>::BEValueList ( void )
{
	clear();
}


template <typename T>
BEValueList<T>::BEValueList ( const T& value_list, const T& delimiter, const bool is_case_sensitive, bool retain_empty_values )
{
	if ( !value_list.empty() ) {
		
		auto token_compress = boost::token_compress_on; // strip empty values
		if ( retain_empty_values ) {
			token_compress = boost::token_compress_off;
		}
		
		boost::split ( values, value_list, boost::is_any_of ( delimiter ), token_compress );
		
	} else {
		BEValueList<T> ( );
	}

	case_sensitive = is_case_sensitive;
}


template <typename T>
BEValueList<T>::BEValueList ( const T& value_list, const bool is_case_sensitive, bool retain_empty_values )
	: BEValueList<T> ( value_list, FILEMAKER_END_OF_LINE, is_case_sensitive, retain_empty_values ) {};


template <typename T>
BEValueList<T>::BEValueList ( const std::vector<T>& value_list, bool is_case_sensitive )
{
	values = value_list;
	case_sensitive = is_case_sensitive;
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
void BEValueList<T>::clear ( void )
{
	values.clear();
	case_sensitive = true;
}


template <typename T>
void BEValueList<T>::append ( BEValueList<T>& to_append )
{
	const std::vector<T> new_values = to_append.get_values();
	values.insert ( values.end(), new_values.begin(), new_values.end() );
}


template <typename T>
void BEValueList<T>::append ( const T& to_append )
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
T BEValueList<T>::starts_with ( const T prefix ) const
{

	BEValueList<T> wanted_values;

	for ( auto it = values.begin() ; it != values.end(); ++it ) {

		if ( boost::istarts_with ( *it, prefix ) == true ) {
			wanted_values.append ( *it );
		}

	}

	wanted_values.remove_prefix ( prefix );

	return wanted_values.get_as_filemaker_string();

} // starts_with


template <typename T>
BEValueList<T> BEValueList<T>::find ( const T find_this ) const
{
	
	BEValueList<T> found_values;
	
	for ( auto it = values.begin() ; it != values.end(); ++it ) {
		
		if ( boost::iequals ( *it, find_this ) == true ) {
			size_t index = std::distance ( values.begin(), it ) + 1;
			found_values.append ( std::to_string ( index ) );
		}
		
	}
	
	return found_values;
	
} // find


template <typename T>
bool BEValueList<T>::change_value ( const size_t which, const T new_value )
{
	auto result = true;
	
	try {
		values.at ( which ) = new_value;
	} catch ( std::out_of_range& /* e */ ) {
		result = false;
	}
	
	return result;
	
} // change_value


template <typename T>
T BEValueList<T>::unique ( )
{
	
	std::set<T> unordered;
	std::vector<T> unique;

	for ( typename std::vector<T>::iterator it = values.begin() ; it != values.end(); ++it ) {
		if ( inserted ( *it, unordered ) ) {
			unique.push_back ( *it );
		}
	}

	return boost::join ( unique, FILEMAKER_END_OF_LINE );

} // unique


template <typename T>
T BEValueList<T>::filter_out ( BEValueList& filter_out )
{
	
	std::vector<T> to_filter = filter_out.get_values();
	
	if ( ! case_sensitive ) {
		for ( typename std::vector<T>::iterator it = to_filter.begin() ; it != to_filter.end(); ++it ) {
			boost::to_lower ( *it );
		}
	}
	
	std::set<T> filter_set ( to_filter.begin(), to_filter.end() );
	
	std::stringstream text;
	
	for ( typename std::vector<T>::iterator it = values.begin() ; it != values.end(); ++it ) {
		
		bool value_inserted = inserted ( *it, filter_set );
		
		if ( value_inserted == true ) {
			
			if ( text.str().size() > 0 ) {
				text << FILEMAKER_END_OF_LINE;
			}
			
			text << *it;
			filter_set.erase ( *it );
		}
		
	}
	
	return text.str();
	
} // filter_out


template <typename T>
const bool BEValueList<T>::contains_duplicates ( )
{
	std::set<T> unique;
	auto contains_duplicates = false;

	for ( typename std::vector<T>::iterator it = values.begin() ; it != values.end(); ++it ) {

		bool value_inserted = inserted ( *it, unique );

		if ( value_inserted == false ) {
			contains_duplicates = true;
			break;
		}

	}

	return contains_duplicates;

} // duplicate_values


template <typename T>
BEValueList<T> BEValueList<T>::times_duplicated ( const long numberOfDuplicates ) const
{
	BEValueList<T> duplicates;
	std::map<T,int> count_of_duplicates;
	
	for ( auto it = values.begin() ; it != values.end(); ++it ) {
		count_of_duplicates[*it]++;
	}
	
	for ( auto it = count_of_duplicates.begin() ; it != count_of_duplicates.end(); ++it ) {
		
		if ( it->second == numberOfDuplicates ) {
			duplicates.append ( it->first );
		}
		
	}
	
	return duplicates;
	
} // duplicate_values


template <typename T>
T BEValueList<T>::sort ( const bool ascending, const bool numeric ) const
{

	std::vector<T> sorted ( values.begin(), values.end() );
	
	if ( ! numeric ) {
		
		if ( ascending ) {
			std::sort ( sorted.begin(), sorted.end() );
		} else {
			std::sort ( sorted.rbegin(), sorted.rend() );
		}

	} else {
		
		if ( ascending ) {
			std::sort ( sorted.begin(), sorted.end(), numeric_sort<T> );
		} else {
			std::sort ( sorted.rbegin(), sorted.rend(), numeric_sort<T> );
		}

	}

	std::stringstream text;

	for ( typename std::vector<T>::iterator it = sorted.begin() ; it != sorted.end(); ++it ) {

		if ( text.str().size() > 0 ) {
			text << FILEMAKER_END_OF_LINE;
		}
		text << *it;

	}

	return text.str();

} // sort


template <typename T>
void BEValueList<T>::remove_prefix ( const T& prefix )
{
	
	for ( typename std::vector<T>::iterator it = values.begin() ; it != values.end(); ++it ) {
		
		size_t where = it->find ( prefix );
		if ( where == 0 ) {
			it->erase ( 0, prefix.size() );
		}
	}
	
} // remove_prefix


template <typename T>
T BEValueList<T>::get_as_comma_separated ( void ) const
{
	return boost::algorithm::join ( values, "," );
}


template <typename T>
T BEValueList<T>::get_as_filemaker_string ( void ) const
{
	return boost::algorithm::join ( values, FILEMAKER_END_OF_LINE );
}


template <typename T>
std::vector<double>  BEValueList<T>::get_as_vector_double ( void ) const
{
	std::vector<double> out;
	
	for ( auto it = values.begin() ; it != values.end(); ++it ) {
		
		double number;
		std::stringstream convert ( *it );
		if ( !(convert >> number) ) {
			number = 0; // if that fails set number to 0
		}
		
		out.push_back ( number );
	}
	
	return out;
}


template <typename T>
BEValueList<T> BEValueList<T>::apply_regular_expression ( const T expression, const std::string options, const T replace_with, const bool replace ) const
{
    BEValueList<T> matched;
    
    for ( auto it = values.begin() ; it != values.end(); ++it ) {
        
        matched.append ( regular_expression ( *it, expression, options, replace_with, replace ) );
    }

    return matched;
}


template <typename T>
void BEValueList<T>::trim_values ( )
{
	for_each ( values.begin(), values.end(), boost::bind ( &boost::trim<T>, _1, std::locale() ) );
}



#pragma mark -
#pragma mark Protected
#pragma mark

template<typename T>
bool BEValueList<T>::inserted ( T value, typename std::set<T>& values_wanted )
{
	
	if ( ! case_sensitive ) {
		boost::to_lower ( value );
	}
	
	std::pair<typename std::set<T>::iterator, bool> was_inserted = values_wanted.insert ( value );
	
	return was_inserted.second;
	
}



#endif /* defined(__BaseElements__BEValueList__) */
