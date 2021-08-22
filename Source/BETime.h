/*
 BETime.h
 BaseElements Plug-In
 
 Copyright 2012-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#ifndef __BaseElements__BETime__
#define __BaseElements__BETime__


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomma"
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma GCC diagnostic pop


const fmx::int64 ptime_to_milliseconds ( const boost::posix_time::ptime& the_time );
const fmx::int64 std_time_to_timestamp ( const std::time_t the_time );



#endif /* defined(__BaseElements__BETime__) */
