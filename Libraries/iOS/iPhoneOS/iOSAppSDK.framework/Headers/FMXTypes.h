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

#ifndef _h_Types_
#define _h_Types_

#include "FMWrapper/FMXClient.h"

#ifndef TRUE
	#define TRUE    1
#endif

#ifndef FALSE
	#define FALSE   0
#endif

extern "C++"
{
	namespace fmx
	{
		class QuadChar
		{
		public:
			inline const QuadChar &operator = ( const QuadChar &value );
			inline uchar operator [] ( int i );
			inline const uchar operator [] ( int i ) const;
			inline bool operator == ( const QuadChar &value ) const;
			inline bool operator != ( const QuadChar &value ) const;
			inline bool operator < ( const QuadChar &value ) const;
			inline bool operator <= ( const QuadChar &value ) const;
			inline bool operator > ( const QuadChar &value ) const;
			inline bool operator >= ( const QuadChar &value ) const;
			inline uint32 GetMacType () const;
			inline void SetMacType ( uint32 value );
			inline void operator delete ( void *obj );

		private:
			QuadChar ();
			QuadChar ( const QuadChar &value );

		};

		class Locale
		{
		public:

			enum Type
			{
				kType_None              = 0,    // Empty
				kType_System            = 1,    // Uses system settings
				kType_UnicodeRaw        = 2,    // Use raw unicode bytes (like the old ASCII ordering)
				kType_UnicodeStandard   = 3,    // Standard unicode rules
				kType_Catalog           = 4,    // FileMaker list rules
				kType_CAT               = 16,   // Catalan
				kType_HRV               = 17,   // Croatian
				kType_CES               = 18,   // Czech
				kType_DAN               = 19,   // Danish
				kType_NLD               = 20,   // Dutch
				kType_ENG               = 21,   // English
				kType_FIN               = 22,   // Finnish
				kType_FIN_fmi           = 23,   // Finnish (FileMaker custom)
				kType_FRA               = 24,   // French
				kType_DEU               = 25,   // German
				kType_DEU_dictionary    = 26,   // German (dictionary ordering)
				kType_ELL               = 27,   // Greek
				kType_HUN               = 28,   // Hungarian
				kType_ISL               = 29,   // Icelandic
				kType_ITA               = 30,   // Italian
				kType_JPN               = 31,   // Japanese
				kType_NOR               = 32,   // Norwegian
				kType_POL               = 33,   // Polish
				kType_POR               = 34,   // Portuguese
				kType_RON               = 35,   // Romanian
				kType_RUS               = 36,   // Russian
				kType_SLK               = 37,   // Slovak
				kType_SLV               = 38,   // Slovenian
				kType_SPA               = 39,   // Spanish
				kType_SPA_traditional   = 40,   // Spanish (traditional)
				kType_SWE               = 41,   // Swedish
				kType_SWE_fmi           = 42,   // Swedish (FileMaker custom)
				kType_TUR               = 43,   // Turkish
				kType_UKR               = 44,   // Ukrainian

				// New to FileMaker Pro 8.5
				kType_CHI               = 45,   // Chinese (PinYin)
				kType_CHI_stroke        = 46,   // Chinese (Stroke-radical)

				// New to FileMaker Pro 13
				kType_KOR               = 76,   // Korean
				
				// For compatibility with WinSoft versions (supported in FMI versions >= 12)
				kType_HE                = 47,   // Hebrew
				kType_HI                = 48,   // Hindi
				kType_AR                = 49,   // Arabic
				kType_ET                = 50,   // Estonian
				kType_LT                = 51,   // Lithuanian
				kType_LV                = 52,   // Latvian
				kType_SR                = 53,   // Serbian
				kType_FA                = 54,   // Persian
				kType_BG                = 55,   // Bulgarian
				kType_VI                = 56,   // Vietnamese
				kType_TH                = 57,   // Thai
				kType_ELL_Mixed         = 58,   // Greek Mixed
				kType_BEN               = 59,   // Bengali
				kType_TEL               = 60,   // Telugu
				kType_MAR               = 61,   // Marathi
				kType_TAM               = 62,   // Tamil
				kType_GUJ               = 63,   // Gujarati
				kType_KAN               = 64,   // Kannada
				kType_MAL               = 65,   // Malayalam
				kType_PAN               = 67,   // Panjabi

				// Used in versions distributed by WinSoft (not supported in FMI versions)
				kType_ORI               = 66,   // Oriya
				kType_SIN               = 68,   // Sinhala
				kType_URD               = 69,   // Urdu
				kType_DIV               = 70,   // Divehi (Thanaa)
				kType_BUR               = 71,   // Burmese (Myanmar)
				kType_SAN               = 72,   // Sanskrit
				kType_LAO               = 73,   // Lao
				kType_KHM               = 74,   // Khmer
				kType_BOD               = 75,   // Tibetan

				kType_Invalid           = 0xFFFF
		   };

			inline Locale &operator = ( const Locale &rhs );
			inline void operator delete ( void *obj );

		private:
			inline Locale ( Type inputType );
			inline Locale ( const Locale &copyConstruct );

		};

#if FMX_USE_AUTO_PTR
		// DEPRECATED in FileMaker Pro 15. C++11 deprecated std::auto_ptr and replaced with std::unique_ptr.
		class DEPRECATED QuadCharAutoPtr : public std::auto_ptr<QuadChar>
		{
						typedef QuadCharAutoPtr UpCaster;
		public:
			inline QuadCharAutoPtr ();
			inline QuadCharAutoPtr ( char c0, char c1, char c2, char c3 );
			inline QuadCharAutoPtr ( const QuadChar &value );

		};

		// DEPRECATED in FileMaker Pro 15. C++11 deprecated std::auto_ptr and replaced with std::unique_ptr.
		class DEPRECATED LocaleAutoPtr : public std::auto_ptr<Locale>
		{
						typedef LocaleAutoPtr   UpCaster;
		public:
			inline LocaleAutoPtr ( Locale::Type inputType = Locale::kType_System );
			inline LocaleAutoPtr ( const Locale &copyConstruct );

		};
#endif
		
#if FMX_USE_UNIQUE_PTR
		class QuadCharUniquePtr : public std::unique_ptr<QuadChar>
		{
			typedef QuadCharUniquePtr UpCaster;
		public:
			inline QuadCharUniquePtr ();
			inline QuadCharUniquePtr ( char c0, char c1, char c2, char c3 );
			inline QuadCharUniquePtr ( const QuadChar &value );
			
		};
		
		class LocaleUniquePtr : public std::unique_ptr<Locale>
		{
			typedef LocaleUniquePtr   UpCaster;
		public:
			inline LocaleUniquePtr ( Locale::Type inputType = Locale::kType_System );
			inline LocaleUniquePtr ( const Locale &copyConstruct );
			
		};
#endif
	}
}


// These functions are only useful for C-only environments or for porting to other languages (ex., Pascal).
extern "C"
{
	fmx::QuadChar FMX_API *FM_QuadChar_Constructor1 ( fmx::_fmxcpt &_x ) throw ();
	fmx::QuadChar FMX_API *FM_QuadChar_Constructor2 ( char c0, char c1, char c2, char c3, fmx::_fmxcpt &_x ) throw ();
	fmx::QuadChar FMX_API *FM_QuadChar_Constructor3 ( const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
	const fmx::QuadChar FMX_API *FM_QuadChar_operatorAS ( void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
		fmx::uchar FMX_API FM_QuadChar_operatorAR ( void *_self, int i, fmx::_fmxcpt &_x ) throw ();
	const fmx::uchar FMX_API FM_QuadChar_operatorCAR ( const void *_self, int i, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_QuadChar_operatorEQ ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_QuadChar_operatorNE ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_QuadChar_operatorLT ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_QuadChar_operatorLE ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_QuadChar_operatorGT ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_QuadChar_operatorGE ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
	fmx::uint32 FMX_API FM_QuadChar_GetMacType ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_QuadChar_SetMacType ( void *_self, fmx::uint32 value, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_QuadChar_Delete ( void *_self, fmx::_fmxcpt &_x ) throw ();

	fmx::Locale FMX_API *FM_Locale_Constructor1 ( fmx::int32 inputType, fmx::_fmxcpt &_x ) throw ();
	fmx::Locale FMX_API *FM_Locale_Constructor2 ( const fmx::Locale &copyConstruct, fmx::_fmxcpt &_x ) throw ();
	fmx::Locale FMX_API *FM_Locale_operatorAS ( void *_self, const fmx::Locale &rhs, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Locale_Delete ( void *_self, fmx::_fmxcpt &_x ) throw ();

}


extern "C++"
{
	namespace fmx
	{           
		inline const QuadChar &QuadChar::operator = ( const QuadChar &value )
		{
			_fmxcpt _x;
			const QuadChar *_rtn = FM_QuadChar_operatorAS ( this, value, _x );
			_x.Check ();
			return *_rtn;
		}
		inline uchar QuadChar::operator [] ( int i )
		{
			_fmxcpt _x;
			uchar _rtn = FM_QuadChar_operatorAR ( this, i, _x );
			_x.Check ();
			return _rtn;
		}
		inline const uchar QuadChar::operator [] ( int i ) const
		{
			_fmxcpt _x;
			const uchar _rtn = FM_QuadChar_operatorCAR ( this, i, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool QuadChar::operator == ( const QuadChar &value ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_QuadChar_operatorEQ ( this, value, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool QuadChar::operator != ( const QuadChar &value ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_QuadChar_operatorNE ( this, value, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool QuadChar::operator < ( const QuadChar &value ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_QuadChar_operatorLT ( this, value, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool QuadChar::operator <= ( const QuadChar &value ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_QuadChar_operatorLE ( this, value, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool QuadChar::operator > ( const QuadChar &value ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_QuadChar_operatorGT ( this, value, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool QuadChar::operator >= ( const QuadChar &value ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_QuadChar_operatorGE ( this, value, _x );
			_x.Check ();
			return _rtn;
		}
		inline uint32 QuadChar::GetMacType () const
		{
			_fmxcpt _x;
			uint32 _rtn = FM_QuadChar_GetMacType ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline void QuadChar::SetMacType ( uint32 value )
		{
			_fmxcpt _x;
			FM_QuadChar_SetMacType ( this, value, _x );
			_x.Check ();
		}
		inline void QuadChar::operator delete ( void *obj )
		{
			_fmxcpt _x;
			FM_QuadChar_Delete ( obj, _x );
			_x.Check ();
		}

#if FMX_USE_AUTO_PTR
		inline QuadCharAutoPtr::QuadCharAutoPtr ()
		{
			_fmxcpt _x;
			reset ( FM_QuadChar_Constructor1 ( _x ) );
			_x.Check ();
		}
		inline QuadCharAutoPtr::QuadCharAutoPtr ( char c0, char c1, char c2, char c3 )
		{
			_fmxcpt _x;
			reset ( FM_QuadChar_Constructor2 ( c0, c1, c2, c3, _x ) );
			_x.Check ();
		}
		inline QuadCharAutoPtr::QuadCharAutoPtr ( const QuadChar &value )
		{
			_fmxcpt _x;
			reset ( FM_QuadChar_Constructor3 ( value, _x ) );
			_x.Check ();
		}
#endif
		
#if FMX_USE_UNIQUE_PTR
		inline QuadCharUniquePtr::QuadCharUniquePtr ()
		{
			_fmxcpt _x;
			reset ( FM_QuadChar_Constructor1 ( _x ) );
			_x.Check ();
		}
		inline QuadCharUniquePtr::QuadCharUniquePtr ( char c0, char c1, char c2, char c3 )
		{
			_fmxcpt _x;
			reset ( FM_QuadChar_Constructor2 ( c0, c1, c2, c3, _x ) );
			_x.Check ();
		}
		inline QuadCharUniquePtr::QuadCharUniquePtr ( const QuadChar &value )
		{
			_fmxcpt _x;
			reset ( FM_QuadChar_Constructor3 ( value, _x ) );
			_x.Check ();
		}
#endif
		inline Locale &Locale::operator = ( const Locale &rhs )
		{
			_fmxcpt _x;
			Locale *_rtn = FM_Locale_operatorAS ( this, rhs, _x );
			_x.Check ();
			return *_rtn;
		}
		inline void Locale::operator delete ( void *obj )
		{
			_fmxcpt _x;
			FM_Locale_Delete ( obj, _x );
			_x.Check ();
		}

#if FMX_USE_AUTO_PTR
		inline LocaleAutoPtr::LocaleAutoPtr ( Locale::Type inputType )
		{
			_fmxcpt _x;
			reset ( FM_Locale_Constructor1 ( inputType, _x ) );
			_x.Check ();
		}
		inline LocaleAutoPtr::LocaleAutoPtr ( const Locale &copyConstruct )
		{
			_fmxcpt _x;
			reset ( FM_Locale_Constructor2 ( copyConstruct, _x ) );
			_x.Check ();
		}
#endif
		
#if FMX_USE_UNIQUE_PTR
		inline LocaleUniquePtr::LocaleUniquePtr ( Locale::Type inputType )
		{
			_fmxcpt _x;
			reset ( FM_Locale_Constructor1 ( inputType, _x ) );
			_x.Check ();
		}
		inline LocaleUniquePtr::LocaleUniquePtr ( const Locale &copyConstruct )
		{
			_fmxcpt _x;
			reset ( FM_Locale_Constructor2 ( copyConstruct, _x ) );
			_x.Check ();
		}
#endif
	}
}


#endif /* _h_Types_ */
