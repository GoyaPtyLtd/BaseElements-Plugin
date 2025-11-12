/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: mimeentity.h,v 1.29 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_MIMEENTITY_H_
#define _MIMETIC_MIMEENTITY_H_
#include <string>
#include <iostream>
#include <streambuf>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <mimetic/strutils.h>
#include <mimetic/utils.h>
#include <mimetic/contenttype.h>
#include <mimetic/contenttransferencoding.h>
#include <mimetic/contentdisposition.h>
#include <mimetic/mimeversion.h>
#include <mimetic/mimeentitylist.h>
#include <mimetic/codec/codec.h>
#include <mimetic/os/file.h>
#include <mimetic/header.h>
#include <mimetic/body.h>
#include <mimetic/parser/itparserdecl.h>
#include <mimetic/streambufs.h>


namespace mimetic
{

class MimeEntity;


/// Represent a MIME entity    
class MimeEntity
{
    friend class Body;
    friend class MimeEntityLoader;
    typedef std::list<std::string> BoundaryList;
    typedef unsigned long int size_type;
public:
    /**
     *  Blank MIME entity
     */
    MimeEntity();
    /**
     *  Parse [beg, end] and build entity based on content
     */
    template<typename Iterator>
    MimeEntity(Iterator beg, Iterator end, int mask = imNone);
    /**
     *  Parse istream and build entity based on content
     */
    MimeEntity(std::istream&);

    virtual ~MimeEntity();

    /**
     * copy text rapresentation of the MimeEntity to the output iterator
     */
    template<typename OutputIt>
    size_type copy(OutputIt out);

    Header& header();
    const Header& header() const;

    Body& body();
    const Body& body() const;

    /** 
     * single step load functions: parse the input provided and build the
     * entity
     *
     * use load(..., mask) to ignore some part of the message when it's
     * not needed saving memory space and execution time
     */
    template<typename Iterator>
    void load(Iterator, Iterator, int mask = imNone);
    void load(std::istream&, int mask = imNone);

    /**
     * helper functions: return header().hasField(str)
     */
    bool hasField(const std::string&) const;

    /**
     * returns entity size 
     * Note: this function is slow, use it if you really need
     */
    size_type size() const;
    friend std::ostream& operator<<(std::ostream&, const MimeEntity&);
protected:
    void commonInit();

    virtual std::ostream& write(std::ostream&, const char* eol = 0) const;

protected:
    Header m_header;
    Body m_body;
    size_type m_lines;
    size_type m_size;

private:
    //MimeEntity(const MimeEntity&);
    //MimeEntity& operator=(const MimeEntity&);
};



template<typename Iterator>
MimeEntity::MimeEntity(Iterator bit, Iterator eit, int mask)
{
    commonInit();
    load(bit, eit, mask);
}


template<typename Iterator>
void MimeEntity::load(Iterator bit, Iterator eit, int mask)
{
    IteratorParser<Iterator, 
        typename std::iterator_traits<Iterator>::iterator_category> prs(*this);
    prs.iMask(mask);
    prs.run(bit, eit);
}

template<typename OutputIt>
MimeEntity::size_type MimeEntity::copy(OutputIt out)
{
    passthrough_streambuf<OutputIt> psb(out);
    std::ostream os(&psb);
    os << *this;
    return psb.size();
}

}

#endif
