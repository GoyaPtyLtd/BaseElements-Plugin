/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: base64.h,v 1.15 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CODEC_BASE64_H_
#define _MIMETIC_CODEC_BASE64_H_
#include <mimetic/circular_buffer.h>
#include <mimetic/codec/codec_base.h>
#include <mimetic/codec/codec_chain.h>

namespace mimetic
{


class Base64
{
    enum { LF = 0xA, CR = 0xD, NL = '\n' };
    enum { default_maxlen = 76 };
    enum { eq_sign = 100 };
    static const char sEncTable[];
    static const char sDecTable[];
    static const int sDecTableSz;
public:
    class Encoder; class Decoder;
    typedef Encoder encoder_type;
    typedef Decoder decoder_type;


/// Base64 encoder
/*!

 \sa encode decode
 */
class Encoder: public buffered_codec, public chainable_codec<Encoder>
{
    enum { pad_idx = 64 };
    char_type m_ch[3];
    int m_cidx;
    int m_pos, m_maxlen;

    template<typename OutIt>
    inline void writeBuf(OutIt& out)
    {
        int pad_count = 3 - m_cidx;
        m_cidx = 0;    
        int idx[4];
        idx[0] = m_ch[0] >> 2;
        switch(pad_count)
        {
        case 0:
            idx[1] = (((m_ch[0] & 3) << 4) |  (m_ch[1] >> 4));
            idx[2] = ((m_ch[1] & 0xf) << 2) | (m_ch[2] >> 6);
            idx[3] = m_ch[2] & 0x3f;
            break;
        case 1:
            idx[1] = (((m_ch[0] & 3) << 4) |  (m_ch[1] >> 4));
            idx[2] = (m_ch[1] & 0xf) << 2 ;
            idx[3] = pad_idx;
            break;
        case 2:
            idx[1] = (m_ch[0] & 3) << 4;
            idx[2] = idx[3] = pad_idx;
            break;
        }
        for(int i = 0; i < 4; ++i)
        {
            *out = sEncTable[ idx[i] ]; ++out;
            if(m_maxlen && ++m_pos > m_maxlen)
            {
				*out = CR; ++out;
				*out = LF; ++out;
                m_pos = 1;
            }
        }
    }
public:
    /*! return the multiplier of the required (max) size of the output buffer 
     * when encoding */
    double codeSizeMultiplier() const
    {
        return 1.5;
    }
    /*! Constructor, maxlen is the maximum length of every encoded line */
    Encoder(int maxlen = default_maxlen)
    : m_cidx(0), m_pos(1), m_maxlen(maxlen)
    {
    }
    /*! Returns the name of the codec ("Base64") */
    const char* name() const { return "Base64"; }
    /*! 
     Encodes [\p bit,\p eit) and write any encoded char to \p out.
     */
    template<typename InIt, typename OutIt>
    void process(InIt bit, InIt eit, OutIt out)
    {
        for(; bit != eit; ++bit)
        {
            m_ch[m_cidx++] = (char_type)*bit; 
            if(m_cidx < 3)
                continue;
            writeBuf(out);
        } 
        if(m_cidx > 0)
            writeBuf(out);
    }
    /*! 
     Encodes \p c and write any encoded output char to \p out.
     \warning You must call flush() when all chars have been 
     processed by the encode funcion.
     \n
     \code
        while( (c = getchar()) != EOF )
            b64.encode(c, out);    
        b64.flush();
     \endcode
     \n
     \sa flush()
     */
    template<typename OutIt>
    void process(char_type c, OutIt& out)
    {
        m_ch[m_cidx++] = c;
        if(m_cidx < 3)
            return;
        writeBuf(out);
    }
    /*!
    Write to \p out any buffered encoded char.
     */
    template<typename OutIt>
    void flush(OutIt& out)
    {
        if(m_cidx > 0)
            writeBuf(out);
    }
};

/// Base64 decoder
/*!

 \sa encode decode
 */
class Decoder: public buffered_codec, public chainable_codec<Decoder>
{
    int m_cidx;
    char_type m_ch[4];

    template<typename OutIt>
    inline void writeBuf(OutIt& out)
    {
        if(m_cidx < 4)
        {  // malformed, missing chars will be cosidered pad 
            switch(m_cidx)
            {
            case 0:
            case 1:
                return; // ignore;
            case 2:
                m_ch[2] = m_ch[3] = eq_sign;
                break;
            case 3:
                m_ch[3] = eq_sign;
                break;
            }
        }
        m_cidx = 0;    
        *out = (m_ch[0] << 2 | ((m_ch[1] >> 4) & 0x3) ); ++out;
        if(m_ch[2] == eq_sign) return;
        *out = (m_ch[1] << 4 | ((m_ch[2] >> 2) & 0xF) ); ++out;
        if(m_ch[3] == eq_sign) return;
        *out = (m_ch[2] << 6 | m_ch[3]); ++out;
    }
public:
    /*! Constructor */
    Decoder()
    : m_cidx(0)
    {
    }
    /*! Returns the name of the codec ("Base64") */
    const char* name() const { return "Base64"; }

    /*! 
     Decodes [\p bit,\p eit) and write any decoded char to \p out.
     */
    template<typename InIt, typename OutIt>
    inline void process(InIt bit, InIt eit, OutIt out)
    {
        char_type c;

        for(; bit != eit; ++bit)
        {
            c = *bit; 
            if(c > sDecTableSz || sDecTable[c] == -1)
                continue; // malformed or newline
            m_ch[m_cidx++] = sDecTable[c]; 
            if(m_cidx < 4)
                continue;
            writeBuf(out);
        } 
        if(m_cidx > 0)
            writeBuf(out);
    }
    /*! 
     Decodes \p c and write any decoded output char to \p out.
     
     \warning You must call flush() when all chars have been 
     processed by the decode funcion.
     \n
     \code
        while( (c = getchar()) != EOF )
            b64.decode(c, out);    
        b64.flush();
     \endcode
     \n
     \sa flush()
     */
    template<typename OutIt>
    void process(char_type c, OutIt& out)
    {
        if(c > sDecTableSz || sDecTable[c] == -1)
            return; // malformed or newline
        m_ch[m_cidx++] = sDecTable[c];
        if(m_cidx < 4)
            return;
        writeBuf(out);
    }
    /*!
    Write to \p out any buffered decoded char.
     */
    template<typename OutIt>
    void flush(OutIt& out)
    {
        if(m_cidx > 0)
            writeBuf(out);
    }
};

}; // Base64

}
#endif

