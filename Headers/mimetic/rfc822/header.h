/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: header.h,v 1.16 2008-10-07 11:06:27 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_RFC822_HEADER_H_
#define _MIMETIC_RFC822_HEADER_H_
#include <string>
#include <deque>
#include <cassert>
#include <functional>
#include <iostream>
#include <mimetic/strutils.h>
#include <mimetic/utils.h>
#include <mimetic/rfc822/field.h>
#include <mimetic/rfc822/mailbox.h>
#include <mimetic/rfc822/messageid.h>
#include <mimetic/rfc822/mailboxlist.h>
#include <mimetic/rfc822/addresslist.h>

namespace mimetic
{


/// RFC822 header class object
/*!
    Use this class to build or parse message header fields.
    This is a STL container so you can browse fields using iterators(see ex. below).

    \sa <a href="../RFC/rfc822.txt">RFC822</a>
 */
class Rfc822Header: public std::deque<Field>
{
public:
    struct find_by_name: 
        public std::unary_function<const Field, bool>
    {
        find_by_name(const std::string&);
        bool operator()(const Field&) const;
    private:
        const istring m_name;
    };

    bool hasField(const std::string&) const;
    
    const Field& field(const std::string&) const;
    Field& field(const std::string&);

    const Mailbox& sender() const;
    Mailbox& sender();
    void sender(const Mailbox&);

    const MailboxList& from() const;
    MailboxList& from();
    void from(const MailboxList&);

    const AddressList& to() const;
    AddressList& to();
    void to(const AddressList&);

    const std::string& subject() const;
    std::string& subject();
    void subject(const std::string&);

    const AddressList& replyto() const;
    AddressList& replyto();
    void replyto(const AddressList&);

    const AddressList& cc() const;
    AddressList& cc();
    void cc(const AddressList&);

    const AddressList& bcc() const;
    AddressList& bcc();
    void bcc(const AddressList&);

    const MessageId& messageid() const;
    MessageId& messageid();
    void messageid(const MessageId&);
protected:
    template<typename T>
    const T& getField(const std::string&) const;
    template<typename T>
    T& getField(const std::string&);
    template<typename T>
    void setField(const std::string&, const T&);
};


// template member functions
template<typename T>
const T& Rfc822Header::getField(const std::string& name) const
{
    const_iterator it;
    it = find_if(begin(), end(), find_by_name(name));
    if(it != end())
    {
        // cast away constness
        Field& f = const_cast<Field&>(*it);
        // to be sure that it's the correct type
        FieldValue* pFv = f.m_pValue;
        if(!pFv->typeChecked())
        {
            std::string val = pFv->str();
            delete pFv;
            pFv = new T(val);
            f.m_pValue = pFv;
        }
        return static_cast<const T&>(*pFv);
    } else {
        static const T null;
        return null;
    }
}
template<typename T>
T& Rfc822Header::getField(const std::string& name)
{    
    iterator it;
    it = find_if(begin(), end(), find_by_name(name));
    if(it != end())
    {
        FieldValue* pFv = it->m_pValue;
        if(pFv == 0)
        {
            pFv = new T;
            assert(pFv);
            it->m_pValue = pFv;
        }
        // be sure that it's the correct type
        else if(!pFv->typeChecked())
        {
            std::string val = pFv->str();
            delete pFv;
            pFv = new T(val);
            it->m_pValue = pFv;
        }
        return static_cast<T&>(*pFv);
    } else {
        // insert and get the reference of the actual 
        // obj in the container, then modify its fields
        Field f;
        it = insert(end(), f);
        it->name(name);
        T* pT = new T;
        assert(pT);
        it->m_pValue = pT;
        assert(it->m_pValue->typeChecked());
        return *pT;
    }
}

template<typename T>
void Rfc822Header::setField(const std::string& name, const T& obj) 
{
    // remove if already exists
    iterator bit = begin(), eit = end();
    iterator found = find_if(bit, eit, find_by_name(name));
    if(found != eit)
        erase(found);
    // add field
    Field f;
    iterator it;
    it = insert(end(), f);
    it->name(name);
    it->m_pValue = new T(obj);
}

}

#endif
