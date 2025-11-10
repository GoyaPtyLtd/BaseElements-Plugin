/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: mailboxlist.h,v 1.12 2008-10-07 11:06:27 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_RFC822_MAILBOXLIST_H_
#define _MIMETIC_RFC822_MAILBOXLIST_H_
#include <string>
#include <vector>
#include <mimetic/utils.h>
#include <mimetic/rfc822/mailbox.h>


namespace mimetic
{
/// List of Mailbox objects
/*!
    MailboxList class is a container class that holds Mailbox objects 

    \code
    const char* str = "dest@domain.com, friends: one@friends.net, "
                "two@friends.net;, last@users.com";
    MailboxList aList(str);
    MailboxList::const_iterator bit(aList.begin()), eit(aList.end());
    for(; bit != eit; ++bit)
    {
        cout << *bit;
    }
    \endcode

    \sa <a href="../RFC/rfc822.txt">RFC822</a>
 */
struct MailboxList: public FieldValue, public std::vector<Mailbox>
{
    MailboxList();
    MailboxList(const char*);
    MailboxList(const std::string&);
    MailboxList(const std::string&, const std::string&);

    std::string str() const;
protected:
    FieldValue* clone() const;
private:
    void set(const std::string&);
    istring m_name;
};



}

#endif
