#ifndef _MIMETIC_PARSER_ITPARSER_H_
#define _MIMETIC_PARSER_ITPARSER_H_
#include <iterator>
#include <algorithm>
#include <stack>
#include <iostream>
#include <mimetic/tree.h>
#include <mimetic/utils.h>
#include <mimetic/mimeentity.h>


// FIXME: handle HigherLevelClosingBoundary

namespace mimetic
{

/// Parse the input reading from an iterator
template<typename Iterator, 
typename ItCategory=typename std::iterator_traits<Iterator>::iterator_category> 
struct IteratorParser
{
};

/*
 * Input Iterator
 */
template<typename Iterator>
struct IteratorParser<Iterator, std::input_iterator_tag>
{

    IteratorParser(MimeEntity& me)
    : m_me(me), m_iMask(imNone), m_lastBoundary(NoBoundary)
    {
        m_entityStack.push(&m_me);
    }
    virtual ~IteratorParser()
    {
    }
    /**
     * set the Ignore Mask to \p mask
     */
    void iMask(size_t mask)    {    m_iMask = mask;        }
    /**
     * get the Ignore Mask 
     */
    size_t iMask() const    {    return m_iMask;        }
    /**
     * start parsing
     */
    void run(Iterator bit, Iterator eit)
    {
        m_bit = bit;
        m_eit = eit;
        doLoad();
    }
protected:
    typedef std::list<std::string> BoundaryList;
    enum { 
        CR = 0xD, 
        LF = 0xA, 
        NL = '\n' 
    };
    enum /* ParsingElem */ { 
        peIgnore, 
        pePreamble, 
        peBody, 
        peEpilogue 
    };
    enum BoundaryType {
        NoBoundary = 0,
        Boundary,
        ClosingBoundary,
        HigherLevelBoundary
        //, HigherLevelClosingBoundary
    };
    enum EntityType { 
        etRfc822, 
        etMsgRfc822, 
        etMultipart 
    };
    // vars
    MimeEntity& m_me;
    Iterator m_bit, m_eit;
    size_t m_iMask; // ignore mask
    BoundaryList m_boundaryList;
    BoundaryType m_lastBoundary;
    std::stack<MimeEntity*> m_entityStack;

protected:
    void appendPreambleBlock(const char* buf, int sz)
    {
        MimeEntity* pMe = m_entityStack.top();
        pMe->body().preamble().append(buf,sz);
    }
    
    void appendEpilogueBlock(const char* buf, int sz)
    {
        MimeEntity* pMe = m_entityStack.top();
        pMe->body().epilogue().append(buf,sz);
    }
    
    void appendBodyBlock(const char* buf, int sz)
    {
        MimeEntity* pMe = m_entityStack.top();
        pMe->body().append(buf, sz);
    }
    
    std::string getBoundary()
    {
        const MimeEntity* pMe = m_entityStack.top();
        const ContentType& ct = pMe->header().contentType();
        return std::string("--") + ct.param("boundary");
    }
    
    void popChild()
    {
        m_entityStack.pop();
    }
    
    void pushNewChild()
    {
        MimeEntity* pMe = m_entityStack.top();
        MimeEntity* pChild = new MimeEntity;
        pMe->body().parts().push_back(pChild);
        m_entityStack.push(pChild);
    }
    
    EntityType getType()
    {
        MimeEntity* pMe = m_entityStack.top();
        const Header& h = pMe->header();
        // will NOT be automatically created if it doesn't exists;
        // null ContentType will be returned
        const ContentType& ct = h.contentType();
        if(ct.isMultipart())
            return etMultipart;
        else if    (ct.type() == "message" && ct.subtype() == "rfc822") 
            return etMsgRfc822;
        else
            return etRfc822;
    }
    
    void addField(const std::string& name, const std::string& value)
    {
        MimeEntity* pMe = m_entityStack.top();
        Header& h = pMe->header();
        Header::iterator it = h.insert(h.end(), Field());
        it->name(name);
        it->value(value);
    }

    BoundaryType isBoundary(const std::string& line) 
    {
        if(line.length() == 0 || line[0] != '-')
            return m_lastBoundary = NoBoundary;

        int level = 0; // multipart nesting level
        int lineLen = line.length();
        BoundaryList::const_iterator bit,eit;
        bit = m_boundaryList.begin(), eit = m_boundaryList.end();
        for(;bit != eit; ++bit, ++level)
        {
            const std::string& b = *bit;
            int bLen = b.length();
            if(line.compare(0, bLen, b) == 0)
            { 
                // not the expected boundary, malformed msg
                if(level > 0)
                    return m_lastBoundary=HigherLevelBoundary;
                // plain boundary or closing boundary?
                if(lineLen > bLen && line.compare(bLen,2,"--") == 0)
                    return m_lastBoundary = ClosingBoundary;
                else
                    return m_lastBoundary = Boundary;
            }
        }
        return m_lastBoundary = NoBoundary;
    }
    // is new line
    inline bool isnl(char c) const
    {
        return (c == CR || c == LF);
    }
    // is a two char newline
    inline bool isnl(char a, char b) const
    {
        if(a == CR || a == LF)
            if(b == (a == CR ? LF : CR))
                return true;
        return false;
    }
    void doLoad()
    {
        loadHeader();
        loadBody();
    }
    bool valid() const
    {
        return m_bit != m_eit;
    }
    void append(char*& buf, size_t& bufsz, char c, size_t& pos)
    {
        enum { alloc_block = 128};
        if(pos == bufsz) 
        {
            // allocate and init buffer
            char* tmp = buf;
            int oldBufsz = bufsz;
            while(pos >= bufsz)
                bufsz = bufsz + alloc_block;
            buf = new char[bufsz+1];    
            if(tmp != 0)
            {
                assert(oldBufsz > 0);
                memset(buf, 0, bufsz);
                memcpy(buf, tmp, oldBufsz);
                delete[] tmp;
            }
        }
        buf[pos++] = c;
    }
    // parses the header and calls addField and pushChild
    // to add fields and nested entities
    void loadHeader()
    {
        enum { 
            sInit,
            sIgnoreLine,
            sNewline,
            sWaitingName, 
            sWaitingValue, 
            sWaitingFoldedValue,
            sName, 
            sValue,
            sIgnoreHeader
        };
        register int status;
        int pos;
        char *name, *value;
        size_t nBufSz, vBufSz, nPos, vPos;
        char prev, c = 0;

        name = value = 0;
        pos = nBufSz = vBufSz = nPos = vPos = 0;
        status = (m_iMask & imHeader ? sIgnoreHeader : sInit);
        //status = sInit;
        while(m_bit != m_eit)
        {
            c = *m_bit;
            switch(status)
            {
            case sInit:
                if(isnl(c))
                    status = sNewline;
                else
                    status = sName;
                continue;
            case sIgnoreLine:
                if(!isnl(c))
                    break;
                status = sNewline;
                continue;
            case sNewline:
                status = sWaitingName;
                if(pos > 0)
                {
                    pos = 0;
                    prev = c;
                    if(++m_bit == m_eit) goto out; //eof
                    c = *m_bit;
                    if(c == (prev == CR ? LF : CR))
                    {
                        --pos;
                        break;
                    } else 
                        continue;
                } else {
                    // empty line, end of header
                    prev = c;
                    if(++m_bit == m_eit) goto out; //eof
                    c = *m_bit;
                    if(c == (prev == CR ? LF : CR))
                        ++m_bit;    
                    goto out;
                }
            case sWaitingName:
                if(isblank(c))
                {
                    // folded value
                    status = sWaitingFoldedValue;
                    continue;
                } 
                // not blank, new field or empty line 
                if(nPos)
                {
                    name[nPos] = 0;
                    // is not an empty field (name: \n)
                    if(vPos) 
                    {
                        value[vPos] = 0;
                        addField(name,value);
                    } else
                        addField(name,"");
                    nPos = vPos = 0;
                }
                status = (isnl(c) ? sNewline : sName);
                continue;
            case sWaitingValue:
                if(isblank(c))
                    break; // eat leading blanks
                status = sValue;
                continue;
            case sWaitingFoldedValue:
                if(isblank(c))
                    break; // eat leading blanks
                append(value, vBufSz, ' ', vPos);
                status = sValue;
                continue;
            case sName:
                if(c > 32 && c < 127 && c != ':') {
                    if(nPos > 0 && isblank(name[nPos-1]))
                    {
                        /* "FIELDNAME BLANK+ c" found, consider that the first 
                           body line */
                        onBlock(name, nPos, peBody);
                        goto out;
                    }
                    append(name, nBufSz, c, nPos);
                } else if(c == ':') {
                    if(nPos == 0)
                    {
                        /* header line starting with ':', ignore the line */
                        status = sIgnoreLine;
                        continue;
                    }

                    /* malformed fix: remove any trailing blanks of the field 
                       name */
                    while(nPos > 0 && isblank(name[nPos-1]))
                        nPos--;

                    status = sWaitingValue;
                } else if(isblank(c)) {
                    /* blank after the field name -> malformed; it may be a 
                       malformed field with trailing blank or
                       the start of the body; save the char so we can try to 
                       recover later trimming the field name or push the
                       whole line to the body part with onBlock() */
                    append(name, nBufSz, c, nPos);
                } else {
                    /* bad header line or blank line between header and body is
                       missing; consider we're in the first line of the body */
                    onBlock(name, nPos, peBody);
                    goto out;
                }
                break;
            case sValue:
                if(isnl(c))
                {
                    status = sNewline;
                    continue;
                }
                append(value, vBufSz, c, vPos);
                break;
            case sIgnoreHeader:
                if(isnl(c))
                {
                    prev = c;
                    if(++m_bit == m_eit) goto out; //eof
                    c = *m_bit;
                    if(c == (prev == CR ? LF : CR))
                        ++m_bit;    
                    if(pos == 0)    
                        goto out; //empty line, eoh
                    pos = 0;
                    continue;
                } 
                break;
            }
            ++m_bit; ++pos;
        }
    out:
        if(name)
            delete[] name;
        if(value)
            delete[] value;
        return;
    }
    void loadBody()
    {
        switch(getType())
        {
        case etRfc822:
            if(m_iMask & imBody)
                jump_to_next_boundary();
            else
                copy_until_boundary(peBody);
            break;
        case etMultipart:
            loadMultipart();
            break;
        case etMsgRfc822:
            if(m_iMask & imChildParts)
                jump_to_next_boundary();
            else {
                pushNewChild();
                doLoad(); // load child entities
                popChild();
            }
            break;
        }
    }
    void loadMultipart()
    {
        std::string boundary = getBoundary();
        m_boundaryList.push_front(boundary);
        ParsingElem pe;
        // preamble
        pe = (m_iMask & imPreamble ? peIgnore : pePreamble );
        copy_until_boundary(pe);
        while(m_bit != m_eit)
        {
            switch(m_lastBoundary)
            {
            case NoBoundary:
                return; // eof
            case Boundary:
                if(m_iMask & imChildParts)
                    jump_to_next_boundary();
                else {
                    pushNewChild();
                    doLoad();
                    popChild();
                }
                break;
            case ClosingBoundary:
                m_boundaryList.erase(m_boundaryList.begin());
                // epilogue
                pe=(m_iMask & imEpilogue? peIgnore: peEpilogue);
                copy_until_boundary(pe);
                return;
            case HigherLevelBoundary:
                m_boundaryList.erase(m_boundaryList.begin());
                return;
            }
        }
    }
    inline void onBlock(const char* block, int sz, ParsingElem pe)
    {
        switch(pe)
        {
        case peIgnore:
            return;
        case pePreamble:
            appendPreambleBlock(block, sz);
            break;
        case peEpilogue:
            appendEpilogueBlock(block, sz);
            break;
        case peBody:
            appendBodyBlock(block, sz);
            break;
        }
    }
    void jump_to_next_boundary()
    {
        copy_until_boundary(peIgnore);
    }
    // this is where most of execution time is spent when parsing
    // large messages; I'm using a plain char[] buffer instead of
    // std::string because I want to be as fast as possible here
    virtual void copy_until_boundary(ParsingElem pe)
    {
        size_t pos, lines, eomsz = 0;
        register char c;
        enum { nlsz = 1 };
        const char *eom = 0;

        enum { blksz = 4096 };
        char block[blksz];
        size_t blkpos = 0;
        size_t sl_off = 0; // start of line offset into *block

        pos = lines = 0;
        while(m_bit != m_eit)
        {
            // if buffer is full
            if(blkpos >= blksz - 2 - nlsz)
            {
                if(sl_off == 0)
                { 
                    // very long line found, assume it 
                    // can't be a boundary and flush the buf
                    // with the partial line
                    block[blkpos] = 0;
                    onBlock(block, blkpos, pe);
                    blkpos = sl_off = 0;
                } else {
                    // flush the buffer except the last
                    // (probably incomplete) line
                    size_t llen = blkpos - sl_off;
                    onBlock(block, sl_off, pe);
                    memmove(block, block + sl_off, llen);
                    sl_off = 0;
                    blkpos = llen;
                }
            }
            c = *m_bit;
            if(isnl(c))
            {
                char nlbuf[3] = { 0, 0, 0 };

                nlbuf[0] = c; // save the current NL char in nlbuf

                // save the second char of the NL sequence (if any) in nlbuf
                if(++m_bit != m_eit) 
                {
                    char next = *m_bit;
                    if(next == (c == CR ? LF : CR))
                    {
                        nlbuf[1] = next; // save the next char in the NL seq
                        ++m_bit;
                    }
                }

                if(pos)
                {
                    // not an empty row, is this a boundary?
                    block[blkpos] = 0;
                    if(block[sl_off] == '-' && sl_off < blkpos &&
                         block[sl_off+1] == '-')
                    {
                        std::string Line(block+sl_off, blkpos-sl_off);
                        if(isBoundary(Line))
                        {
                            // trim last newline
                            if (sl_off>=2) 
                            {
                                int i = sl_off;
                                char a = block[--i];
                                char b = block[--i];

                                if(isnl(a,b))
                                    sl_off -= 2;
                                else if(isnl(a))
                                    sl_off--;

                            } else if (sl_off==1 && isnl(block[0])) {
                                sl_off--;
                            }
                            onBlock(block, sl_off, pe);
                            return;
                        }
                    }
                    // exit if this is the end of message 
                    // marker
                    if(eom && pos >= eomsz)
                    {
                        char *line = block + sl_off;
                        size_t i = 0;
                        for(; i < eomsz; i++)
                            if(eom[i] != line[i])
                                break;
                        if(i==eomsz) // if eom found
                        {
                            onBlock(block, sl_off,
                                pe);
                            return; 
                        }
                    }
                }
                // append the saved NL sequence
                for(int i = 0; nlbuf[i] != 0; i++)
                    block[blkpos++] = nlbuf[i];
                block[blkpos] = 0;
                sl_off = blkpos;
                pos = 0;
            } else {
                pos++; // line pos
                block[blkpos++] = c;
                ++m_bit; 
            }
        }
        // eof
        block[blkpos] = 0;
        onBlock(block, blkpos, pe);
    }
};


/*
 * Forward Iterator
 */
template<typename Iterator>
struct IteratorParser<Iterator, std::forward_iterator_tag>: 
    public IteratorParser<Iterator, std::input_iterator_tag>
{
    /* input_iterator ops
     * *it = xxx
     * X& op++
     * X& op++(int)
     */
    typedef IteratorParser<Iterator, std::input_iterator_tag> base_type;
    IteratorParser(MimeEntity& me)
    : base_type(me)
    {
    }
};

/*
 * Bidirectional Iterator
 */
template<typename Iterator>
struct IteratorParser<Iterator, std::bidirectional_iterator_tag>:
    public IteratorParser<Iterator, std::forward_iterator_tag>
{
    typedef IteratorParser<Iterator, std::forward_iterator_tag> base_type;
    IteratorParser(MimeEntity& me)
    : base_type(me)
    {
    }
};

/*
 * Random Access Iterator
 */
template<typename Iterator>
struct IteratorParser<Iterator, std::random_access_iterator_tag>:
    public IteratorParser<Iterator, std::bidirectional_iterator_tag>
{
    typedef IteratorParser<Iterator, std::bidirectional_iterator_tag> base_type;
    IteratorParser(MimeEntity& me)
    : base_type(me)
    {
    }
private:
    using base_type::peIgnore;
    using base_type::pePreamble;
    using base_type::peBody;
    using base_type::peEpilogue;
    
    using base_type::NoBoundary;
    using base_type::Boundary;
    using base_type::ClosingBoundary;
    using base_type::HigherLevelBoundary;
    
    using base_type::m_boundaryList;
    using base_type::m_lastBoundary;
    using base_type::m_entityStack;
    using base_type::m_me;
    using base_type::m_iMask;
    using base_type::m_bit;
    using base_type::m_eit;
    using base_type::isnl;
    
    typedef TreeNode<char> BoundaryTree;
    inline void onBlock(Iterator bit, int size, ParsingElem pe)
    {
        if(pe == peIgnore)
            return;
        Iterator eit = bit + size;
        MimeEntity* pMe = m_entityStack.top();
        switch(pe)
        {
        case pePreamble:
            pMe->body().preamble().append(bit, eit);
            break;
        case peEpilogue:
            pMe->body().epilogue().append(bit, eit);
            break;
        case peBody:
            pMe->body().append(bit, eit);
            break;
        }
    }
    void copy_until_boundary(ParsingElem pe)
    {
        // if we don't have any boundary copy until m_eit and return
        if(m_boundaryList.empty())
        {
            onBlock(m_bit, m_eit-m_bit, pe);
            m_bit = m_eit;
            return;
        }
        // search for current boundary; if not found (i.e. malformed
        // message) repeat the search for higher level boundary
        // (slow just for malformed msg, very fast otherwise)
        typename base_type::BoundaryList::const_iterator 
            bBit = m_boundaryList.begin(), bEit = m_boundaryList.end();
        m_lastBoundary = NoBoundary;
        int depth = 0;
        for( ;bBit != bEit; ++bBit, ++depth)
        {
            const std::string& boundary = *bBit;
            Iterator off;
            if( (off=utils::find_bm(m_bit,m_eit,boundary)) != m_eit)
            {
                Iterator base = m_bit;
                size_t block_sz = off - base;
                m_lastBoundary = 
                    (depth ? HigherLevelBoundary: Boundary);
                off += boundary.length();
                m_bit = off;
                if(off<m_eit-1 && *off =='-' && *(off+1) == '-')
                {
                    m_lastBoundary = ClosingBoundary;
                    m_bit = off + 2;
                }
                if(m_bit < m_eit-1 && isnl(*m_bit)) 
                {
                    char c = *m_bit++;
                    char next = *m_bit;
                    if(isnl(next) && next != c)
                        ++m_bit;
                }

                // trim last newline
                if(block_sz)
                {
                    Iterator p = base + block_sz;
                    char a = *--p, b = *--p;
                    if(isnl(a,b))
                        block_sz -= 2;
                    else if(isnl(a))
                        block_sz--;
                }
                onBlock(base, block_sz, pe);
                return;
            } else {
                onBlock(m_bit, m_eit-m_bit, pe);
                m_bit = m_eit;
            }
        }
    }
    BoundaryTree m_boundaryTree;
    void buildBoundaryTree()
    {
        m_boundaryTree = BoundaryTree(); // clear
        typename base_type::BoundaryList::const_iterator 
            bit = m_boundaryList.begin(), eit = m_boundaryList.end();
        BoundaryTree::NodeList *pChilds;
        BoundaryTree::NodeList::iterator it;
        int depth = 0;
        for( ; bit != eit; ++bit)
        {
            pChilds = &m_boundaryTree.childList();
            it = pChilds->begin();
            const char *w = bit->c_str();
            do
            {
                it = find_if(pChilds->begin(), pChilds->end(), 
                        FindNodePred<char>(*w));
                if( it == pChilds->end() )
                    it = pChilds->insert(pChilds->end(),*w);
                pChilds = &it->childList();
                depth++;
            } while(*(++w));
        }
    }

};

}

#endif
