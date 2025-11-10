/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: other_codecs.h,v 1.13 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CODEC_OTHER_CODECS_H_
#define _MIMETIC_CODEC_OTHER_CODECS_H_
#include <mimetic/codec/codec_base.h>

namespace mimetic
{

/// Pass through codec. Copies input to output
/*!

 \sa encode decode
 */
struct NullCodec: public unbuffered_codec, public chainable_codec<NullCodec>
{
    template<typename OutIt>
    void process(char c, OutIt& out)
    {
        *out = c; ++out;    
    }
    const char* name() const 
    {    
        return "NullCodec"; 
    }
};

/// Converts input chars to upper case
/*!

 \sa encode decode
 */
struct ToUpperCase: public unbuffered_codec, public chainable_codec<ToUpperCase>
{
    template<typename OutIt>
    void process(char c, OutIt& out)
    {
        enum { offset = 'A' - 'a' };
        if(c >= 'a' && c <= 'z')
            c += offset;
        *out = c;
        ++out;
    }
    const char* name() const
    {    
        return "ToUpperCase"; 
    }
};

/// Converts input chars to lower case
/*!

 \sa encode decode
 */
struct ToLowerCase: public unbuffered_codec, public chainable_codec<ToLowerCase>
{
    template<typename OutIt>
    void process(char c, OutIt& out)
    {
        enum { offset = 'a' - 'A' };
        if(c >= 'A' && c <= 'Z')
            c += offset;
        *out = c;
        ++out;
    }
    const char* name() const 
    {    
        return "ToLowerCase";
    }
};


/// Converts any LF (\\n) to CRLF (\\r\\n)
/*!

 \sa encode decode
 */
struct Lf2CrLf: public unbuffered_codec, public chainable_codec<Lf2CrLf>
{
    template<typename OutIt>
    void process(char c, OutIt& out)
    {
        enum { LF = 0xA, CR = 0xD };
        if(c == LF)
        {
            *out = CR; ++out;
            *out = LF; ++out; 
        } else
            *out = c; ++out;
    }
    const char* name() const
    {    
        return "Lf2CrLf"; 
    }
};

/// Inserts a new line if the input line is too long
/*!

 \sa encode decode
 */
struct MaxLineLen: public unbuffered_codec, public chainable_codec<MaxLineLen>
{
    MaxLineLen()
    : m_max(0), m_written(0)
    {
    }
    MaxLineLen(uint m)
    : m_max(m), m_written(0)
    {
    }
    template<typename OutIt>
    void process(char c, OutIt& out)
    {
        enum { cr = 0xD, lf = 0xA };
        if(m_max && m_written++ == m_max)
        {
            *out = cr; ++out;
            *out = lf; ++out;
            m_written = 1;
        }
        *out = c;
        ++out;
    }
    const char* name() const
    {    
        return "MaxLineLen"; 
    }
private:
    unsigned int m_max, m_written;
};

// internal
template<typename OutIt>
struct oiterator_wrapper: 
    public unbuffered_codec, 
    public chainable_codec<oiterator_wrapper<OutIt> >
{
    oiterator_wrapper(): m_pOut(0)
    {
    }
    oiterator_wrapper(OutIt& out): m_pOut(&out)
    {
    }
    template<typename OutIt2>
    void process(char c, OutIt2& out)
    {
        **m_pOut = c; ++(*m_pOut);
        *out = c; ++out;
    }
    const char* name() const
    {    
        return "oiterator_wrapper"; 
    }
private:
    OutIt* m_pOut;    
};


}
#endif
