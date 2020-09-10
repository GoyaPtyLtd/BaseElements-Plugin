/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: strutils.h,v 1.10 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_STRINGUTILS_H_
#define _MIMETIC_STRINGUTILS_H_
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cstring>

namespace mimetic
{

extern const std::string nullstring;

struct ichar_traits : public std::char_traits<char>
{
    static bool eq (const char_type & c1, const char_type& c2)
    {    return (toupper(c1) == toupper(c2));    }
    static bool ne (const char_type& c1, const char_type& c2)
    {    return (toupper(c1) != toupper(c2));    }
    static bool lt (const char_type& c1, const char_type& c2)
    {    return (toupper(c1) < toupper(c2));    }
    static int compare (const char_type* s1, const char_type* s2, size_t n)
    {
        for(size_t i=0; i < n; ++i)
            if(toupper(s1[i]) != toupper(s2[i]))
                return (toupper(s1[i]) < toupper(s2[i])) ?-1: 1;
        return 0;
    }
    static const char* find( const char* s, int n, char a ) 
    {
        while( n-- > 0 && tolower(*s) != tolower(a) ) 
                         ++s;
        return s;
    }
};

//typedef std::istring <char, ichar_traits> istring;
using std::string;

struct istring: public string
{
    istring()
    {}
    //typedef std::string::allocator_type allocator_type;
    istring(const std::string& right)
    : string(right)
    {}
    explicit istring(const allocator_type& al)
    : string(al)
    {}
    istring(const istring& right)
    : string(right)
    {}
    istring(const istring& right, size_type roff, size_type count = npos)
    : string(right, roff, count)
    {}
    istring(const istring& right, size_type roff, size_type count, 
        const allocator_type& al)
    : string(right, roff, count, al)
    {}
    istring(const value_type *ptr, size_type count)
    : string(ptr, count)
    {}
    istring(const value_type *ptr, size_type count,const allocator_type& al)
    : string(ptr, count, al)
    {}
    istring(const value_type *ptr)
    : string(ptr)
    {}
    istring(const value_type *ptr,const allocator_type& al)
    : string(ptr, al)
    {}
    istring(size_type count, value_type ch)
    : string(count,ch)
    {}
    istring(size_type count, value_type ch,const allocator_type& al)
    : string(count,ch,al)
    {}
    template <class InIt>
    istring(InIt first, InIt last)
    : string(first, last)
    {}
    template <class InIt>
    istring(InIt first, InIt last,const allocator_type& al)
    : string(first, last, al)
    {}
};


inline bool operator==(const istring& is, const std::string& s)
{
    return (0 == ichar_traits::compare(is.c_str(),s.c_str(),
            std::max(is.length(),s.length())) );
}

inline bool operator!=(const istring& is, const std::string& s)
{
    return (0 != ichar_traits::compare(is.c_str(),s.c_str(),
            std::max(is.length(),s.length())) );
}

inline bool operator!=(const istring& is, const char* str)
{
    return (0 != ichar_traits::compare(is.c_str(),str,
            std::max(is.length(),::strlen(str))) );
}

inline bool operator==(const istring& is, const char* str)
{
    return (0 == ichar_traits::compare(is.c_str(),str,
            std::max(is.length(),::strlen(str))) );
}

inline std::string dquoted(const std::string& s)
{    
    return "\"" + s + "\"";
}

inline std::string parenthed(const std::string& s)
{    
    return "(" + s + ")";
}

/// removes double quotes
inline std::string remove_dquote(const std::string& s)
{
    int len = s.length();
    if( len < 2)
        return s;
    if(s[0] == '"' && s[len-1] == '"')
        return std::string(s, 1, len-2);
    return s;
}

/**
 * returns the \e canonical representation of \p s (see RFC822)
 * if \p no_ws is true removes all blanks from the resulting string
 */
std::string canonical(const std::string& s, bool no_ws = false);

/// removes leading and trailing blanks
inline std::string remove_external_blanks(const std::string& in)
{
    if(!in.length())
        return in;
    std::string s = in;
    int beg = 0, end = s.length();
    for(; beg < end; ++beg)
        if(s[beg] != ' ' && s[beg] != '\t')
            break;
    end = s.length() - 1;
    for(; end > beg; --end)
        if(s[end] != ' ' && s[end] != '\t')
            break;
    s.assign(std::string(s, beg, end - beg + 1));
    return s;
}

}

#endif

