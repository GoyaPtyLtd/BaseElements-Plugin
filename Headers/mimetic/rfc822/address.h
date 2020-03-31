/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: address.h,v 1.14 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_RFC822_ADDRESS_H_
#define _MIMETIC_RFC822_ADDRESS_H_
#include <string>
#include <mimetic/rfc822/mailbox.h>
#include <mimetic/rfc822/group.h>
#include <mimetic/rfc822/fieldvalue.h>

namespace mimetic 
{
///    Address class as defined by RFC822
/*!

    Address class is a C++ representation of RFC822 \e address structure.
    Use this class to parse fields that contains email addresses or email group.

    \code
    Rfc822::Address adr(msg.from());
    if(adr.isGroup())
        cout << *adr.group();
    else
        cout << *adr.mailbox();
    \endcode

    \sa <a href="../RFC/rfc822.txt">RFC822</a>
 */
struct Address: public FieldValue
{
    Address();
    Address(const char*);
    Address(const std::string&);
    bool isGroup() const;
    Mailbox& mailbox();
    const Mailbox& mailbox() const;
    Group& group();
    const Group& group() const;
    void set(const std::string&);
    std::string str() const;
    bool operator==(const Address&) const;
    bool operator!=(const Address&) const;
private:
    FieldValue* clone() const;
    Mailbox m_mbx;
    Group m_group;
    bool m_isGroup;
};


}

#endif

