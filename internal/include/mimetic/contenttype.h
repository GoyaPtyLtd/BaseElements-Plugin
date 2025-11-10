/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: contenttype.h,v 1.13 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CONTENT_TYPE_H_
#define _MIMETIC_CONTENT_TYPE_H_
#include <string>
#include <mimetic/strutils.h>
#include <mimetic/rfc822/fieldvalue.h>
#include <mimetic/fieldparam.h>

namespace mimetic
{

/// Content-Type field value
class ContentType: public FieldValue
{
public:
    static const char label[];
    struct Boundary
    {
        Boundary();
        operator const std::string&() const;
    private:
        std::string m_boundary;
        static std::string ms_common_boundary;
        static int ms_i;
    };
    typedef FieldParam Param;
    typedef FieldParamList ParamList;
public:
    ContentType();
    ContentType(const char*);
    ContentType(const std::string&);
    ContentType(const std::string&, const std::string&);

    void set(const std::string&);
    void set(const std::string&, const std::string&);

    bool isMultipart() const;

    const istring& type() const;
    void type(const std::string&);

    void subtype(const std::string&);
    const istring& subtype() const;

    const ParamList& paramList() const;
    ParamList& paramList();

    const std::string& param(const std::string&) const;
    void param(const std::string&, const std::string&);

    std::string str() const;
protected:
    FieldValue* clone() const;
private:
    istring m_type, m_subtype;
    ParamList m_paramList;
};

}

#endif
