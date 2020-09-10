/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: group.h,v 1.12 2008-10-07 11:06:27 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_RFC822_GROUP_H_
#define _MIMETIC_RFC822_GROUP_H_
#include <string>
#include <vector>
#include <mimetic/rfc822/mailbox.h>

namespace mimetic
{


/// Represent the \e group type in the RFC822
/**
    Groups class is a container class that stores Rfc822::Mailbox objects.
    Use this class when you need to create or parse rfc822 \e email \e groups

    Parsing:
    \code
    Rfc822::Group grp("drivers: first@do.com, second@dom.com, last@dom.com;");
    Rfc822::Group::const_iterator bit(grp.begin()), eit(grp.end());
    cout << "Group " << grp.name() << endl;
    for(; bit != eit; ++bit)
        cout << "    " << *bit << endl;
    \endcode

    Building:
    \code
    Rfc822::Group grp;
    grp.push_back("first@dom.com");
    grp.push_back(Rfc822::Mailbox("second@dom.com"));
    grp.push_back(string("last@dom.com"));
    \endcode

    \sa <a href="../RFC/rfc822.txt">RFC822</a>
 */
struct Group: public FieldValue, public std::vector<Mailbox>
{
    Group();
    Group(const char*);
    Group(const std::string&);
    void name(const std::string&);
    std::string name(int bCanonical = 0) const;
    void set(const std::string&);
    std::string str() const;
protected:
    FieldValue* clone() const;
private:
    std::string m_text, m_name;
};


}
#endif
