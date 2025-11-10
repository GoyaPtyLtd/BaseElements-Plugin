/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: qp.h,v 1.20 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CODEC_QP_H_
#define _MIMETIC_CODEC_QP_H_
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <mimetic/libconfig.h>
#include <mimetic/utils.h>
#include <mimetic/circular_buffer.h>
#include <mimetic/codec/codec_base.h>
#include <mimetic/codec/codec_chain.h>

namespace mimetic
{

class QP
{
    friend class test_qp;
    enum { LF = 0xA, CR = 0xD, NL = LF, TAB = 9, SP = 32 };
    enum { default_maxlen = 76 };
    enum { 
        printable,  /* print as-is */
        tab,        /* print if !isBinary */
        sp,         /* ' ' */
        newline,    /* cr or lf; encode if isBinary*/    
        binary,     /* rest of the ascii map */
        unsafe      /* "!\"#$@[]\\^`{}|~" */
    };
    static char sTb[256];

public:

/// quoted-printable encoder
/*!

 \sa encode decode
 */
class Encoder: public buffered_codec, public chainable_codec<Encoder>
{
    enum { laBufSz = 5 }; // look-ahead buffer
    size_t m_pos, m_maxlen;
    bool m_binary;
    circular_buffer<char_type> m_cbuf;

    template<typename OutIt>
    void hardLineBrk(OutIt& out)
    {
        *out = NL; ++out;
        m_pos = 1;
    }
    template<typename OutIt>
    void softLineBrk(OutIt& out)
    {
        *out = '='; ++out;
        hardLineBrk(out);
    }
    template<typename OutIt>
    void write(char_type ch, OutIt& out)
    {
        bool is_last_ch = m_cbuf.empty();
        if(!is_last_ch && m_pos == m_maxlen)
            softLineBrk(out);
        *out = ch; ++out;
        m_pos++;
    }
    template<typename OutIt>
    void writeHex(char_type ch, OutIt& out)
    {
        static char_type hexc[] =
        { 
            '0', '1', '2', '3', '4', '5' ,'6', '7', '8', '9',
            'A', 'B', 'C', 'D', 'E', 'F'
        };        
        bool is_last_ch = m_cbuf.empty();
        if(m_pos + (is_last_ch ? 1 : 2) >= m_maxlen)
            softLineBrk(out);
        // write out =HH
        *out = '='; ++out;
        *out = hexc[ch >> 4]; ++out;
        *out = hexc[ch & 0xf]; ++out;
        m_pos += 3;
    } 
    template<typename OutIt>
    void encodeChar(char_type c, OutIt& out)
    {
        int cnt = m_cbuf.count();
        switch(sTb[c])
        {
        case printable:
            if(m_pos == 1)
            {
                switch(c)
                {
                case 'F': // hex enc on "^From .*"
                    if(cnt>=4 && m_cbuf.compare(0,4,"rom "))
                    {
                        writeHex(c,out);
                        return;
                    }
                    break;
                case '.': // hex encode if "^.[\r\n]" or on eof
                    if(!cnt || sTb[ m_cbuf[0] ] == newline)
                    {
                        writeHex(c,out);
                        return;
                    }
                    break;
                }
            } 
            write(c,out);
            break;
        case tab:
        case sp:
            // on binary encoding, or last input ch or newline
            if(m_binary || !cnt || sTb[ m_cbuf[0] ] == newline)
                writeHex(c,out);
            else
                write(c,out);
            break;
        case newline:
            if(m_binary)
                writeHex(c, out);
            else {
                if(cnt && m_cbuf[0] == (c == CR ? LF : CR))
                    m_cbuf.pop_front(); // eat it 
                hardLineBrk(out);
            }
            break;
        case binary:
            if(!m_binary) m_binary = 1; // switch to binary mode
            writeHex(c, out);
            break;
        case unsafe:
            writeHex(c, out);
            break;
        }
    }
public:
    /*! return the multiplier of the required (max) size of the output buffer 
     * when encoding */
    double codeSizeMultiplier() const
    {
        // worse case is *3 but we'll use the (euristic) average value of 1.5.
        // this may decrease performance when encoding messages with many 
        // non-ASCII (> 127) characters 
        return 1.5;
    }
    /*!
     Constructor
     \param isBinary if true all space and newline characters will be
     treated like binary chars and will be hex encoded (useful if you
     want to encode a binary file).
     */
    Encoder(bool isBinary = false)
    : m_pos(1), m_maxlen(default_maxlen), 
      m_binary(isBinary), m_cbuf(laBufSz) 
    {
    }
    /*! Returns the name of the codec ("Quoted-Printable") */
    const char* name() const { return "Quoted-Printable"; }
    /*! Returns the max line length */
    size_t maxlen()
    {
        return m_maxlen;
    }
    /*! 
        Set the max line length. No more then \p i chars will be 
        printed on one line.
    */
    void maxlen(size_t i)
    {
        m_maxlen = i;
    }
    /*! 
     Encodes [\p bit,\p eit) and write any encoded char to \p out.
     */
    template<typename InIt, typename OutIt>
    void process(InIt bit, InIt eit, OutIt out)
    {
        for(; bit != eit; ++bit)
            process(*bit, out);
        flush(out);
    }
    /*! 
     Encodes \p ic and write any encoded output char to \p out.
     \warning You must call flush() when all chars have been 
     processed by the encode funcion.
     \n
     \code
        while( (c = getchar()) != EOF )
            qp.process(c, out);    
        qp.flush();
     \endcode
     \n
     \sa flush()
     */
    template<typename OutIt>
    void process(char_type ic, OutIt& out)
    {
        m_cbuf.push_back(ic);
        if(m_cbuf.count() < laBufSz)
            return;
        char_type c = m_cbuf.front();
        m_cbuf.pop_front();
        encodeChar(c, out);
    }
    /*!
    Write to \p out any buffered encoded char.
     */
    template<typename OutIt>
    void flush(OutIt& out)
    {
        char_type c;
        while(!m_cbuf.empty())
        {
            c = m_cbuf.front();
            m_cbuf.pop_front();
            encodeChar(c, out);
        }
    }
};

/// quoted-printable decoder
/*!

 \sa encode decode
 */
class Decoder: public buffered_codec, public chainable_codec<Encoder>
{
    enum { laBufSz = 80 }; // look-ahead buffer
    enum {
        sWaitingChar,
        sAfterEq,
        sWaitingFirstHex,
        sWaitingSecondHex,
        sBlank,
        sNewline,
        sOtherChar
    };
    size_t m_pos, m_maxlen;


    int m_state, m_nl;
    std::string m_prev;

    template<typename OutIt>
    void hardLineBrk(OutIt& out) const
    {
        *out = NL; ++out;
    }
    template<typename OutIt>
    void write(char_type ch, OutIt& out) const
    {
        *out = ch; ++out;
    }
    bool isnl(char_type c) const
    {
        return (c == CR || c == LF);
    }
    template<typename OutIt>
    void flushPrev(OutIt& out)
    {
        copy(m_prev.begin(), m_prev.end(), out);
        m_prev.clear();
    }
    int hex_to_int(char_type c) const
    {
        if( c >= '0' && c <='9') return c - '0';
        else if( c >= 'A' && c <='F') return c - 'A' + 10;
        else if( c >= 'a' && c <='f') return c - 'a' + 10;
        else return 0;
    }
    bool ishex(char_type c) const
    {
        return  (c >= '0' && c <= '9') || 
            (c >= 'A' && c <= 'F') || 
            (c >= 'a' && c <= 'f');
    }
    template<typename OutIt>
    void decodeChar(char_type c, OutIt& out)
    {
        for(;;)
        {
            switch(m_state)
            {
            case sBlank:
                if(isblank(c))
                    m_prev.append(1,c);
                else if(isnl(c)) {
                    // soft linebrk & ignore trailing blanks
                    m_prev.clear(); 
                    m_state = sWaitingChar;
                } else {
                    flushPrev(out);
                    m_state = sWaitingChar;
                    continue;
                }
                return;
            case sAfterEq:
                if(isblank(c))
                    m_prev.append(1,c);
                else if(isnl(c)) {
                    // soft linebrk 
                    m_state = sNewline;
                    continue;
                } else {
                    if(m_prev.length() > 1) 
                    {
                        // there're blanks after =
                        flushPrev(out);
                        m_state = sWaitingChar;
                    } else
                        m_state = sWaitingFirstHex;
                    continue;
                }
                return;
            case sWaitingFirstHex:
                if(!ishex(c))
                {
                    // malformed: =[not-hexch]
                    flushPrev(out);
                    write(c, out);
                    m_state = sWaitingChar;
                    return;
                } else {
                    m_prev.append(1,c);
                    m_state = sWaitingSecondHex;
                }
                return;
            case sWaitingSecondHex:
                if(!ishex(c))
                { // malformed (=[hexch][not-hexch])
                    flushPrev(out);
                    write(c, out);
                } else {
                    char_type oc, last;
                    assert(m_prev.length());
                    last = m_prev[m_prev.length()-1];
                    oc = hex_to_int(last) << 4 | 
                        hex_to_int(c) ;
                    write(oc,out);
                    m_prev.clear();
                }
                m_state = sWaitingChar;
                return;
            case sNewline:
                if(m_nl == 0)
                {
                    m_nl = c;
                    return;
                } else {
                    int len = m_prev.length();
                    if(!len || m_prev[0] != '=')
                        hardLineBrk(out);
                    m_prev.clear();
                    m_state = sWaitingChar;
                    bool is2Ch;
                    is2Ch = (c == (m_nl == CR ? LF : CR));
                    m_nl = 0;
                    if(is2Ch)
                        return;
                    continue;
                }
            case sWaitingChar:
                if(isblank(c))
                {
                    m_state = sBlank;
                    continue;
                } else if(isnl(c)) {
                    m_state = sNewline;
                    continue;
                } else if(c == '=') {
                    m_state = sAfterEq;
                    m_prev.append(1, c);
                    return;
                } else {
                    // WARNING: NOT ignoring chars > 126
                    // as suggested in rfc2045 6.7 note 4
                    if(c < 32 && c != TAB)
                    {
                        // malformed, CTRL ch found
                        // ignore (rfc2045 6.7 note 4)
                        return;
                    }
                    write(c,out);
                }
                return;
            }
        }
    }
public:
    /*! Constructor */
    Decoder()
    : m_state(sWaitingChar), m_nl(0)
    {
    }
    /*! Returns the name of the codec ("Quoted-Printable") */
    const char* name() const { return "Quoted-Printable"; }
    /*! Returns the max line length */
    size_t maxlen()
    {
        return m_maxlen;
    }
    /*! 
    Set the max line length. No more then \p i chars will be 
    printed on one line.
    */
    void maxlen(size_t i)
    {
        m_maxlen = i;
    }
    /*! 
     Decodes [\p bit,\p eit) and write any decoded char to \p out.
     */
    template<typename InIt, typename OutIt>
    void process(InIt bit, InIt eit, OutIt out)
    {
        for(;bit != eit; ++bit)
            decodeChar(*bit, out);
        flush(out);
    }
    /*! 
     Decodes \p ic and write any decoded output char to \p out.
     
     \warning You must call flush() when all chars have been 
     processed by the code(...) funcion.
     \n
     \code
        while( (c = getchar()) != EOF )
            qp.process(c, out);    
        qp.flush();
     \endcode
     \n
     \sa flush()
     */
    template<typename OutIt>
    void process(char_type ic, OutIt& out)
    {
        decodeChar(ic, out);
    }
    /*!
    Write to \p out any buffered decoded char.
     */
    template<typename OutIt>
    void flush(OutIt& out)
    {
        /* m_prev can be (regex):
            empty: 
                ok
            '=' : 
              malformed, '=' is last stream char, print as is
              (rfc2045 6.7 note 3)
            '=[a-zA-Z]'
              malformed, print as is
              (rfc2045 6.7 note 2)
            '= +'
              malformed, just print '=' and ignore trailing
              blanks (rfc2045 6.7 (3) )
        */
        int len = m_prev.length();
        if(len)
        {
            if(len == 1)
            {
                /* malformed if m_prev[0] == '=' */
                write('=', out);
            } else {
                write('=', out);
                if(m_prev[1] != ' ')
                    write(m_prev[1], out);
            }
        } else if(m_nl != 0) // stream ends with newline
            hardLineBrk(out);

    }
};

};


} // namespace

#endif

