/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: circular_buffer.h,v 1.8 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CODEC_CIRCULAR_BUFFER_H_
#define _MIMETIC_CODEC_CIRCULAR_BUFFER_H_
#include <string>
#include <iostream>

namespace mimetic
{

template<typename T>
struct circular_buffer
{
    typedef circular_buffer<T> self_type;
    typedef T value_type;
    typedef unsigned int size_type;
    circular_buffer(unsigned int sz = 4)
    : m_sz(sz), m_count(0), m_first(0), m_last(0)
    {
        m_pItem = new value_type[sz];
    }
    ~circular_buffer()
    {
        delete[]  m_pItem;
    }
    circular_buffer(const circular_buffer& r)
    : m_sz(r.m_sz), m_count(r.m_count),
      m_first(r.m_first) ,m_last(r.m_last)
    {
         m_pItem = new value_type[m_sz];
        for(size_type i =0; i < m_sz; i++)
            m_pItem[i] = r.m_pItem[i];
    }
    circular_buffer& operator=(const circular_buffer& r)
    {
        m_sz = r.m_sz;
        m_count = r.m_count;
          m_first = r.m_first;
        m_last = r.m_last;

        if(m_pItem)
            delete[] m_pItem;
         m_pItem = new value_type[m_sz];
        for(size_type i =0; i < m_sz; i++)
            m_pItem[i] = r.m_pItem[i];
        return *this;
    }
    inline void push_back(const value_type& c)
    {
        m_pItem[m_last] = c;    
        m_last = ++m_last % m_sz;
        m_count += (m_count == m_sz ? 0 : 1);
    }
    inline void push_front(const value_type& c)
    {
        m_first = (--m_first + m_sz) % m_sz;        
        m_pItem[m_first] = c;    
        m_count += (m_count == m_sz ? 0 : 1);
    }
    inline void pop_front()
    {
        m_first = ++m_first % m_sz;        
        m_count--;
    }
    inline void pop_back()
    {
        m_last = (--m_last + m_sz) % m_sz;
        m_count--;
    }
    inline const value_type& front() const
    {
        return m_pItem[m_first];
    }
    inline const value_type& back() const
    {
        int last = (m_last -1 + m_sz) % m_sz;
        return m_pItem[last];
    }
    inline bool operator==(const std::string& r) const
    {
        if(m_count < r.length())
            return false;
        const self_type& me = *this;
        for(size_type i = 0; i < m_count; i++)
            if(me[i] != r[i])
                return false;
        return true;
    }
    inline bool operator!=(const std::string& r) const
    {
        return !operator==(r);
    }    
    bool compare(size_type off, size_type n0, const std::string& r) const
    {
        const self_type& me = *this;
        for(size_type i = 0; i < n0; i++)
            if(me[off+i] != r[i])
                return false;
        return true;
    }
    inline value_type& operator[](unsigned int i) const
    {
        unsigned int idx = (m_first + i) % m_sz;
        return m_pItem[idx];
    }
    inline bool empty() const
    {
        return m_count == 0;
    }
    std::string str() const
    {
        std::string result;
        const self_type& me = *this;
        for(size_type i = 0; i < m_count; i++)
            result += me[i];
        return result;
    }
    inline size_type count() const
    {
        return m_count;
    }
    inline size_type max_size() const
    {
        return m_sz;
    }
private:
    size_type m_sz, m_count;
    int m_first, m_last;
    value_type* m_pItem;
};

}

#endif

