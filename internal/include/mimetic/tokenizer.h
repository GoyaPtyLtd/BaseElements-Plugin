/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: tokenizer.h,v 1.18 2008-10-07 11:44:38 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_TOKENIZER_H_
#define _MIMETIC_TOKENIZER_H_
#include <iterator>
#include <algorithm>
#include <set>
#include <string>
#include <cstring>

namespace mimetic
{

template<typename value_type>
struct IsDelim: public std::unary_function<value_type,bool>
{
    bool operator()(const value_type& val) const
    {
        return m_delims.count(val) != 0; 
    }
    template<typename Container>
    void setDelimList(const Container& cont)
    {
        typename Container::const_iterator bit, eit;
        bit = cont.begin(), eit = cont.end();
        for(; bit != eit; ++bit)
            m_delims.insert(*bit);
    }
    template<typename Iterator>
    void setDelimList(Iterator bit, Iterator eit)
    {
        for(; bit != eit; ++bit)
            m_delims.insert(*bit);
    }
    void addDelim(const value_type& value)
    {
        m_delims.insert(value);
    }
    void removeDelim(const value_type& value)
    {
        m_delims.erase(value);
    }
private:
    std::set<value_type> m_delims;
};

template<>
struct IsDelim<char>: public std::unary_function<char, bool>
{
    void setDelimList(const std::string& delims)
    {
        setDelimList(delims.begin(), delims.end());
    }
    template<typename Iterator>
    void setDelimList(Iterator bit, Iterator eit)
    {
        memset(&m_lookup, 0, sizeof(m_lookup));
        for(; bit != eit; ++bit)
            m_lookup[(int)*bit] = 1;
    }
    bool operator()(unsigned char val) const
    {
        return m_lookup[val] != 0;
    }
private:
    char m_lookup[256];
};


/// Iterator tokenizer template class
template<class Iterator,typename value_type>
class ItTokenizer
{
public:
    ItTokenizer(Iterator bit, Iterator eit)
    : m_bit(bit), m_eit(eit), m_tok_eit(bit)
    {
    }
    void setSource(Iterator bit, Iterator eit)
    {
        m_bit = bit;
        m_eit = eit;
        m_tok_eit = bit;
    }
    template<typename DelimCont>
    void setDelimList(const DelimCont& cont)
    {
        m_delimPred.setDelimList(cont);
    }
    template<typename It>
    void setDelimList(It bit, It eit)
    {
        m_delimPred.setDelimList(bit, eit);
    }
    template<typename DestCont>
    bool next(DestCont& dst)
    {
        dst.erase(dst.begin(), dst.end());
        if(m_tok_eit == m_eit)
            return false;
        m_tok_eit = std::find_if(m_bit, m_eit, m_delimPred);
        m_matched = 0; // end of input
        if(m_tok_eit != m_eit)
            m_matched = *m_tok_eit; // matched delimiter
        std::copy(m_bit, m_tok_eit, std::back_inserter<DestCont>(dst));
        m_bit = (m_tok_eit != m_eit && ++m_tok_eit != m_eit ? m_tok_eit :m_eit);
        return true;
    }
    const value_type& matched() const
    {
        return m_matched;
    }
    void addDelim(const value_type& value)
    {
        m_delimPred.addDelim(value);
    }
    void removeDelim(const value_type& value)
    {
        m_delimPred.removeDelim(value);
    }
private:
    Iterator m_bit, m_eit, m_tok_eit;
    IsDelim<value_type> m_delimPred;
    value_type m_matched;
};


/// char container tokenizer template class
template<typename Container>
struct ContTokenizer: public ItTokenizer<typename Container::const_iterator,typename Container::value_type>
{
    typedef typename Container::value_type value_type;
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;
    // i want to be fast here so i don't want to copy "cont"
    // so "cont" MUST be in scope for all following calls
    // to next(...). 
    ContTokenizer(const Container* cont)
    : ItTokenizer<const_iterator, value_type>(cont->begin(), cont->end())
    {
    }
    template<typename DelimCont>
    ContTokenizer(const Container* cont, const DelimCont& delims)
    : ItTokenizer<const_iterator,value_type>(cont->begin(), cont->end())
    {
        this->setDelimList(delims);
    }
    void setSource(const Container* cont)
    {
        ItTokenizer<const_iterator,value_type>::setSource(cont->begin(), cont->end());
    }
private:
    ContTokenizer(const ContTokenizer&);
    ContTokenizer& operator=(const ContTokenizer&);
};

/// std::string tokenizer
typedef ContTokenizer<std::string> StringTokenizer;

}

#endif

