/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: codec_base.h,v 1.13 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CODEC_CODECBASE_H_
#define _MIMETIC_CODEC_CODECBASE_H_
namespace mimetic
{


struct buffered_codec_type_tag
{
};

struct unbuffered_codec_type_tag
{
};


/// Codecs base class
struct codec
{
    typedef unsigned char char_type;
    virtual ~codec() {}
    virtual const char* name() const = 0;

    /*! return the multiplier of the required (max) size of the output buffer 
     * when encoding */
    virtual double codeSizeMultiplier() const { return 1.0; }
};



/// Base class for unbuffered codecs
struct unbuffered_codec: public codec
{
    typedef unbuffered_codec_type_tag codec_type;
    template<typename OutIt>
    void flush(OutIt&)
    {
    }
};

/// Base class for buffered codecs
struct buffered_codec: public codec
{
    typedef buffered_codec_type_tag codec_type;
};


}

#endif

