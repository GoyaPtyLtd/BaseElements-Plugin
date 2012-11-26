/*
 BEShell.cpp
 BaseElements Plug-In
 
 Copyright 2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEPluginUtilities.h"
#include "BETime.h"



fmx::int64 ptime_to_milliseconds ( const boost::posix_time::ptime& the_time )
{
	// filemaker doesn't do epoch time and and boost::gregorian::date doesn't do prior to 1400
	
	const boost::posix_time::ptime epoch ( boost::gregorian::date ( 1970, boost::gregorian::Jan, 1 ) );
	const boost::posix_time::time_duration epoch_time = the_time - epoch;
	
	return epoch_time.total_milliseconds() + ( FILEMAKER_EPOCH_OFFSET * 1000 );
}


