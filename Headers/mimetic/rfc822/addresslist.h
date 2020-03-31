/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: addresslist.h,v 1.12 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_RFC822_ADDRESSLIST_H_
#define _MIMETIC_RFC822_ADDRESSLIST_H_
#include <string>
#include <vector>
#include <mimetic/rfc822/address.h>
#include <mimetic/rfc822/fieldvalue.h>
namespace mimetic 
{

/// List of Address
/**
    AddressList class is a container class that holds Address objects which,
      in turn can be a Group or a Mailbox.

    \code
    const char* str = "dest@domain.com, friends: one@friends.net, "
                "two@friends.net;, last@users.com";
    AddressList aList(str);
    AddressList::const_iterator bit(aList.begin()), eit(aList.end());
    for(; bit != eit; ++bit)
    {
        Address& adr = *bit;
        if(adr.isGroup())
            cout << *adr.group();
        else
            cout << *adr.mailbox();
    }
    \endcode

    \sa <a href="../RFC/rfc822.txt">RFC822</a>
 */
struct AddressList: public FieldValue, public std::vector<Address>
{
    AddressList();
    AddressList(const char*);
    AddressList(const std::string&);

    std::string str() const;
    void set(const std::string&);
protected:
    FieldValue* clone() const;
private:
};


}

#endif
