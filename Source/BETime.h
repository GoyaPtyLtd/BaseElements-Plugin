/*
 BETime.h
 BaseElements Plug-In
 
 Copyright 2012 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#ifndef __BaseElements__BETime__
#define __BaseElements__BETime__


#include "boost/date_time/posix_time/posix_time.hpp"


fmx::int64 ptime_to_milliseconds ( const boost::posix_time::ptime& the_time );



#endif /* defined(__BaseElements__BETime__) */
