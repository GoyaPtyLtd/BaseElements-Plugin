/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: streambufs.h,v 1.7 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_MIMESTREAMBUF_H_
#define _MIMETIC_MIMESTREAMBUF_H_
#include <iostream>
#include <string>
#include <mimetic/libconfig.h>
#include <mimetic/strutils.h>

namespace mimetic
{


struct read_streambuf: public std::streambuf
{
    enum { bufsz = 512 };
    typedef unsigned int size_type;
    read_streambuf()
    : m_iBuf(new char_type[bufsz])
    {
        setg(m_iBuf, m_iBuf + bufsz, m_iBuf + bufsz);
    }
    virtual ~read_streambuf()
    {
        if(m_iBuf)
            delete[] m_iBuf;
        m_iBuf = 0;
    } 
    int_type underflow()
    {
        int bread;

        if(gptr() < egptr())
            return traits_type::to_int_type(*gptr());

        if((bread = read(eback(), bufsz)) == 0)
            return traits_type::eof();
        else
            setg(eback(), eback(), eback() + bread);

        return traits_type::to_int_type(*gptr());
    }
    // must return number of bytes read or 0 on eof
    virtual int_type read(char*, int) = 0;
private:
    read_streambuf(const read_streambuf&);
    read_streambuf& operator=(const read_streambuf&);
    char_type* m_iBuf;
};


template<typename InputIt>
struct inputit_streambuf: public read_streambuf
{
    inputit_streambuf(InputIt beg, InputIt end)
    : m_beg(beg), m_end(end)
    {
    }
    // returns number of  bytes read or 0 on eof
    int_type read(char* buf, int bufsz)
    {
        // fill buffer
        int c;
        for(c = 0; m_beg != m_end && c < bufsz; ++m_beg, ++buf, ++c)
            *buf = *m_beg;
        return c;
    }
private:
    InputIt m_beg, m_end;
};

struct transform_streambuf: public std::streambuf
{
    typedef unsigned int size_type;
    transform_streambuf()
    : m_oBuf(new char_type[512])
    {
        setp(m_oBuf, m_oBuf + 512);
    }
    virtual ~transform_streambuf()
    {
        if(m_oBuf)
        {
            sync();
            delete[] m_oBuf;
        }
    }
    int overflow(int meta = EOF)
    {
        if(sync() == -1)
            return EOF;
        if(meta != EOF)
        {
            *pptr() = meta;
            pbump(1);
        }
        return meta;
    }
    int sync()
    {
        int toSend = pptr() - pbase();
        if(toSend)
        {
            write(pbase(), pbase() + toSend);
            setp(m_oBuf, epptr());
        }
        return 0;
    }
    virtual void write(const char_type* beg, const char_type* end)=0;
private:
    transform_streambuf(const transform_streambuf&);
    transform_streambuf& operator=(const transform_streambuf&);
    char_type* m_oBuf;
};

/*
 * stream buffer that does nothing except counting character written into it.
 * characters count is available through the size() method
 */
struct count_streambuf: public transform_streambuf
{
    count_streambuf()
    : m_count(0)
    {
    }
    void write(const char_type* beg, const char_type* end)
    {
        int toSend = end - beg;
        if(toSend)
            m_count += toSend;
    }
    size_type size()
    {
        return m_count;
    }
private:
    size_type m_count;
};



/*
 * stream buffer that count char written into it and copy every char to the 
 * output iterator passed as ctor parameter
 * characters count is available through the size() method
 */
template<typename OutputIt>
struct passthrough_streambuf: public transform_streambuf
{
    typedef unsigned int size_type;
    passthrough_streambuf(const OutputIt& out)
    : m_out(out), m_count(0)
    {
    }
    void write(const char_type* beg, const char_type* end)
    {
        int toSend = end - beg;
        if(toSend)
        {
            m_count += toSend;
            copy(beg, end, m_out);
        }
    }
    size_type size()
    {
        return m_count;
    }
private:
    OutputIt m_out;
    size_type m_count;
};



struct crlftolf_streambuf: public transform_streambuf
{
    typedef unsigned int size_type;
    crlftolf_streambuf(std::streambuf* osbuf)
    : m_osbuf(osbuf) 
    {
    }
    void write(const char_type* beg, const char_type* end)
    {
        enum { cr = 0xD, lf = 0xA };
        char_type c;
        bool got_cr = 0;
        for(; beg != end; ++beg)
        {
            c = *beg;
            if(got_cr)
            {
                if(c == lf)
                    m_osbuf->sputc(lf);
                else {
                    m_osbuf->sputc(cr);
                    m_osbuf->sputc(c);
                }
                got_cr = 0;
            } else if(c == cr) {
                got_cr = 1;
                continue;
            } else 
                m_osbuf->sputc(c);
        }
        if(got_cr)
            m_osbuf->sputc(c);
    }
private:
    std::streambuf* m_osbuf;
};


}

#endif
