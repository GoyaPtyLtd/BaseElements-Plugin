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

#ifndef _h_Text_
#define _h_Text_

#include "FMWrapper/FMXClient.h"

extern "C++"
{
	namespace fmx
	{
		class CharacterStyle;

		class Text
		{
		public:

			enum Encoding
			{
				kEncoding_Native,                 // The platform's default
				kEncoding_UTF8,
				kEncoding_ASCII_DOS,              // OEM/DOS
				kEncoding_ASCII_Windows,          // Windows ANSI
				kEncoding_ASCII_Mac,              // Mac Roman
				kEncoding_ISO_8859_1,             // Latin 1
				kEncoding_ShiftJIS_Mac,           // Japanese Shift JIS Mac
				kEncoding_ShiftJIS_Win,           // Japanese Shift JIS Windows
				kEncoding_Korean_Mac,             // Mac Korean
				kEncoding_Korean_Win,             // Windows Korean Unified Hangul (949)
				kEncoding_Korean_Johab,           // Korean Johab (1361)
				kEncoding_ChineseTrad_Mac,        // Mac Traditional Chinese
				kEncoding_ChineseTrad_Win,        // Windows Traditional Chinese (950)
				kEncoding_ChineseSimp_Mac,        // Mac Simplified Chinese
				kEncoding_ChineseSimp_Win,        // Windows Simplified Chinese (936)
				kEncoding_Cyrillic_Mac,           // Mac Russian
				kEncoding_Cyrillic_Win,           // Windows Russian (1251)
				kEncoding_ISO_8859_5,             // ISO Cyrillic
				kEncoding_CentralEurope_Mac,      // Central Europe
				kEncoding_EasternEurope_Win,      // Eastern Europe (1250)
				kEncoding_ISO_8859_2,             // Central Europe
				kEncoding_Turkish_Mac,            // Mac Turkish
				kEncoding_Turkish_Win,            // Windows Turkish (1254)
				kEncoding_ISO_8859_3,             // Latin 3
				kEncoding_ISO_8859_9,             // Latin 5
				kEncoding_Baltic_Win,             // Windows Baltic (1257)
				kEncoding_ISO_8859_4,             // Baltic
				kEncoding_Arabic_Mac,             // Mac Arabic
				kEncoding_Arabic_Win,             // Windows Arabic (1256)
				kEncoding_ISO_8859_6,             // Arabic
				kEncoding_Greek_Mac,              // Mac Greek
				kEncoding_Greek_Win,              // Windows Greek (1253)
				kEncoding_ISO_8859_7,             // Greek
				kEncoding_Hebrew_Mac,             // Mac Hebrew
				kEncoding_Hebrew_Win,             // Windows Hebrew (1255)
				kEncoding_ISO_8859_8,             // Hebrew: Visual Ordering
				kEncoding_ISO_8859_15             // Latin 9
			};

			enum
			{
				kSize_Invalid = 0xFFFFFFFF,
				kSize_End = 0xFFFFFFFF
			};

			inline uint32 GetSize () const;

			// These three methods expect null terminated strings.
			inline void Assign ( const char *s, Encoding encoding = kEncoding_Native );
			inline void AssignWide ( const wchar_t *s );
			inline void AssignUnicode ( const uint16 *s );

			inline void AssignWithLength ( const char *s, uint32 strlength, Encoding encoding = kEncoding_Native );
			inline void AssignWideWithLength ( const wchar_t *s, uint32 strlength );
			inline void AssignUnicodeWithLength ( const uint16 *s, uint32 strlength );

			// Buffer size is in bytes, position and size are in Unicode characters, encoded bytes copied to buffer with a null terminator
			inline void GetBytes ( char *charbuffer, uint32 buffersize, uint32 position = 0, uint32 size = kSize_End, Encoding encoding = kEncoding_Native ) const;

			// Position and size are in Unicode characters, characters copied to unicodebuffer without any terminating null character
			inline void GetUnicode ( uint16 *unicodebuffer, uint32 position, uint32 size ) const;

			inline void SetText ( const Text &other, uint32 position = 0, uint32 size = kSize_End );
			inline void AppendText ( const Text &other, uint32 position = 0, uint32 size = kSize_End );
			inline void InsertText ( const Text &other, uint32 position );
			inline void DeleteText ( uint32 positionToDelete, uint32 sizeToDelete = kSize_End );

			inline uint32 Find ( const Text &other, uint32 position ) const;
			inline uint32 FindPrev ( const Text &other, uint32 position ) const;
			inline uint32 FindIgnoringCase ( const Text &other, uint32 position ) const;
			inline uint32 FindPrevIgnoringCase ( const Text &other, uint32 position ) const;

			inline void Uppercase ();
			inline void Lowercase ();

			inline void GetStyle ( CharacterStyle& style, uint32 position ) const;
			inline void GetDefaultStyle ( CharacterStyle& style ) const;

			// Overwrites any existing CharacterStyle in this Text at position..(position+size)
			inline void SetStyle ( const CharacterStyle& style, uint32 position = 0, uint32 size = kSize_End );

			// Removes the the given style.  This will remove elements of the given style
			// that match exactly with the current styles.  For example, if the given style
			// specifies a size of 12, then styles in the text that are size 12 will be
			// disabled, thus picking up the default style.
			inline void RemoveStyle ( const CharacterStyle& style );

			// This will reset all character and paragraph style information to the default settings
			inline void ResetAllStyleBuffers ();

			// These six operators use the raw unicode values for comparison.
			inline bool operator == ( const Text &that ) const;
			inline bool operator != ( const Text &that ) const;
			inline bool operator < ( const Text &that ) const;
			inline bool operator <= ( const Text &that ) const;
			inline bool operator > ( const Text &that ) const;
			inline bool operator >= ( const Text &that ) const;

			inline void operator delete ( void *obj );

			// New to FileMaker Pro 16 (API VERSION 57) and later
			// Buffer size is in bytes, position and size are in Unicode characters, encoded bytes copied to buffer and number of bytes converted is returned
			// Note that one unicode character can potentially expanded to four UTF8 bytes. Also this function does NOT null terminate the resulting buffer.
			inline uint32 GetBytesEx ( char *charbuffer, uint32 buffersize, uint32 position = 0, uint32 size = kSize_End, Encoding encoding = kEncoding_UTF8 ) const;

		private:
			Text ();
			Text ( const Text &source);

		};

#if FMX_USE_AUTO_PTR
		// DEPRECATED in FileMaker Pro 15. C++11 deprecated std::auto_ptr and replaced with std::unique_ptr.
		class DEPRECATED TextAutoPtr : public std::auto_ptr<Text>
		{
			typedef TextAutoPtr     UpCaster;
		public:
			inline TextAutoPtr ();

		};
#endif
		
#if FMX_USE_UNIQUE_PTR
		class TextUniquePtr : public std::unique_ptr<Text>
		{
			typedef TextUniquePtr     UpCaster;
		public:
			inline TextUniquePtr ();
			
		};
#endif
	}
}


// These functions are only useful for C-only environments or for porting to other languages (ex., Pascal).
extern "C"
{
	fmx::Text FMX_API *FM_Text_Constructor1 ( fmx::_fmxcpt &_x ) throw ();
	fmx::uint32 FMX_API FM_Text_GetSize ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_Assign ( void *_self, const char *s, fmx::int32 encoding, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_GetBytes ( const void *_self, char *buffer, fmx::uint32 buffersize,  fmx::uint32 position, fmx::uint32 size, fmx::int32 encoding, fmx::_fmxcpt &_x ) throw ();
	fmx::uint32 FMX_API FM_Text_GetBytesEx ( const void *_self, char *buffer, fmx::uint32 buffersize,  fmx::uint32 position, fmx::uint32 size, fmx::int32 encoding, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_AssignWide ( void *_self, const wchar_t *s, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_AssignUnicode ( void *_self, const fmx::uint16 *s, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_AssignWithLength ( void *_self, const char *s, fmx::uint32 strlength, fmx::int32 encoding, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_AssignWideWithLength ( void *_self, const wchar_t *s, fmx::uint32 strlength, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_AssignUnicodeWithLength ( void *_self, const fmx::uint16 *s, fmx::uint32 strlength, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_GetUnicode ( const void *_self, fmx::uint16 *s, fmx::uint32 position, fmx::uint32 size, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_SetText ( void *_self, const fmx::Text &other, fmx::uint32 position, fmx::uint32 size, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_AppendText ( void *_self, const fmx::Text &other, fmx::uint32 position, fmx::uint32 size, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_InsertText ( void *_self, const fmx::Text &other, fmx::uint32 position, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_DeleteText ( void *_self, fmx::uint32 positionToDelete, fmx::uint32 sizeToDelete, fmx::_fmxcpt &_x ) throw ();
	fmx::uint32 FMX_API FM_Text_Find ( const void *_self, const fmx::Text &other, fmx::uint32 position, fmx::_fmxcpt &_x ) throw ();
	fmx::uint32 FMX_API FM_Text_FindPrev ( const void *_self, const fmx::Text &other, fmx::uint32 position, fmx::_fmxcpt &_x ) throw ();
	fmx::uint32 FMX_API FM_Text_FindIgnoringCase ( const void *_self, const fmx::Text &other, fmx::uint32 position, fmx::_fmxcpt &_x ) throw ();
	fmx::uint32 FMX_API FM_Text_FindPrevIgnoringCase ( const void *_self, const fmx::Text &other, fmx::uint32 position, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_Uppercase ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_Lowercase ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_GetStyle( const void *_self, fmx::CharacterStyle& style, fmx::uint32 position, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_GetDefaultStyle( const void *_self, fmx::CharacterStyle& style, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_SetStyle( void *_self, const fmx::CharacterStyle& style, fmx::uint32 position, fmx::uint32 size, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_RemoveStyle( void *_self, const fmx::CharacterStyle& style, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_ResetAllStyleBuffers( void *_self, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_Text_operatorEQ ( const void *_self, const fmx::Text &that, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_Text_operatorNE ( const void *_self, const fmx::Text &that, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_Text_operatorLT ( const void *_self, const fmx::Text &that, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_Text_operatorLE ( const void *_self, const fmx::Text &that, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_Text_operatorGT ( const void *_self, const fmx::Text &that, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_Text_operatorGE ( const void *_self, const fmx::Text &that, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Text_Delete ( void *_self, fmx::_fmxcpt &_x ) throw ();

}


extern "C++"
{
	namespace fmx
	{
		inline uint32 Text::GetSize () const
		{
			_fmxcpt _x;
			uint32 _rtn = FM_Text_GetSize ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline void Text::Assign ( const char *s, Encoding encoding )
		{
			_fmxcpt _x;
			FM_Text_Assign ( this, s, encoding, _x );
			_x.Check ();
		}
		inline void Text::GetBytes ( char *buffer, uint32 buffersize, uint32 position, uint32 size, Encoding encoding ) const
		{
			_fmxcpt _x;
			FM_Text_GetBytes ( this, buffer, buffersize, position, size, encoding, _x );
			_x.Check ();
		}
		inline uint32 Text::GetBytesEx ( char *buffer, uint32 buffersize, uint32 position, uint32 size, Encoding encoding ) const
		{
			_fmxcpt _x;
			uint32 _rtn = FM_Text_GetBytesEx ( this, buffer, buffersize, position, size, encoding, _x );
			_x.Check ();
			return _rtn;
		}
		inline void Text::AssignWide ( const wchar_t *s )
		{
			_fmxcpt _x;
			FM_Text_AssignWide ( this, s, _x );
			_x.Check ();
		}
		inline void Text::AssignUnicode ( const uint16 *s )
		{
			_fmxcpt _x;
			FM_Text_AssignUnicode ( this, s, _x );
			_x.Check ();
		}
		inline void Text::AssignWithLength ( const char *s, uint32 strlength, Encoding encoding )
		{
			_fmxcpt _x;
			FM_Text_AssignWithLength ( this, s, strlength, encoding, _x );
			_x.Check ();
		}
		inline void Text::AssignWideWithLength ( const wchar_t *s, uint32 strlength )
		{
			_fmxcpt _x;
			FM_Text_AssignWideWithLength ( this, s, strlength, _x );
			_x.Check ();
		}
		inline void Text::AssignUnicodeWithLength ( const uint16 *s, uint32 strlength )
		{
			_fmxcpt _x;
			FM_Text_AssignUnicodeWithLength ( this, s, strlength, _x );
			_x.Check ();
		}
		inline void Text::GetUnicode ( uint16 *s, uint32 position, uint32 size ) const
		{
			_fmxcpt _x;
			FM_Text_GetUnicode ( this, s, position, size, _x );
			_x.Check ();
		}
		inline void Text::SetText ( const Text &other, uint32 position, uint32 size )
		{
			_fmxcpt _x;
			FM_Text_SetText ( this, other, position, size, _x );
			_x.Check ();
		}
		inline void Text::AppendText ( const Text &other, uint32 position, uint32 size )
		{
			_fmxcpt _x;
			FM_Text_AppendText ( this, other, position, size, _x );
			_x.Check ();
		}
		inline void Text::InsertText ( const Text &other, uint32 position )
		{
			_fmxcpt _x;
			FM_Text_InsertText ( this, other, position, _x );
			_x.Check ();
		}
		inline void Text::DeleteText ( uint32 positionToDelete, uint32 sizeToDelete )
		{
			_fmxcpt _x;
			FM_Text_DeleteText ( this, positionToDelete, sizeToDelete, _x );
			_x.Check ();
		}
		inline uint32 Text::Find ( const Text &other, uint32 position ) const
		{
			_fmxcpt _x;
			uint32 _rtn = FM_Text_Find ( this, other, position, _x );
			_x.Check ();
						return _rtn;
		}
		inline uint32 Text::FindPrev ( const Text &other, uint32 position ) const
		{
			_fmxcpt _x;
			uint32 _rtn = FM_Text_FindPrev ( this, other, position, _x );
			_x.Check ();
						return _rtn;
	   }
		inline uint32 Text::FindIgnoringCase ( const Text &other, uint32 position ) const
		{
			_fmxcpt _x;
			uint32 _rtn = FM_Text_FindIgnoringCase ( this, other, position, _x );
			_x.Check ();
						return _rtn;
		}
		inline uint32 Text::FindPrevIgnoringCase ( const Text &other, uint32 position ) const
		{
			_fmxcpt _x;
			uint32 _rtn = FM_Text_FindPrevIgnoringCase ( this, other, position, _x );
			_x.Check ();
						return _rtn;
	   }
		inline void Text::Uppercase ()
		{
			_fmxcpt _x;
			FM_Text_Uppercase ( this, _x );
			_x.Check ();
		}
		inline void Text::Lowercase ()
		{
			_fmxcpt _x;
			FM_Text_Lowercase ( this, _x );
			_x.Check ();
		}
				inline void Text::GetStyle( CharacterStyle& style, uint32 position ) const
		{
			_fmxcpt _x;
			FM_Text_GetStyle ( this, style, position, _x );
			_x.Check ();
		}
				inline void Text::GetDefaultStyle( CharacterStyle& style ) const
		{
			_fmxcpt _x;
			FM_Text_GetDefaultStyle ( this, style, _x );
			_x.Check ();
		}
				inline void Text::SetStyle( const CharacterStyle& style, uint32 position, uint32 size )
		{
			_fmxcpt _x;
			FM_Text_SetStyle ( this, style, position, size, _x );
			_x.Check ();
		}
				inline void Text::RemoveStyle( const CharacterStyle& style )
		{
			_fmxcpt _x;
			FM_Text_RemoveStyle ( this, style, _x );
			_x.Check ();
		}
				inline void Text::ResetAllStyleBuffers()
		{
			_fmxcpt _x;
			FM_Text_ResetAllStyleBuffers ( this, _x );
			_x.Check ();
		}
		inline bool Text::operator == ( const Text &that ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_Text_operatorEQ ( this, that, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool Text::operator != ( const Text &that ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_Text_operatorNE ( this, that, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool Text::operator < ( const Text &that ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_Text_operatorLT ( this, that, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool Text::operator <= ( const Text &that ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_Text_operatorLE ( this, that, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool Text::operator > ( const Text &that ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_Text_operatorGT ( this, that, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool Text::operator >= ( const Text &that ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_Text_operatorGE ( this, that, _x );
			_x.Check ();
			return _rtn;
		}
		inline void Text::operator delete ( void *obj )
		{
			_fmxcpt _x;
			FM_Text_Delete ( obj, _x );
			_x.Check ();
		}

#if FMX_USE_AUTO_PTR
		inline TextAutoPtr::TextAutoPtr ()
		{
			_fmxcpt _x;
			reset ( FM_Text_Constructor1 ( _x ) );
			_x.Check ();
		}
#endif
		
#if FMX_USE_UNIQUE_PTR
		inline TextUniquePtr::TextUniquePtr ()
		{
			_fmxcpt _x;
			reset ( FM_Text_Constructor1 ( _x ) );
			_x.Check ();
		}
#endif
		
	}
}


#endif /* _h_Text_ */
