/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: utils.h,v 1.23 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_UTILS_H_
#define _MIMETIC_UTILS_H_
#include <iostream>
#include <string>
#include <ctype.h>
#include <mimetic/libconfig.h>
#include <mimetic/strutils.h>

namespace mimetic
{

std::ostream& crlf(std::ostream&);
std::ostream& nl(std::ostream&);

#ifndef isblank
inline int isblank(char c)
{
    return c == ' ' || c == '\t';
}
#endif

namespace utils
{

/// returns the filename out of the fqn (fully qualified name) 
std::string extractFilename(const std::string&);

/// returns a string representation of \p n
std::string int2str(int n);

/// return true if the string contains just blanks (space and tabs)
bool string_is_blank(const std::string&);

/// returns the integer value represented by \p s
int str2int(const std::string& s);

/// returns a string hexadecimal representation of \p n
std::string int2hex(unsigned int n);

// find_bm specialization for random access iterators
template<typename Iterator>
Iterator find_bm(Iterator bit, Iterator eit, const std::string& word, const std::random_access_iterator_tag&)
{
    int bLen = word.length();
    const char* pWord = word.c_str();
    int i, t, shift[256];
    unsigned char c;

    for(i = 0; i < 256; ++i)  
        shift[i] = bLen;

    for(i = 0; i < bLen; ++i)
        shift[ (unsigned char) pWord[i] ] = bLen -i - 1;

    for(i = t = bLen-1; t >= 0; --i, --t)
    {
        if((bit + i) >= eit)
            return eit; 

        while((c = *(bit + i)) != pWord[t]) 
        {
            i += std::max(bLen-t, shift[c]);
            if((bit + i) >= eit) return eit; 
            t = bLen-1;
        }
    }

    return bit + i + 1;
}

// boyer-moore find 
/**
 * find the first occurrence of \p word in (\p bit, \p eit]
 *
 * returns an Iterator pointing at the first character of the found pattern
 * or \p eit if the search fails
 */
template<typename Iterator>
Iterator find_bm(Iterator bit, Iterator eit, const std::string& word)
{
    return find_bm(bit, eit, word, 
        typename std::iterator_traits<Iterator>::iterator_category());
}



} // ns utils

}

#endif
