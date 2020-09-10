/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: mailbox.h,v 1.14 2008-10-07 11:06:27 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_RFC822_MAILBOX_H_
#define _MIMETIC_RFC822_MAILBOX_H_
#include <string>
#include <mimetic/rfc822/fieldvalue.h>
namespace mimetic
{



/// Represents a \e mailbox email address as defined in the RFC822
/**
    Use this class if you want to build or parse email addresses. Each email address
    as defined by RFC822 have a mailbox std::string, a domain name, a sourceroute and
    a label. Note that just mailbox and domain are mandatory.
    Mailboxes can be represented in different ways, can contain rfc822 comments and
    blank spaces, can be double-quoted and contain source route. Please read the
    RFC822 for details.

    Parsing:
    \code
    Mailbox mbx("Mario (Spider)Rossi <@free.it@move.it:mrossi@dom.it>");
    cout << mbx.mailbox() << endl;
    cout << mbx.domain() << endl;
    cout << mbx.label() << endl;
    cout << mbx.sourceroute() << endl;
    cout << mbx.text() << endl;
    \endcode

    Building:
    \code
    Mailbox mbx;
    mbx.mailbox("mrossi");
    mbx.domain("dom.it");
    mbx.label("Mario (Spider)Rossi");
    mbx.sourceroute("@free.it@move.it");
    \endcode

    \sa <a href="../RFC/rfc822.txt">RFC822</a>
 */
struct Mailbox: public FieldValue
{
    Mailbox();
    Mailbox(const char*);
    Mailbox(const std::string&);
    void mailbox(const std::string&);
    void domain(const std::string&);    
    void label(const std::string&);
    void sourceroute(const std::string&);
    std::string mailbox(int bCanonical = 1) const;
    std::string domain(int bCanonical = 1) const;    
    std::string label(int bCanonical = 0) const;
    std::string sourceroute(int bCanonical = 1) const;
    bool operator==(const Mailbox&) const;
    bool operator!=(const Mailbox&) const;
    void set(const std::string&);
    std::string str() const;
protected:
    FieldValue* clone() const;
private:
    std::string m_mailbox, m_domain, m_label, m_route;
};


}

#endif
