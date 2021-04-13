/*
 BEMimeticMIMEEntity.cpp
 BaseElements Plug-In
 
 Copyright 2018-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#include "BEMimeticMIMEEntity.h"

/*
 BEMimeticMIMEEntity::write is, aside from the static_casts, the Field::write
 line (and a couple of lines at the beginning which have been deleted)
 directly from the mimetic source
*/

std::ostream& BEMimeticMIMEEntity::write ( std::ostream& os, const char* /* eol */ ) const
{
    // header field
    mimetic::Header::const_iterator hbit = header().begin(), heit = header().end();
    for(; hbit != heit; ++hbit)
        hbit->write ( os ) << mimetic::crlf; // don't fold long header lines
    const mimetic::ContentType& ct = m_header.contentType();
    // body
    if(ct.isMultipart())
    {
        std::string boundary = "--" + ct.param("boundary");
        if(body().preamble().length())
            os << mimetic::crlf << body().preamble();
        // opening boundary
        if(body().parts().size() == 0)
            os << mimetic::crlf << boundary << mimetic::crlf;
        mimetic::MimeEntityList::const_iterator bit, eit;
		static_cast<void>(bit = body().parts().begin()), eit = body().parts().end();
        for(; bit != eit; ++bit)
        {
            os << mimetic::crlf << boundary << mimetic::crlf;
            MimeEntity* pMe = *bit;
            os << *pMe;
        }
        // closing boundary
        os << mimetic::crlf << boundary + "--" << mimetic::crlf;
        if(body().epilogue().length())
            os << body().epilogue();
    } else if(ct.type() == "message" && ct.subtype() == "rfc822") {
        mimetic::MimeEntityList::const_iterator bit, eit;
		static_cast<void>(bit = body().parts().begin()), eit = body().parts().end();
        for(; bit != eit; ++bit)
        {
            os << mimetic::crlf;
            MimeEntity* pMe = *bit;
            os << *pMe;
        }
    } else {
        os << mimetic::crlf << body();
    }
    os.flush();
    return os;
}

