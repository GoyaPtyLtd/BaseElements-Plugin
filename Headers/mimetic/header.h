/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: header.h,v 1.12 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_HEADER_H_
#define _MIMETIC_HEADER_H_
#include <string>
#include <algorithm>
#include <mimetic/rfc822/header.h>
#include <mimetic/mimeversion.h>
#include <mimetic/contenttype.h>
#include <mimetic/contentid.h>
#include <mimetic/contenttransferencoding.h>
#include <mimetic/contentdisposition.h>
#include <mimetic/contentdescription.h>

namespace mimetic
{

/// MIME message header class
struct Header: public Rfc822Header
{
    const MimeVersion& mimeVersion() const;
    MimeVersion& mimeVersion();
    void mimeVersion(const MimeVersion&);

    const ContentType& contentType() const;
    ContentType& contentType();
    void contentType(const ContentType&);

    const ContentTransferEncoding& contentTransferEncoding() const;
    ContentTransferEncoding& contentTransferEncoding();
    void contentTransferEncoding(const ContentTransferEncoding&);

    const ContentDisposition& contentDisposition() const;
    ContentDisposition& contentDisposition();
    void contentDisposition(const ContentDisposition&);

    const ContentDescription& contentDescription() const;
    ContentDescription& contentDescription();
    void contentDescription(const ContentDescription&);

    const ContentId& contentId() const;
    ContentId& contentId();
    void contentId(const ContentId&);
};

}

#endif
