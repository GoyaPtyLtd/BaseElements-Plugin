/*
 BE_Boost_FileSystem_Additions.hpp
 BaseElements Plug-In
 
 Copyright 2011 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


//	from:	https://svn.boost.org/trac/boost/ticket/1976


#include <boost/filesystem.hpp>


boost::filesystem::path NaiveUncomplete ( const boost::filesystem::path file, const boost::filesystem::path base );



boost::filesystem::path NaiveUncomplete ( const boost::filesystem::path file, const boost::filesystem::path base )
{
    if ( file.has_root_path() ) {
		
        if ( file.root_path() != base.root_path() ) {
            return file;
        } else {
            return NaiveUncomplete ( file.relative_path(), base.relative_path() );
        }

    } else {

        if ( file.has_root_path() ) {
            throw "cannot uncomplete a path relative path from a rooted base";
        } else {

            typedef boost::filesystem::path::const_iterator path_iterator;
            path_iterator path_it = file.begin();
            path_iterator base_it = base.begin();
			
            while ( path_it != file.end() && base_it != base.end() ) {
				
                if ( *path_it != *base_it ) {
					break;
				}
				
                ++path_it; ++base_it;
            
			}
            
			boost::filesystem::path result;
            for ( ; base_it != base.end(); ++base_it ) {
                result /= "..";
            }
			
            for ( ; path_it != file.end(); ++path_it ) {
                result /= *path_it;
            }
			
            return result;
        }
    }
} // NaiveUncomplete
