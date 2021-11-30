/*
 BEShell.cpp
 BaseElements Plug-In
 
 Copyright 2012-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"
#include "BETime.h"


#if defined ( FMX_WIN_TARGET )

	#include <ctime>
	#define timegm _mkgmtime

#endif


#define FILEMAKER_EPOCH_OFFSET 62135596800


const fmx::int64 ptime_to_milliseconds ( const boost::posix_time::ptime& the_time )
{
	// filemaker doesn't do epoch time and and boost::gregorian::date doesn't do prior to 1400
	
	const boost::posix_time::ptime epoch ( boost::gregorian::date ( 1970, boost::gregorian::Jan, 1 ) );
	const boost::posix_time::time_duration epoch_time = the_time - epoch;
	
	return epoch_time.total_milliseconds() + ( FILEMAKER_EPOCH_OFFSET * 1000 );
}



const fmx::int64 std_time_to_timestamp ( const std::time_t the_time )
{
	const std::tm * time_struct = std::localtime ( &the_time );
	const std::time_t localtime = timegm ( (std::tm *)time_struct );
	const fmx::int64 timestamp = localtime + FILEMAKER_EPOCH_OFFSET;
	
	return timestamp;
}


const time_t timestamp_to_epoch ( const time_t timestamp )
{
	const auto local_time = ptime_to_milliseconds ( boost::posix_time::microsec_clock::local_time() );
	const auto utc_time = ptime_to_milliseconds ( boost::posix_time::microsec_clock::universal_time() );
	const time_t utc_offset_seconds = ( local_time - utc_time ) / 1000;
	const auto epoch_time = timestamp - FILEMAKER_EPOCH_OFFSET - utc_offset_seconds;

	return epoch_time;
}


