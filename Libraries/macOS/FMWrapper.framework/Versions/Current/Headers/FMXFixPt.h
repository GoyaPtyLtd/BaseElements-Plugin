/*
 
 Copyright © 1998 - 2016  FileMaker, Inc.
 All rights reserved.
 
 FileMaker, Inc. grants you a non-exclusive limited license to use this file solely to enable
 licensees of FileMaker Pro Advanced to compile plug-ins for use with FileMaker products.
 Redistribution and use in source and binary forms, without modification, are permitted provided
 that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of
 conditions and the following disclaimer.
 
 * The name FileMaker, Inc. may not be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY FILEMAKER, INC. ''AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL FILEMAKER, INC. BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 */

#ifndef _h_FixPt_
#define _h_FixPt_

#include "FMWrapper/FMXClient.h"

extern "C++"
{
    namespace fmx
    {
        class FixPt
        {
        public:

            enum
            {
                kDefltFixedPrecision = 16
            };
           
            inline void AssignInt ( fmx::int32 that );
            inline void AssignDouble ( double that );
            inline void AssignFixPt ( const FixPt &that );
            inline bool operator == ( const FixPt &that ) const;
            inline bool operator != ( const FixPt &that ) const;
            inline bool operator < ( const FixPt &that ) const;
            inline bool operator <= ( const FixPt &that ) const;
            inline bool operator > ( const FixPt &that ) const;
            inline bool operator >= ( const FixPt &that ) const;
            inline void Increment ( fmx::int32 n );
            inline void Decrement ( fmx::int32 n );
            inline void Negate ();
            
            // Obtain and modify the decimal precision from the standard 16 digits. For more information, see SetPrecision calculation function in FileMaker Pro Advanced Help.
            inline int GetPrecision () const;
            inline void SetPrecision ( int precision );
            
            inline void Add ( const FixPt &arg, FixPt &result ) const;
            inline void Subtract ( const FixPt &arg, FixPt &result ) const;
            inline void Multiply ( const FixPt &arg, FixPt &result ) const;
            inline fmx::errcode Divide ( const FixPt &arg, FixPt &result ) const;
            inline bool AsBool () const;
            inline fmx::int32 AsLong () const;
            inline double AsFloat () const;

            inline void operator delete ( void *obj );

        private:
            FixPt ();
            FixPt ( const FixPt &other );

        };

        // DEPRECATED in FileMaker Pro 15. C++11 deprecated std::auto_ptr and replaced with std::unique_ptr.
        class DEPRECATED FixPtAutoPtr : public std::auto_ptr<FixPt>
        {
            typedef FixPtAutoPtr    UpCaster;
        public:
            inline FixPtAutoPtr ( fmx::int32 val = 0, int precision = FixPt::kDefltFixedPrecision );
            inline FixPtAutoPtr ( fmx::int32 val, const FixPt &precisionExample );

        };
        
#if FMX_USE_UNIQUE_PTR
        class FixPtUniquePtr : public std::unique_ptr<FixPt>
        {
            typedef FixPtUniquePtr    UpCaster;
        public:
            inline FixPtUniquePtr ( fmx::int32 val = 0, int precision = FixPt::kDefltFixedPrecision );
            inline FixPtUniquePtr ( fmx::int32 val, const FixPt &precisionExample );
            
        };
#endif
    }
}


// These functions are only useful for C-only environments or for porting to other languages (ex., Pascal).
extern "C"
{
    fmx::FixPt FMX_API *FM_FixPt_Constructor1 ( fmx::int32 val, int precision, fmx::_fmxcpt &_x ) throw ();
    fmx::FixPt FMX_API *FM_FixPt_Constructor2 ( fmx::int32 val, const fmx::FixPt &precisionExample, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_AssignInt ( void *_self, fmx::int32 that, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_AssignDouble ( void *_self, double that, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_AssignFixPt ( void *_self, const fmx::FixPt &that, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_FixPt_operatorEQ ( const void *_self, const fmx::FixPt &that, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_FixPt_operatorNE ( const void *_self, const fmx::FixPt &that, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_FixPt_operatorLT ( const void *_self, const fmx::FixPt &that, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_FixPt_operatorLE ( const void *_self, const fmx::FixPt &that, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_FixPt_operatorGT ( const void *_self, const fmx::FixPt &that, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_FixPt_operatorGE ( const void *_self, const fmx::FixPt &that, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_Increment ( void *_self, fmx::int32 n, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_Decrement ( void *_self, fmx::int32 n, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_Negate ( void *_self, fmx::_fmxcpt &_x ) throw ();
    int FMX_API FM_FixPt_GetPrecision ( const void *_self, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_SetPrecision ( void *_self, int precision, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_Add ( const void *_self, const fmx::FixPt &arg, fmx::FixPt &result, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_Subtract ( const void *_self, const fmx::FixPt &arg, fmx::FixPt &result, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_Multiply ( const void *_self, const fmx::FixPt &arg, fmx::FixPt &result, fmx::_fmxcpt &_x ) throw ();
    fmx::errcode FMX_API FM_FixPt_Divide ( const void *_self, const fmx::FixPt &arg, fmx::FixPt &result, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_FixPt_AsBool ( const void *_self, fmx::_fmxcpt &_x ) throw ();
    fmx::int32 FMX_API FM_FixPt_AsLong ( const void *_self, fmx::_fmxcpt &_x ) throw ();
    double FMX_API FM_FixPt_AsFloat ( const void *_self, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_FixPt_Delete ( void *_self, fmx::_fmxcpt &_x ) throw ();

}


extern "C++"
{
    namespace fmx
    {
        inline void FixPt::AssignInt ( fmx::int32 that )
        {
            _fmxcpt _x;
            FM_FixPt_AssignInt ( this, that, _x );
            _x.Check ();
        }
        inline void FixPt::AssignDouble ( double that )
        {
            _fmxcpt _x;
            FM_FixPt_AssignDouble ( this, that, _x );
            _x.Check ();
        }
        inline void FixPt::AssignFixPt ( const FixPt &that )
        {
            _fmxcpt _x;
            FM_FixPt_AssignFixPt ( this, that, _x );
            _x.Check ();
        }
        inline bool FixPt::operator == ( const FixPt &that ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_FixPt_operatorEQ ( this, that, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool FixPt::operator != ( const FixPt &that ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_FixPt_operatorNE ( this, that, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool FixPt::operator < ( const FixPt &that ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_FixPt_operatorLT ( this, that, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool FixPt::operator <= ( const FixPt &that ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_FixPt_operatorLE ( this, that, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool FixPt::operator > ( const FixPt &that ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_FixPt_operatorGT ( this, that, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool FixPt::operator >= ( const FixPt &that ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_FixPt_operatorGE ( this, that, _x );
            _x.Check ();
            return _rtn;
        }
        inline void FixPt::Increment ( fmx::int32 n )
        {
            _fmxcpt _x;
            FM_FixPt_Increment ( this, n, _x );
            _x.Check ();
        }
        inline void FixPt::Decrement ( fmx::int32 n )
        {
            _fmxcpt _x;
            FM_FixPt_Decrement ( this, n, _x );
            _x.Check ();
        }
        inline void FixPt::Negate ()
        {
            _fmxcpt _x;
            FM_FixPt_Negate ( this, _x );
            _x.Check ();
        }
        inline int FixPt::GetPrecision () const
        {
            _fmxcpt _x;
            int _rtn = FM_FixPt_GetPrecision ( this, _x );
            _x.Check ();
            return _rtn;
        }
        inline void FixPt::SetPrecision ( int precision )
        {
            _fmxcpt _x;
            FM_FixPt_SetPrecision ( this, precision, _x );
            _x.Check ();
        }
        inline void FixPt::Add ( const FixPt &arg, FixPt &result ) const
        {
            _fmxcpt _x;
            FM_FixPt_Add ( this, arg, result, _x );
            _x.Check ();
        }
        inline void FixPt::Subtract ( const FixPt &arg, FixPt &result ) const
        {
            _fmxcpt _x;
            FM_FixPt_Subtract ( this, arg, result, _x );
            _x.Check ();
        }
        inline void FixPt::Multiply ( const FixPt &arg, FixPt &result ) const
        {
            _fmxcpt _x;
            FM_FixPt_Multiply ( this, arg, result, _x );
            _x.Check ();
        }
        inline errcode FixPt::Divide ( const FixPt &arg, FixPt &result ) const
        {
            _fmxcpt _x;
            errcode _rtn = FM_FixPt_Divide ( this, arg, result, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool FixPt::AsBool () const
        {
            _fmxcpt _x;
            bool _rtn = FM_FixPt_AsBool ( this, _x );
            _x.Check ();
            return _rtn;
        }
        inline fmx::int32 FixPt::AsLong () const
        {
            _fmxcpt _x;
            fmx::int32 _rtn = FM_FixPt_AsLong ( this, _x );
            _x.Check ();
            return _rtn;
        }
        inline double FixPt::AsFloat () const
        {
            _fmxcpt _x;
            double _rtn = FM_FixPt_AsFloat ( this, _x );
            _x.Check ();
            return _rtn;
        }
        inline void FixPt::operator delete ( void *obj )
        {
            _fmxcpt _x;
            FM_FixPt_Delete ( obj, _x );
            _x.Check ();
        }

        inline FixPtAutoPtr::FixPtAutoPtr ( fmx::int32 val, int precision )
        {
            _fmxcpt _x;
            reset ( FM_FixPt_Constructor1 ( val, precision, _x ) );
            _x.Check ();
        }
        inline FixPtAutoPtr::FixPtAutoPtr ( fmx::int32 val, const FixPt &precisionExample )
        {
            _fmxcpt _x;
            reset ( FM_FixPt_Constructor2 ( val, precisionExample, _x ) );
            _x.Check ();
        }

#if FMX_USE_UNIQUE_PTR
        inline FixPtUniquePtr::FixPtUniquePtr ( fmx::int32 val, int precision )
        {
            _fmxcpt _x;
            reset ( FM_FixPt_Constructor1 ( val, precision, _x ) );
            _x.Check ();
        }
        inline FixPtUniquePtr::FixPtUniquePtr ( fmx::int32 val, const FixPt &precisionExample )
        {
            _fmxcpt _x;
            reset ( FM_FixPt_Constructor2 ( val, precisionExample, _x ) );
            _x.Check ();
        }
#endif

    }
}


#endif /* _h_FixPt_ */
