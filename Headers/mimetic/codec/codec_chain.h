/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: codec_chain.h,v 1.13 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CODEC_CODEC_CHAIN_
#define _MIMETIC_CODEC_CODEC_CHAIN_
#include <iterator>
#include <string>
#include <mimetic/codec/codec_base.h>


namespace mimetic
{

struct null_node;

template<typename C, typename N = null_node>
struct codec_chain;


/*
 * push_back_node
 */
template<typename Node, typename LastNode>
struct push_back_node
{
    typedef    
     codec_chain<
         typename Node::content_type,
        typename
         push_back_node<
            typename Node::next_node_type, 
            LastNode
            >::node_type
        > node_type;
};

template<typename LastNode>
struct push_back_node<null_node, LastNode>
{
    typedef LastNode node_type;
};


/*
 * returns item[idx] of the Node passed to the ctor
 */
template<typename Node, unsigned int idx>
struct item
{
    typedef typename Node::next_node_type next_node_type;
    typedef typename item<next_node_type, idx-1>::node_type node_type;
    item(const Node& node)
    : m_node(node)
    {}
    const node_type& node() const
    {
        return item<next_node_type, idx-1>(m_node.m_next).node();
    }
    const typename node_type::content_type& content() const
    {
        return node().m_c;
    }

private:
    const Node& m_node;
};

template<typename Node>
struct item<Node, 0>
{
    typedef Node node_type;
    item(const Node& node)
    :m_node(node)
    {}
    const node_type& node() const
    {
        return m_node;
    }
    const typename node_type::content_type& content() const
    {
        return m_node.m_c;
    }
private:
    const Node& m_node;
};


/*
 * build push_back_node<Node,TailNode::node_type and
 * initialize it with values stored in Node
 */
template<typename Node, typename TailNode, unsigned int idx = Node::count-1>
struct build_push_back_node
{
    typedef typename item<Node,idx>::node_type nth_node_type;
    typedef typename nth_node_type::content_type nth_content_type;
    typedef codec_chain<nth_content_type,TailNode>     
        next_tail_node_type;
    typedef typename 
        build_push_back_node<Node,next_tail_node_type,idx-1>::result_node_type
        result_node_type;
    /* 
    result_node_type is equal to push_back_node<Node,TailNode>::node_type
    */
    build_push_back_node(const Node& initn, const TailNode& tailn)
    : m_initn(initn), m_tailn(tailn)
    {
    }
    operator const result_node_type() const
    {
        return get();
    }
    const result_node_type get() const
    {
        const nth_content_type& nth_c=item<Node,idx>(m_initn).content();
        next_tail_node_type next_tail(nth_c, m_tailn);
        return build_push_back_node<Node,next_tail_node_type,idx-1>(m_initn,next_tail).get();
    }
private:
    const Node& m_initn;
    const TailNode& m_tailn;
};


template<typename Node, typename TailNode>
struct build_push_back_node<Node,TailNode,0>
{
    typedef typename item<Node,0>::node_type nth_node_type;
    typedef typename nth_node_type::content_type nth_content_type;
    typedef codec_chain<nth_content_type, TailNode> next_tail_node_type;
    typedef next_tail_node_type result_node_type;

    build_push_back_node(const Node& initn, const TailNode& tailn)
    : m_initn(initn), m_tailn(tailn)
    {
    }
    operator const result_node_type() const
    {
        return get();
    }
    const result_node_type get() const
    {
        const nth_content_type& nth_c=item<Node,0>(m_initn).content();
        next_tail_node_type next_tail(nth_c, m_tailn);
        return next_tail;    
    }
private:
    const Node& m_initn;
    const TailNode& m_tailn;
};

/// Defines a chain of codecs
/*!
 Chain of codecs. <b>Don't use it directly use | operator instead</b>.

 \code
     // converts test string to upper case, replaces LF chars with
    // CRLF and encodes it using quoted-printable codec
    ToUpperCase tuc;
    Lf2CrLf l2c;
    QP::Encoder qp; 
    char buf[MAXLEN]; 

    string test("....some text here....");
    code(test.begin(), test.end(), tuc | l2c | qp, buf);
 \endcode

 \warning Chainable codecs must derive from chainable_codec<>
 \sa encode decode
 */


template<typename C, typename N>
struct codec_chain
{
    typedef codec_chain<C, N> self_type;
    typedef C content_type;
    typedef N next_node_type;
    enum { count = 1 + next_node_type::count };
    codec_chain()
    {
        setName();
    }
    codec_chain(const content_type& c)
    : m_c(c)
    {
        setName();
    }
    codec_chain(const content_type& c, const next_node_type& node)
    : m_c(c), m_next(node)
    {
        setName();
    }
    codec_chain(const codec_chain& node)
    : m_c(node.m_c), m_next(node.m_next)
    {
        setName();
    }
    codec_chain(const null_node&) 
    {
        setName();
    }
    const char* name() const
    {
        return m_name.c_str();
    }
    void process(char c)
    {
        m_c.process(c, m_next);
    }
    void flush()
    {
        m_c.flush(m_next);
        m_next.flush();
    }
    template<typename Cn>
    const Cn& get_c(int idx) const
    {
        return get_c(--idx);
    }
    const content_type& get_c(int idx) const
    {
        if(idx == 0)
            return m_c;
        else
            return get_c(--idx);
    }
    template<typename C1>
    const C1& operator[](int idx) const
    {
        if(idx == 0)
            return m_c;
        else
            return m_next[--idx];
    }
    self_type& operator*()
    {    return *this;    }
    self_type& operator=(char c)
    {
        m_c.process(c, m_next);
        return *this;
    }
    self_type& operator++()
    {    return *this;    }
    self_type& operator++(int)
    {    return *this;    }
    template<typename TailC>
    typename 
    push_back_node<self_type, codec_chain<TailC> >::node_type 
    operator|(const TailC& l)
    {
        typedef codec_chain<TailC> tail_node;
        tail_node tail = l;
        build_push_back_node<self_type, tail_node> bpbn(*this,tail);
        return bpbn.get();
    }
 //protected:
    content_type m_c;
    next_node_type m_next;
    std::string m_name;
private:
    void setName()
    {
        m_name = std::string() + m_c.name() + "|" + m_next.name();
    }
};


struct null_node
{
    enum { idx = 1 };
    enum { count = 0 };
    struct null_content
    {};
    typedef null_node self_type;
    typedef null_content content_type;
    null_node()
    {
    }
    template<typename C1, typename N1>
    null_node(const codec_chain<C1, N1>& node)
    {
    }
    const char* name() const
    {    return "null_node";    }
    self_type& operator*()
    {    return *this;    }
    self_type& operator=(char c)
    {    return *this;    }
    self_type& operator++()
    {    return *this;    }
    self_type& operator++(int)
    {    return *this;    }
    void flush()
    {
    }
    null_content m_c;
};


/*
 * helper classes useful to build codec chains
 * i.e.  node_traits<Base64,QP>::node_type
 * i.e.  node_traits<Base64,QP,Lf2CrLf>::node_type
 */
template<typename A, typename B=null_node, typename C=null_node, typename D=null_node, typename E=null_node, typename F=null_node, typename G=null_node>
struct node_traits
{
};

// class specializations...

template<typename A, typename B, typename C, typename D, typename E,typename F>
struct node_traits<A,B,C,D,E,F>
{
    typedef codec_chain<A,
        codec_chain<B,
        codec_chain<C,
        codec_chain<D,
        codec_chain<E,
        codec_chain<F> > > > > > node_type;
};

template<typename A, typename B, typename C, typename D, typename E>
struct node_traits<A,B,C,D,E>
{
    typedef codec_chain<A,
        codec_chain<B,
        codec_chain<C,
        codec_chain<D,
        codec_chain<E> > > > > node_type;
};

template<typename A, typename B, typename C, typename D>
struct node_traits<A,B,C,D>
{
    typedef codec_chain<A,
        codec_chain<B,
        codec_chain<C,
        codec_chain<D> > > > node_type;
};

template<typename A, typename B, typename C>
struct node_traits<A,B,C>
{
    typedef codec_chain<A,
        codec_chain<B,
        codec_chain<C> > > node_type;
};


template<typename A, typename B>
struct node_traits<A,B>
{
    typedef codec_chain<A,
        codec_chain<B> > node_type;
};

template<typename A>
struct node_traits<A>
{
    typedef codec_chain<A> node_type;
};


/*
 * must be the base of all chainable codecs
 */
template<typename A>
struct chainable_codec
{
    template<typename B>
    typename node_traits<A,B>::node_type
    operator|(const B& b)
    {
        typedef codec_chain<B> node_b;
        const A& a = static_cast<A&>(*this);
        return typename node_traits<A,B>::node_type(a, node_b(b));
    }
};


/*
 * operator|-creates temporary nodes to initialize chain contents
 */


#if 0
template<class A, class B>
typename node_traits<A,B>::node_type 
operator|(const A& a, const B& b)
{

    typedef codec_chain<B> node_b;
    return typename node_traits<A,B>::node_type(a, node_b(b));
}

template<typename C, typename Node, typename Last>
typename 
push_back_node<codec_chain<C, Node>, codec_chain<Last> >::node_type 
operator|(const codec_chain<C, Node>& node, const Last& l)
{
    typedef codec_chain<C,Node> InitNode;
    typedef codec_chain<Last> TailNode;
    TailNode tailnode = l;
    build_push_back_node<InitNode,TailNode> bpbn(node,tailnode);

    return bpbn.get();
}

#endif
} // namespace mimetic

#endif

