/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: tree.h,v 1.5 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_TREE_H_
#define _MIMETIC_TREE_H_
#include <list>
#include <iostream>

namespace mimetic
{

/// INTERNAL: N-tree impl.
template<typename value_type>
struct TreeNode 
{
    typedef TreeNode<value_type> self_type;
    typedef std::list<TreeNode<value_type> > NodeList;
    TreeNode()
    {
    }
    TreeNode(const value_type& data)
    : m_data(data)
    {
    }
    void set(const value_type& data)
    {
        m_data = data;
    }
    value_type& get()
    {
        return m_data;
    }
    const value_type& get() const
    {
        return m_data;
    }
    NodeList& childList()
    {
        return m_nList;
    }
    const NodeList& childList() const
    {
        return m_nList;
    }
private:
    NodeList m_nList;
    value_type m_data;
};

template<typename value_type>
struct FindNodePred
{
    FindNodePred(const value_type& data)
    : m_data(data)
    {
    }
    inline bool operator()(const TreeNode<value_type>& node) const
    {
        return node.get() == m_data;
    }
private:
    value_type m_data;
};

}

#endif

