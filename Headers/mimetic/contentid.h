/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: contentid.h,v 1.11 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_CONTENTID_H_
#define _MIMETIC_CONTENTID_H_
#include <string>
#include <mimetic/utils.h>
#include <mimetic/os/utils.h>
#include <mimetic/rfc822/fieldvalue.h>

namespace mimetic
{

/// Content-ID field value
struct ContentId: public FieldValue
{
    // format: yyyymmgg.pid.seq@hostname
    static const char label[];
    ContentId();
    ContentId(const char*);
    ContentId(const std::string&);
    void set(const std::string&);
    std::string str() const;
protected:
    FieldValue* clone() const;
private:
    static unsigned int ms_sequence_number;
    std::string m_cid;
};

}

#endif
