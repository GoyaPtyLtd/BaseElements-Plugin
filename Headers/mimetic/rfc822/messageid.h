/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: messageid.h,v 1.15 2008-10-07 11:06:27 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_MESSAGEID_H_
#define _MIMETIC_MESSAGEID_H_
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include <string>
#include <mimetic/libconfig.h>
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <mimetic/utils.h>
#include <mimetic/os/utils.h>
#include <mimetic/rfc822/fieldvalue.h>

namespace mimetic
{


/// Message-ID field value 
/// On Win32 Winsock library must be initialized before using this class.
struct MessageId: public FieldValue
{
    MessageId(uint32_t thread_id = 0 );
    MessageId(const std::string&);
    std::string str() const;
    void set(const std::string&);
protected:
    FieldValue* clone() const;
private:
    static unsigned int ms_sequence_number;
    std::string m_msgid;
};


}

#endif
