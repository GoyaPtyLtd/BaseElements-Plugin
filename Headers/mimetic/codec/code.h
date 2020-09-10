/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: code.h,v 1.5 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CODEC_CODE_H_
#define _MIMETIC_CODEC_CODE_H_
#include <mimetic/codec/codec_base.h>
#include <mimetic/codec/codec_chain.h>
#include <mimetic/codec/other_codecs.h>
#include <mimetic/utils.h>

namespace mimetic
{



template<typename InIt, typename OutIt, typename Codec>
void code(InIt beg, InIt end, Codec& cc, OutIt out)
{
    typedef typename Codec::codec_type codec_type;
    code(beg, end, cc, out, codec_type());
}

// code func for buffered codecs 
template<typename InIt, typename OutIt, typename Codec>
void code(InIt beg, InIt end, Codec& cc, OutIt out,const buffered_codec_type_tag&)
{ 
    for(; beg != end; ++beg)
        cc.process(*beg, out);
    cc.flush(out);
}

// code func for unbuffered codecs 
template<typename InIt, typename OutIt, typename Codec>
void code(InIt beg, InIt end,Codec& codec,OutIt out,const unbuffered_codec_type_tag&)
{
    for(; beg != end; ++beg)
        codec.process(*beg, out);
}

// code func for chained codecs
template<typename InIt, typename OutIt, typename Codec, typename Next>
void code(InIt beg, InIt end, const codec_chain<Codec,Next>& cc, OutIt out)
{
    typedef codec_chain<Codec,Next> Node1;
    typedef codec_chain< oiterator_wrapper<OutIt> > 
        TailNode;
    typedef typename push_back_node<Node1, TailNode>::node_type 
        codec_chain_type;

    oiterator_wrapper<OutIt> oiw(out);
    codec_chain_type chain = build_push_back_node<Node1,TailNode>(cc,TailNode(oiw));

    for(; beg != end; ++beg)
        chain.process(*beg);
    chain.flush();
}

/// Encodes (beg, end] using \p cc codec 
/*!
    Encodes (beg, end] using \p cc codec and write any
    output characters to the output iterator \p out.

    \p cc can be a simple codec:
    \code
        Base64::Encoder b64;
        code(beg, end, b64, out);
    \endcode
    or a chain of codecs:
    \code
        Base64::Encoder b64;
        ToUpperCase tuc;
        code(beg, end, tuc | b64, out);
    \endcode
 */
template<typename InIt, typename OutIt, typename Codec>
void encode(InIt beg, InIt end, Codec& cc, OutIt out)
{
    code(beg, end, cc, out);
}

/// decodes (beg, end] using \e cc codec and write any
/*!
    decodes (beg, end] using \e cc codec and write any
    output characters to the output iterator \e out
 */
template<typename InIt, typename OutIt, typename Codec>
void decode(InIt beg, InIt end, Codec& cc, OutIt out)
{
    code(beg, end, cc, out);
}


template<typename InIt, typename OutIt, typename Codec, typename Next>
void encode(InIt beg, InIt end, const codec_chain<Codec,Next>& cc, OutIt out)
{
    code(beg,end,cc,out);
}

template<typename InIt, typename OutIt, typename Codec, typename Next>
void decode(InIt beg, InIt end, const codec_chain<Codec,Next>& cc, OutIt out)
{
    code(beg,end,cc,out);
}

} 



#endif 






