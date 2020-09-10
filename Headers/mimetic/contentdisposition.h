/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: contentdisposition.h,v 1.12 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CONTENT_DISPOSITION_H_
#define _MIMETIC_CONTENT_DISPOSITION_H_
#include <string>
#include <iostream>
#include <mimetic/fieldparam.h>
#include <mimetic/rfc822/fieldvalue.h>

namespace mimetic
{



/// Content-Disposition field value
struct ContentDisposition: public FieldValue
{
    typedef FieldParam Param;
    typedef FieldParamList ParamList;
public:
    static const char label[];
    ContentDisposition();
    ContentDisposition(const char*);
    ContentDisposition(const std::string&);

    void type(const std::string&);
    const istring& type() const;

    const ParamList& paramList() const;
    ParamList& paramList();

    const std::string& param(const std::string&) const;
    void param(const std::string&, const std::string&);
    
    void set(const std::string&);
    std::string str() const;

    std::ostream& write(std::ostream& os, int fold = 0) const;
protected:
    FieldValue* clone() const;
private:
    istring m_type;
    ParamList m_paramList;
};

}

#endif

