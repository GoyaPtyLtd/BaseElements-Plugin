/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: field.h,v 1.14 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_RFC822_FIELD_H_
#define _MIMETIC_RFC822_FIELD_H_
#include <string>
#include <mimetic/strutils.h>
#include <mimetic/rfc822/fieldvalue.h>

namespace mimetic
{



/// Field class as defined by RFC822
/**
    Field class is a C++ representation of RFC822 \e header \e field.
    Use this class when you need to create or parse messages' header fields.
    Note that field name is case insensitive.

    Parsing:
    \code
    Rfc822::Field f1("X-My-Field: some text(with a trailing comment)");
    cout << f.name() << endl;
    cout << f.value() << endl;
    cout << f.value(true) << endl; // canonicalize (see RFC822)
    \endcode

    Building:
    \code
    Rfc822::Field f;
    f.name("X-Unknown");
    f.value("some text(with a trailing comment)");
    cout << f;
    \endcode

    \sa <a href="../RFC/rfc822.txt">RFC822</a>
 */
struct Field
{
    typedef mimetic::istring istring;
    static const Field null;
    Field();
    Field(const std::string&);
    Field(const std::string&, const std::string&);
    ~Field();

    Field(const Field&);
    Field& operator=(const Field&);

    void name(const std::string&);
    const istring& name() const;

    void value(const std::string&);
    std::string value() const;

    std::ostream& write(std::ostream&, unsigned int fold = 0) const;
    friend std::ostream& operator<<(std::ostream&, const Field&);
private:
    friend class Rfc822Header;
    istring m_name;
    FieldValue* m_pValue;
};


}
#endif
