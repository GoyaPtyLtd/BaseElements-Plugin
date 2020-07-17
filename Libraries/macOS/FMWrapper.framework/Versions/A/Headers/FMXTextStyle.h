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

#ifndef _h_TextStyle_
#define _h_TextStyle_


#include "FMWrapper/FMXClient.h"


extern "C++"
{
	namespace fmx
	{
		class CharacterStyle
		{
		public:
			typedef unsigned short Face;
			typedef unsigned short FontID;
			typedef unsigned short FontSize;
			typedef unsigned char ColorChannel;
			typedef unsigned short ColorChannel16;

			// DEPRECATED in FileMaker Pro 14. Fonts no longer belong to "scripts"
			typedef unsigned short FontScript;
			enum
			{
				kRoman                  = 0,
				kGreek                  = 1,
				kCyrillic               = 2,
				kCentralEurope          = 3,
				kShiftJIS               = 4,
				kTraditionalChinese     = 5,
				kSimplifiedChinese      = 6,
				kOEM                    = 7,
				kSymbol                 = 8,
				kNative                 = 9,
				kOther                  = 10,
				kKorean                 = 11,
				kDingbats               = 12,
				kTurkish                = 13,

				kInvalidFont            = 0xFFFF
			};

			class Color
			{
			public:
				enum
				{
					kColorChannel_ON = 255,
					kColorChannel_OFF = 0
				};
				inline void SetRed ( ColorChannel r );
				inline void SetGreen ( ColorChannel g );
				inline void SetBlue ( ColorChannel b );
				inline void SetAlpha ( ColorChannel a );
				inline ColorChannel GetRed () const;
				inline ColorChannel GetGreen () const;
				inline ColorChannel GetBlue () const;
				inline ColorChannel GetAlpha () const;
				inline ColorChannel16 GetRedUpsample () const;
				inline ColorChannel16 GetGreenUpsample () const;
				inline ColorChannel16 GetBlueUpsample () const;
				inline ColorChannel16 GetAlphaUpsample () const;
				inline Color &operator = ( const Color &rhs );
				inline bool operator == ( const Color &rhs ) const;
				inline bool operator != ( const Color &rhs ) const;
				inline static const Color &White () throw ();
				inline static const Color &Black () throw ();

				inline void operator delete ( void *obj );

			private:
				Color ();
				Color ( const Color &color );

			};


			enum
			{
				kFace_Plain = 0,
				kFace_Bold = 256,
				kFace_Italic = 512,
				kFace_Underline = 1024,
				kFace_Outline = 2048,
				kFace_Shadow = 4096,
				kFace_Condense = 8192,
				kFace_Extend = 16384,
				kFace_Strikethrough = 1,
				kFace_SmallCaps = 2,
				kFace_Superscript = 4,
				kFace_Subscript = 8,
				kFace_Uppercase = 16,
				kFace_Lowercase = 32,
				kFace_Titlecase = ( kFace_Uppercase | kFace_Lowercase ),
				kFace_WordUnderline = 64,
				kFace_DoubleUnderline = 128,
				kFace_AllStyles = ( kFace_Plain | kFace_Bold | kFace_Italic | kFace_Underline | kFace_Outline | kFace_Shadow
									| kFace_Condense | kFace_Extend | kFace_Strikethrough | kFace_SmallCaps | kFace_Superscript
									| kFace_Subscript | kFace_Uppercase | kFace_Lowercase | kFace_Titlecase | kFace_WordUnderline
									| kFace_DoubleUnderline )
			};

			// These methods enables you to specify only one trait, and not overwrite others. A newly constructed CharacterStyle
			// will have everything disabled. Using any of the Set methods further down will also enable that trait in this style.
			// Disabled traits will be defaulted to the settings of the field on the layout.
			inline void EnableFont ();
			inline void EnableFace ( Face face );
			inline void EnableSize ();
			inline void EnableColor ();
			inline void DisableFont ();
			inline void DisableFace ( Face face );
			inline void DisableAllFaces ();
			inline void DisableSize ();
			inline void DisableColor ();
			inline void DisableAll ();
			inline bool IsAllDisabled () const;
			inline bool IsFontEnabled () const;
			inline bool IsFaceEnabled ( Face face ) const;
			inline bool IsAnyFaceEnabled () const;
			inline bool IsSizeEnabled () const;
			inline bool IsColorEnabled () const;

			inline void SetFontInformation ( FontID font, Face face, FontSize size );
			inline void GetFontInformation ( FontID &font, Face &face, FontSize &size );
			inline void SetFont ( FontID font );
			inline void SetFace ( Face face );
			inline void SetSize ( FontSize size );
			inline FontID GetFont () const;
			inline Face GetFace () const;
			inline FontSize GetSize () const;
			inline void SetColor ( const Color &color );
			inline const Color &GetColor () const;
			inline CharacterStyle &operator = ( const CharacterStyle &rhs );
			inline bool operator == ( const CharacterStyle &rhs ) const;
			inline bool operator != ( const CharacterStyle &rhs ) const;

			inline void operator delete ( void *obj );

		private:
			CharacterStyle ();
			CharacterStyle ( const CharacterStyle &style );

		};

#if FMX_USE_AUTO_PTR
		// DEPRECATED in FileMaker Pro 15. C++11 deprecated std::auto_ptr and replaced with std::unique_ptr.
		class DEPRECATED ColorAutoPtr : public std::auto_ptr<CharacterStyle::Color>
		{
			typedef ColorAutoPtr    UpCaster;
		public:
			inline ColorAutoPtr ();
			inline ColorAutoPtr ( CharacterStyle::ColorChannel red, CharacterStyle::ColorChannel green, CharacterStyle::ColorChannel blue,
											CharacterStyle::ColorChannel alpha = CharacterStyle::Color::kColorChannel_ON );
			inline ColorAutoPtr ( const CharacterStyle::Color &mask );

		};

		// DEPRECATED in FileMaker Pro 15. C++11 deprecated std::auto_ptr and replaced with std::unique_ptr.
		class DEPRECATED CharacterStyleAutoPtr : public std::auto_ptr<CharacterStyle>
		{
			typedef CharacterStyleAutoPtr   UpCaster;
		public:
			inline CharacterStyleAutoPtr ();
			inline CharacterStyleAutoPtr ( CharacterStyle::FontID font, CharacterStyle::Face face, CharacterStyle::FontSize size,
											const CharacterStyle::Color &color = CharacterStyle::Color::Black() );
			inline CharacterStyleAutoPtr ( const CharacterStyle &style );

		};
#endif
		
#if FMX_USE_UNIQUE_PTR
		class ColorUniquePtr : public std::unique_ptr<CharacterStyle::Color>
		{
			typedef ColorUniquePtr    UpCaster;
		public:
			inline ColorUniquePtr ();
			inline ColorUniquePtr ( CharacterStyle::ColorChannel red, CharacterStyle::ColorChannel green, CharacterStyle::ColorChannel blue,
								 CharacterStyle::ColorChannel alpha = CharacterStyle::Color::kColorChannel_ON );
			inline ColorUniquePtr ( const CharacterStyle::Color &mask );
			
		};
		
		class CharacterStyleUniquePtr : public std::unique_ptr<CharacterStyle>
		{
			typedef CharacterStyleUniquePtr   UpCaster;
		public:
			inline CharacterStyleUniquePtr ();
			inline CharacterStyleUniquePtr ( CharacterStyle::FontID font, CharacterStyle::Face face, CharacterStyle::FontSize size,
										  const CharacterStyle::Color &color = CharacterStyle::Color::Black() );
			inline CharacterStyleUniquePtr ( const CharacterStyle &style );
			
		};
#endif
	}
}


// These functions are only useful for C-only environments or for porting to other languages (ex., Pascal).
extern "C"
{

	fmx::CharacterStyle::Color FMX_API *FM_Color_Constructor1 ( fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::Color FMX_API *FM_Color_Constructor2 ( fmx::CharacterStyle::ColorChannel red, fmx::CharacterStyle::ColorChannel green, fmx::CharacterStyle::ColorChannel blue, fmx::CharacterStyle::ColorChannel alpha, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::Color FMX_API *FM_Color_Constructor3 ( const fmx::CharacterStyle::Color &color, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Color_SetRed ( void *_self, fmx::CharacterStyle::ColorChannel r, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Color_SetGreen ( void *_self, fmx::CharacterStyle::ColorChannel g, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Color_SetBlue ( void *_self, fmx::CharacterStyle::ColorChannel b, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_Color_SetAlpha ( void *_self, fmx::CharacterStyle::ColorChannel a, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::ColorChannel FMX_API FM_Color_GetRed ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::ColorChannel FMX_API FM_Color_GetGreen ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::ColorChannel FMX_API FM_Color_GetBlue ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::ColorChannel FMX_API FM_Color_GetAlpha ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::ColorChannel16 FMX_API FM_Color_GetRedUpsample ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::ColorChannel16 FMX_API FM_Color_GetGreenUpsample ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::ColorChannel16 FMX_API FM_Color_GetBlueUpsample ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::ColorChannel16 FMX_API FM_Color_GetAlphaUpsample ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::Color FMX_API *FM_Color_operatorAS ( void *_self, const fmx::CharacterStyle::Color &rhs, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_Color_operatorEQ ( const void *_self, const fmx::CharacterStyle::Color &rhs, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_Color_operatorNE ( const void *_self, const fmx::CharacterStyle::Color &rhs, fmx::_fmxcpt &_x ) throw ();
	const fmx::CharacterStyle::Color FMX_API *FM_Color_White () throw ();
	const fmx::CharacterStyle::Color FMX_API *FM_Color_Black () throw ();
	void FMX_API FM_Color_Delete ( void *_self, fmx::_fmxcpt &_x ) throw ();


	fmx::CharacterStyle FMX_API *FM_CharacterStyle_Constructor1 ( fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle FMX_API *FM_CharacterStyle_Constructor2 ( fmx::CharacterStyle::FontID font, fmx::CharacterStyle::Face face, fmx::CharacterStyle::FontSize size, const fmx::CharacterStyle::Color &color, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle FMX_API *FM_CharacterStyle_Constructor3 ( const fmx::CharacterStyle &style, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_EnableFont ( void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_EnableFace ( void *_self, fmx::CharacterStyle::Face face, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_EnableSize ( void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_EnableColor ( void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_DisableFont ( void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_DisableFace ( void *_self, fmx::CharacterStyle::Face face, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_DisableAllFaces ( void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_DisableSize ( void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_DisableColor ( void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_DisableAll ( void *_self, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_CharacterStyle_IsAllDisabled ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_CharacterStyle_IsFontEnabled ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_CharacterStyle_IsFaceEnabled ( const void *_self, fmx::CharacterStyle::Face face, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_CharacterStyle_IsAnyFaceEnabled ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_CharacterStyle_IsSizeEnabled ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_CharacterStyle_IsColorEnabled ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_SetFontInformation ( void *_self, fmx::CharacterStyle::FontID font, fmx::CharacterStyle::Face face, fmx::CharacterStyle::FontSize size, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_GetFontInformation ( void *_self, fmx::CharacterStyle::FontID &font, fmx::CharacterStyle::Face &face, fmx::CharacterStyle::FontSize &size, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_SetFont ( void *_self, fmx::CharacterStyle::FontID font, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_SetFace ( void *_self, fmx::CharacterStyle::Face face, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_SetSize ( void *_self, fmx::CharacterStyle::FontSize size, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::FontID FMX_API FM_CharacterStyle_GetFont ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::Face FMX_API FM_CharacterStyle_GetFace ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle::FontSize FMX_API FM_CharacterStyle_GetSize ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_SetColor ( void *_self, const fmx::CharacterStyle::Color &color, fmx::_fmxcpt &_x ) throw ();
	const fmx::CharacterStyle::Color FMX_API *FM_CharacterStyle_GetColor ( const void *_self, fmx::_fmxcpt &_x ) throw ();
	fmx::CharacterStyle FMX_API *FM_CharacterStyle_operatorAS ( void *_self, const fmx::CharacterStyle &rhs, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_CharacterStyle_operatorEQ ( const void *_self, const fmx::CharacterStyle &rhs, fmx::_fmxcpt &_x ) throw ();
	bool FMX_API FM_CharacterStyle_operatorNE ( const void *_self, const fmx::CharacterStyle &rhs, fmx::_fmxcpt &_x ) throw ();
	void FMX_API FM_CharacterStyle_Delete ( void *_self, fmx::_fmxcpt &_x ) throw ();

}


extern "C++"
{
	namespace fmx
	{
		inline void CharacterStyle::Color::SetRed ( ColorChannel r )
		{
			_fmxcpt _x;
			FM_Color_SetRed ( this, r, _x );
			_x.Check ();
		}
		inline void CharacterStyle::Color::SetGreen ( ColorChannel g )
		{
			_fmxcpt _x;
			FM_Color_SetGreen ( this, g, _x );
			_x.Check ();
		}
		inline void CharacterStyle::Color::SetBlue ( ColorChannel b )
		{
			_fmxcpt _x;
			FM_Color_SetBlue ( this, b, _x );
			_x.Check ();
		}
		inline void CharacterStyle::Color::SetAlpha ( ColorChannel a )
		{
			_fmxcpt _x;
			FM_Color_SetAlpha ( this, a, _x );
			_x.Check ();
		}
		inline CharacterStyle::ColorChannel CharacterStyle::Color::GetRed () const
		{
			_fmxcpt _x;
			ColorChannel _rtn = FM_Color_GetRed ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::ColorChannel CharacterStyle::Color::GetGreen () const
		{
			_fmxcpt _x;
			ColorChannel _rtn = FM_Color_GetGreen ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::ColorChannel CharacterStyle::Color::GetBlue () const
		{
			_fmxcpt _x;
			ColorChannel _rtn = FM_Color_GetBlue ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::ColorChannel CharacterStyle::Color::GetAlpha () const
		{
			_fmxcpt _x;
			ColorChannel _rtn = FM_Color_GetAlpha ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::ColorChannel16 CharacterStyle::Color::GetRedUpsample () const
		{
			_fmxcpt _x;
			ColorChannel16 _rtn = FM_Color_GetRedUpsample ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::ColorChannel16 CharacterStyle::Color::GetGreenUpsample () const
		{
			_fmxcpt _x;
			ColorChannel16 _rtn = FM_Color_GetGreenUpsample ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::ColorChannel16 CharacterStyle::Color::GetBlueUpsample () const
		{
			_fmxcpt _x;
			ColorChannel16 _rtn = FM_Color_GetBlueUpsample ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::ColorChannel16 CharacterStyle::Color::GetAlphaUpsample () const
		{
			_fmxcpt _x;
			ColorChannel16 _rtn = FM_Color_GetAlphaUpsample ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::Color &CharacterStyle::Color::operator = ( const Color &rhs )
		{
			_fmxcpt _x;
			Color *_rtn = FM_Color_operatorAS ( this, rhs, _x );
			_x.Check ();
			return *_rtn;
		}
		inline bool CharacterStyle::Color::operator == ( const Color &rhs ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_Color_operatorEQ ( this, rhs, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool CharacterStyle::Color::operator != ( const Color &rhs ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_Color_operatorNE ( this, rhs, _x );
			_x.Check ();
			return _rtn;
		}
		inline const CharacterStyle::Color &CharacterStyle::Color::White () throw ()
		{
			return *FM_Color_White ();
		}
		inline const CharacterStyle::Color &CharacterStyle::Color::Black () throw ()
		{
			return *FM_Color_Black ();
		}
		inline void CharacterStyle::Color::operator delete ( void *obj )
		{
			_fmxcpt _x;
			FM_Color_Delete ( obj, _x );
			_x.Check ();
		}

#if FMX_USE_AUTO_PTR
		inline ColorAutoPtr::ColorAutoPtr ()
		{
			_fmxcpt _x;
			reset ( FM_Color_Constructor1 ( _x ) );
			_x.Check ();
		}
		inline ColorAutoPtr::ColorAutoPtr ( CharacterStyle::ColorChannel red, CharacterStyle::ColorChannel green, CharacterStyle::ColorChannel blue, CharacterStyle::ColorChannel alpha )
		{
			_fmxcpt _x;
			reset ( FM_Color_Constructor2 ( red, green, blue, alpha, _x ) );
			_x.Check ();
		}
		inline ColorAutoPtr::ColorAutoPtr ( const CharacterStyle::Color &color )
		{
			_fmxcpt _x;
			reset ( FM_Color_Constructor3 ( color, _x ) );
			_x.Check ();
		}
#endif
		
#if FMX_USE_UNIQUE_PTR
		inline ColorUniquePtr::ColorUniquePtr ()
		{
			_fmxcpt _x;
			reset ( FM_Color_Constructor1 ( _x ) );
			_x.Check ();
		}
		inline ColorUniquePtr::ColorUniquePtr ( CharacterStyle::ColorChannel red, CharacterStyle::ColorChannel green, CharacterStyle::ColorChannel blue, CharacterStyle::ColorChannel alpha )
		{
			_fmxcpt _x;
			reset ( FM_Color_Constructor2 ( red, green, blue, alpha, _x ) );
			_x.Check ();
		}
		inline ColorUniquePtr::ColorUniquePtr ( const CharacterStyle::Color &color )
		{
			_fmxcpt _x;
			reset ( FM_Color_Constructor3 ( color, _x ) );
			_x.Check ();
		}
#endif
		
		inline void CharacterStyle::EnableFont ()
		{
			_fmxcpt _x;
			FM_CharacterStyle_EnableFont ( this, _x );
			_x.Check ();
		}
		inline void CharacterStyle::EnableFace ( Face face )
		{
			_fmxcpt _x;
			FM_CharacterStyle_EnableFace ( this, face, _x );
			_x.Check ();
		}
		inline void CharacterStyle::EnableSize ()
		{
			_fmxcpt _x;
			FM_CharacterStyle_EnableSize ( this, _x );
			_x.Check ();
		}
		inline void CharacterStyle::EnableColor ()
		{
			_fmxcpt _x;
			FM_CharacterStyle_EnableColor ( this, _x );
			_x.Check ();
		}
		inline void CharacterStyle::DisableFont ()
		{
			_fmxcpt _x;
			FM_CharacterStyle_DisableFont ( this, _x );
			_x.Check ();
		}
		inline void CharacterStyle::DisableFace ( Face face )
		{
			_fmxcpt _x;
			FM_CharacterStyle_DisableFace ( this, face, _x );
			_x.Check ();
		}
		inline void CharacterStyle::DisableAllFaces ()
		{
			_fmxcpt _x;
			FM_CharacterStyle_DisableAllFaces ( this, _x );
			_x.Check ();
		}
		inline void CharacterStyle::DisableSize ()
		{
			_fmxcpt _x;
			FM_CharacterStyle_DisableSize ( this, _x );
			_x.Check ();
		}
		inline void CharacterStyle::DisableColor ()
		{
			_fmxcpt _x;
			FM_CharacterStyle_DisableColor ( this, _x );
			_x.Check ();
		}
		inline void CharacterStyle::DisableAll ()
		{
			_fmxcpt _x;
			FM_CharacterStyle_DisableAll ( this, _x );
			_x.Check ();
		}
		inline bool CharacterStyle::IsAllDisabled () const
		{
			_fmxcpt _x;
			bool _rtn = FM_CharacterStyle_IsAllDisabled ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool CharacterStyle::IsFontEnabled () const
		{
			_fmxcpt _x;
			bool _rtn = FM_CharacterStyle_IsFontEnabled ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool CharacterStyle::IsFaceEnabled ( Face face ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_CharacterStyle_IsFaceEnabled ( this, face, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool CharacterStyle::IsAnyFaceEnabled () const
		{
			_fmxcpt _x;
			bool _rtn = FM_CharacterStyle_IsAnyFaceEnabled ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool CharacterStyle::IsSizeEnabled () const
		{
			_fmxcpt _x;
			bool _rtn = FM_CharacterStyle_IsSizeEnabled ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool CharacterStyle::IsColorEnabled () const
		{
			_fmxcpt _x;
			bool _rtn = FM_CharacterStyle_IsColorEnabled ( this, _x );
			_x.Check ();
			return _rtn;
		}

		inline void CharacterStyle::SetFontInformation ( FontID font, Face face, FontSize size )
		{
			_fmxcpt _x;
			FM_CharacterStyle_SetFontInformation ( this, font, face, size, _x );
			_x.Check ();
		}
		inline void CharacterStyle::GetFontInformation ( FontID &font, Face &face, FontSize &size )
		{
			_fmxcpt _x;
			FM_CharacterStyle_GetFontInformation ( this, font, face, size, _x );
			_x.Check ();
		}
		inline void CharacterStyle::SetFont ( FontID font )
		{
			_fmxcpt _x;
			FM_CharacterStyle_SetFont ( this, font, _x );
			_x.Check ();
		}
		inline void CharacterStyle::SetFace ( Face face )
		{
			_fmxcpt _x;
			FM_CharacterStyle_SetFace ( this, face, _x );
			_x.Check ();
		}
		inline void CharacterStyle::SetSize ( FontSize size )
		{
			_fmxcpt _x;
			FM_CharacterStyle_SetSize ( this, size, _x );
			_x.Check ();
		}
		inline CharacterStyle::FontID CharacterStyle::GetFont () const
		{
			_fmxcpt _x;
			FontID _rtn = FM_CharacterStyle_GetFont ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::Face CharacterStyle::GetFace () const
		{
			_fmxcpt _x;
			Face _rtn = FM_CharacterStyle_GetFace ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline CharacterStyle::FontSize CharacterStyle::GetSize () const
		{
			_fmxcpt _x;
			FontSize _rtn = FM_CharacterStyle_GetSize ( this, _x );
			_x.Check ();
			return _rtn;
		}
		inline void CharacterStyle::SetColor ( const Color &color )
		{
			_fmxcpt _x;
			FM_CharacterStyle_SetColor ( this, color, _x );
			_x.Check ();
		}
		inline const CharacterStyle::Color &CharacterStyle::GetColor () const
		{
			_fmxcpt _x;
			const Color *_rtn = FM_CharacterStyle_GetColor ( this, _x );
			_x.Check ();
			return *_rtn;
		}
		inline CharacterStyle &CharacterStyle::operator = ( const CharacterStyle &rhs )
		{
			_fmxcpt _x;
			CharacterStyle *_rtn = FM_CharacterStyle_operatorAS ( this, rhs, _x );
			_x.Check ();
			return *_rtn;
		}
		inline bool CharacterStyle::operator == ( const CharacterStyle &rhs ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_CharacterStyle_operatorEQ ( this, rhs, _x );
			_x.Check ();
			return _rtn;
		}
		inline bool CharacterStyle::operator != ( const CharacterStyle &rhs ) const
		{
			_fmxcpt _x;
			bool _rtn = FM_CharacterStyle_operatorNE ( this, rhs, _x );
			_x.Check ();
			return _rtn;
		}
		inline void CharacterStyle::operator delete ( void *obj )
		{
			_fmxcpt _x;
			FM_CharacterStyle_Delete ( obj, _x );
			_x.Check ();
		}

#if FMX_USE_AUTO_PTR
		inline CharacterStyleAutoPtr::CharacterStyleAutoPtr ()
		{
			_fmxcpt _x;
			reset ( FM_CharacterStyle_Constructor1 ( _x ) );
			_x.Check ();
		}
		inline CharacterStyleAutoPtr::CharacterStyleAutoPtr ( CharacterStyle::FontID font, CharacterStyle::Face face, CharacterStyle::FontSize size, const CharacterStyle::Color &color )
		{
			_fmxcpt _x;
			reset ( FM_CharacterStyle_Constructor2 ( font, face, size, color, _x ) );
			_x.Check ();
		}
		inline CharacterStyleAutoPtr::CharacterStyleAutoPtr ( const CharacterStyle &style )
		{
			_fmxcpt _x;
			reset ( FM_CharacterStyle_Constructor3 ( style, _x ) );
			_x.Check ();
		}
#endif
		
#if FMX_USE_UNIQUE_PTR
		inline CharacterStyleUniquePtr::CharacterStyleUniquePtr ()
		{
			_fmxcpt _x;
			reset ( FM_CharacterStyle_Constructor1 ( _x ) );
			_x.Check ();
		}
		inline CharacterStyleUniquePtr::CharacterStyleUniquePtr ( CharacterStyle::FontID font, CharacterStyle::Face face, CharacterStyle::FontSize size, const CharacterStyle::Color &color )
		{
			_fmxcpt _x;
			reset ( FM_CharacterStyle_Constructor2 ( font, face, size, color, _x ) );
			_x.Check ();
		}
		inline CharacterStyleUniquePtr::CharacterStyleUniquePtr ( const CharacterStyle &style )
		{
			_fmxcpt _x;
			reset ( FM_CharacterStyle_Constructor3 ( style, _x ) );
			_x.Check ();
		}
#endif
	}
}


#endif /* _h_TextStyle_ */
