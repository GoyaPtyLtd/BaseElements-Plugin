/*
 
 Copyright © 1998 - 2019  Claris International Inc.
 All rights reserved.
 
 Claris International Inc. grants you a non-exclusive limited license to use this file solely
 to enable licensees of Claris FileMaker Pro to compile plug-ins for use with Claris products.
 Redistribution and use in source and binary forms, without modification, are permitted provided
 that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of
 conditions and the following disclaimer.
 
 * The name Claris International Inc. may not be used to endorse or promote products derived
 from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY CLARIS INTERNATIONAL INC. ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL FILEMAKER, INC. BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 */

#ifndef _h_DateTime_
#define _h_DateTime_

#include "FMWrapper/FMXClient.h"
#include "FMWrapper/FMXExtern.h"

extern "C++"
{
	namespace fmx
	{
		class Locale;
		class Text;
		class FixPt;

		class DateTime
		{
		public:
			inline bool operator == ( const DateTime &that ) const;
			inline bool operator != ( const DateTime &that ) const;
			inline bool IsLeapYear () const;
			inline int16 DayOfWeek () const;
			inline int16 DayOfYear () const;
			inline int16 WeekOfYear () const;
			inline void Now ();
			inline void SetDate ( const DateTime &datetime );
			inline errcode SetNormalizedDate ( int16 month, int16 day, int16 year );
			inline errcode SetNormalizedDate ( const FixPt &year, const FixPt &month, const FixPt &day );
			inline void SetDaysSinceEpoch ( int64 days );
			inline void SetTime ( const DateTime &datetime );
			inline errcode SetNormalizedTime ( int64 hour, int16 minute, int16 sec, int32 usec = 0 );
			inline errcode SetNormalizedTime ( const FixPt &hour, const FixPt &minute, const FixPt &sec );
			inline void SetSecsSinceMidnight ( const FixPt &secs );
			inline void SetSecondsSinceEpoch ( const FixPt &seconds );
			inline int16 GetYear () const;
			inline int16 GetMonth () const;
			inline int16 GetDay () const;
			inline int32 GetDaysSinceEpoch () const;
			inline int32 GetHour () const;
			inline int16 GetMinute () const;
			inline int16 GetSec () const;
			inline int32 GetUSec () const;
			inline void GetSeconds ( FixPt &results ) const;
			inline void GetSecsSinceMidnight ( FixPt &results ) const;
			inline void GetSecondsSinceEpoch ( FixPt &results ) const;

			inline void operator delete ( void *obj );

		private:
			DateTime ();
			DateTime ( const DateTime &other );

		};

#if FMX_USE_AUTO_PTR
		// DEPRECATED in FileMaker Pro 15. C++11 deprecated std::auto_ptr and replaced with std::unique_ptr.
		class DEPRECATED DateTimeAutoPtr : public std::auto_ptr<DateTime>
		{
			typedef DateTimeAutoPtr     UpCaster;
		public:
			inline DateTimeAutoPtr ();
			inline DateTimeAutoPtr ( const unichar16* dateString, uint32 dateLength, const Locale& intl );
			inline DateTimeAutoPtr ( const Text &dateText, const Locale &intl );
		};
#endif
		
#if FMX_USE_UNIQUE_PTR
		class DateTimeUniquePtr : public std::unique_ptr<DateTime>
		{
			typedef DateTimeUniquePtr     UpCaster;
		public:
			inline DateTimeUniquePtr ();
			inline DateTimeUniquePtr ( const unichar16* dateString, uint32 dateLength, const Locale& intl );
			inline DateTimeUniquePtr ( const Text &dateText, const Locale &intl );
		};
#endif
	}
}


// These functions are only useful for C-only environments or for porting to other languages (ex., Pascal).
extern "C"
{

	fmx::DateTime FMX_API *FM_DateTime_Constructor1 ( fmx::_fmxcpt &_x ) throw ();
	fmx::DateTime FMX_API *FM_DateTime_Constructor2 ( const fmx::unichar16* dateString, fmx::uint32 dateLength, const fmx::Locale& intl, fmx::_fmxcpt &_x ) throw ();
	fmx::DateTime FMX_API *FM_DateTime_Constructor3 ( const fmx::Text &dateText, const fmx::Locale &intl, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_DateTime_operatorEQ ( const void *_self, const fmx::DateTime &that, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_DateTime_operatorNE ( const void *_self, const fmx::DateTime &that, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_DateTime_IsLeapYear ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int16 FMX_API FM_DateTime_DayOfWeek ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int16 FMX_API FM_DateTime_DayOfYear ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int16 FMX_API FM_DateTime_WeekOfYear ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_Now ( void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_SetDate ( void *_self, const fmx::DateTime &datetime, fmx::_fmxcpt &_x ) throw ();
	fmx::errcode FMX_API FM_DateTime_SetNormalizedDate1 ( void *_self, fmx::int16 month, fmx::int16 day, fmx::int16 year, fmx::_fmxcpt &_x ) throw ();
	fmx::errcode FMX_API FM_DateTime_SetNormalizedDate2 ( void *_self, const fmx::FixPt &year, const fmx::FixPt &month, const fmx::FixPt &day, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_SetDaysSinceEpoch ( void *_self, fmx::int64 days, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_SetTime ( void *_self, const fmx::DateTime &datetime, fmx::_fmxcpt &_x ) throw ();
	fmx::errcode FMX_API FM_DateTime_SetNormalizedTime1 ( void *_self, fmx::int64 hour, fmx::int16 minute, fmx::int16 sec, fmx::int32 usec, fmx::_fmxcpt &_x ) throw ();
	fmx::errcode FMX_API FM_DateTime_SetNormalizedTime2 ( void *_self, const fmx::FixPt &hour, const fmx::FixPt &minute, const fmx::FixPt &sec, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_SetSecsSinceMidnight ( void *_self, const fmx::FixPt &secs, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_SetSecondsSinceEpoch ( void *_self, const fmx::FixPt &seconds, fmx::_fmxcpt &_x ) throw ();
	fmx::int16 FMX_API FM_DateTime_GetYear ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int16 FMX_API FM_DateTime_GetMonth ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int16 FMX_API FM_DateTime_GetDay ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int32 FMX_API FM_DateTime_GetDaysSinceEpoch ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int32 FMX_API FM_DateTime_GetHour ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int16 FMX_API FM_DateTime_GetMinute ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int16 FMX_API FM_DateTime_GetSec ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::int32 FMX_API FM_DateTime_GetUSec ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_GetSeconds ( const void *_self, fmx::FixPt &results, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_GetSecsSinceMidnight ( const void *_self, fmx::FixPt &results, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_GetSecondsSinceEpoch ( const void *_self, fmx::FixPt &results, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_DateTime_Delete ( void *_self, fmx::_fmxcpt &_x ) throw ();

}


extern "C++"
{
	namespace fmx
	{
		inline bool DateTime::operator == ( const DateTime &that ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_DateTime_operatorEQ ( this, that, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool DateTime::operator != ( const DateTime &that ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_DateTime_operatorNE ( this, that, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool DateTime::IsLeapYear () const
		{
			_fmxcpt _x;
			bool _rtn = FM_DateTime_IsLeapYear ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int16 DateTime::DayOfWeek () const
		{
			_fmxcpt _x;
			int16 _rtn = FM_DateTime_DayOfWeek ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int16 DateTime::DayOfYear () const
		{
			_fmxcpt _x;
			int16 _rtn = FM_DateTime_DayOfYear ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int16 DateTime::WeekOfYear () const
		{
			_fmxcpt _x;
			int16 _rtn = FM_DateTime_WeekOfYear ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline void DateTime::Now ()
		{
			_fmxcpt _x;
			FM_DateTime_Now ( this, _x );
			_x.Check ();
		}
		inline void DateTime::SetDate ( const DateTime &datetime )
		{
			_fmxcpt _x;
			FM_DateTime_SetDate ( this, datetime, _x );
			_x.Check ();
		}
		inline errcode DateTime::SetNormalizedDate ( int16 month, int16 day, int16 year )
		{
			_fmxcpt _x;
			errcode _rtn = FM_DateTime_SetNormalizedDate1 ( this, month, day, year, _x );
			_x.Check ();
			return _rtn;
		}
		inline errcode DateTime::SetNormalizedDate ( const FixPt &year, const FixPt &month, const FixPt &day )
		{
			_fmxcpt _x;
			errcode _rtn = FM_DateTime_SetNormalizedDate2 ( this, year, month, day, _x );
			_x.Check ();
			return _rtn;
		}
		inline void DateTime::SetDaysSinceEpoch ( int64 days )
		{
			_fmxcpt _x;
			FM_DateTime_SetDaysSinceEpoch ( this, days, _x );
			_x.Check ();
		}
		inline void DateTime::SetTime ( const DateTime &datetime )
		{
			_fmxcpt _x;
			FM_DateTime_SetTime ( this, datetime, _x );
			_x.Check ();
		}
		inline errcode DateTime::SetNormalizedTime ( int64 hour, int16 minute, int16 sec, int32 usec )
		{
			_fmxcpt _x;
			errcode _rtn = FM_DateTime_SetNormalizedTime1 ( this, hour, minute, sec, usec, _x );
			_x.Check ();
			return _rtn;
		}
		inline errcode DateTime::SetNormalizedTime ( const FixPt &hour, const FixPt &minute, const FixPt &sec )
		{
			_fmxcpt _x;
			errcode _rtn = FM_DateTime_SetNormalizedTime2 ( this, hour, minute, sec, _x );
			_x.Check ();
			return _rtn;
		}
		inline void DateTime::SetSecsSinceMidnight ( const FixPt &secs )
		{
			_fmxcpt _x;
			FM_DateTime_SetSecsSinceMidnight ( this, secs, _x );
			_x.Check ();
		}
		inline void DateTime::SetSecondsSinceEpoch ( const FixPt &seconds )
		{
			_fmxcpt _x;
			FM_DateTime_SetSecondsSinceEpoch ( this, seconds, _x );
			_x.Check ();
		}
		inline int16 DateTime::GetYear () const
		{
			_fmxcpt _x;
			int16 _rtn = FM_DateTime_GetYear ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int16 DateTime::GetMonth () const
		{
			_fmxcpt _x;
			int16 _rtn = FM_DateTime_GetMonth ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int16 DateTime::GetDay () const
		{
			_fmxcpt _x;
			int16 _rtn = FM_DateTime_GetDay ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int32 DateTime::GetDaysSinceEpoch () const
		{
			_fmxcpt _x;
			int32 _rtn = FM_DateTime_GetDaysSinceEpoch ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int32 DateTime::GetHour () const
		{
			_fmxcpt _x;
			int32 _rtn = FM_DateTime_GetHour ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int16 DateTime::GetMinute () const
		{
			_fmxcpt _x;
			int16 _rtn = FM_DateTime_GetMinute ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int16 DateTime::GetSec () const
		{
			_fmxcpt _x;
			int16 _rtn = FM_DateTime_GetSec ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline int32 DateTime::GetUSec () const
		{
			_fmxcpt _x;
			int32 _rtn = FM_DateTime_GetUSec ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline void DateTime::GetSeconds ( FixPt &results ) const
		{
			_fmxcpt _x;
			FM_DateTime_GetSeconds ( this, results, _x );
			_x.Check ();
		}
		inline void DateTime::GetSecsSinceMidnight ( FixPt &results ) const
		{
			_fmxcpt _x;
			FM_DateTime_GetSecsSinceMidnight ( this, results, _x );
			_x.Check ();
		}
		inline void DateTime::GetSecondsSinceEpoch ( FixPt &results ) const
		{
			_fmxcpt _x;
			FM_DateTime_GetSecondsSinceEpoch ( this, results, _x );
			_x.Check ();
		}
		inline void DateTime::operator delete ( void *obj )
		{
			_fmxcpt _x;
			FM_DateTime_Delete ( obj, _x );
			_x.Check ();
		}

#if FMX_USE_AUTO_PTR
		inline DateTimeAutoPtr::DateTimeAutoPtr ()
		{
			_fmxcpt _x;
			reset ( FM_DateTime_Constructor1 ( _x ) );
			_x.Check ();
		}
		inline DateTimeAutoPtr::DateTimeAutoPtr ( const unichar16* dateString, uint32 dateLength, const Locale& intl )
		{
			_fmxcpt _x;
			reset ( FM_DateTime_Constructor2 ( dateString, dateLength, intl, _x ) );
			_x.Check ();
		}
		inline DateTimeAutoPtr::DateTimeAutoPtr ( const Text &dateText, const Locale &intl )
		{
			_fmxcpt _x;
			reset ( FM_DateTime_Constructor3 ( dateText, intl, _x ) );
			_x.Check ();
		}
#endif
		
#if FMX_USE_UNIQUE_PTR
		inline DateTimeUniquePtr::DateTimeUniquePtr ()
		{
			_fmxcpt _x;
			reset ( FM_DateTime_Constructor1 ( _x ) );
			_x.Check ();
		}
		inline DateTimeUniquePtr::DateTimeUniquePtr ( const unichar16* dateString, uint32 dateLength, const Locale& intl )
		{
			_fmxcpt _x;
			reset ( FM_DateTime_Constructor2 ( dateString, dateLength, intl, _x ) );
			_x.Check ();
		}
		inline DateTimeUniquePtr::DateTimeUniquePtr ( const Text &dateText, const Locale &intl )
		{
			_fmxcpt _x;
			reset ( FM_DateTime_Constructor3 ( dateText, intl, _x ) );
			_x.Check ();
		}
#endif
		
	}
}


#endif /* _h_DateTime_ */
