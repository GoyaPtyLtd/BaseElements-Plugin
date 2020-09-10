/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: body.h,v 1.16 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_BODY_H_
#define _MIMETIC_BODY_H_
#include <string>
#include <math.h>
#include <mimetic/rfc822/body.h>
#include <mimetic/codec/code.h>
#include <mimetic/mimeentitylist.h>
#include <mimetic/os/file.h>


namespace mimetic
{

/// MIME message body
class Body: public Rfc822Body
{
public:
    friend class MimeEntity;
    Body();

    /**
      set body content
     */
    void set(const std::string&);

    /**
      load file as is, no encoding is performed
     */
    bool load(const std::string&);

    /**
      load file and code it using \p Codec 
     */
    template<typename Codec>
    bool load(const std::string&, const Codec&);
    
    /**
      en/decode body content
     */
    template<typename Codec>
    bool code(const Codec&);
    
    /**
      set body \e preamble 

      \sa RFC822
     */
    void preamble(const std::string&);
    /**
      get body \e preamble 

      \sa RFC822
     */
    const std::string& preamble() const;
    std::string& preamble();
    
    /**
      set body \e epilogue 

      \sa RFC822
     */
    void epilogue(const std::string&);
    /**
      get body \e epilogue 

      \sa RFC822
     */
    const std::string& epilogue() const;
    std::string& epilogue();
    
    /**
      get body's parts list 
     */
    MimeEntityList& parts();
    const MimeEntityList& parts() const;

    /**
      get body's MimeEntity owner
     */
    MimeEntity* owner();
    const MimeEntity* owner() const;
    
protected:
    void owner(MimeEntity*);
protected:
    MimeEntity* m_owner;
    MimeEntityList m_parts;
    std::string m_preamble, m_epilogue;
};

template<typename Codec>
bool Body::load(const std::string& fqn, const Codec& cc)
{
    File in(fqn);
    if(!in)
        return false;

    File::iterator beg = in.begin(), end = in.end();
    Codec codec(cc);

    if(codec.codeSizeMultiplier() > 1.0)
    {
        /* increase body string size */
        struct stat st;
        if(::stat(fqn.c_str(), &st))
            return false;
        reserve((size_type)(::ceil(st.st_size * codec.codeSizeMultiplier())));
    }

    this->clear();
    mimetic::code(beg, end, codec, back_inserter(*this) );
    return true;
}


template<typename Codec>
bool Body::code(const Codec& cc)
{
    // OPTIMIZE
    std::string coded;
    Codec codec(cc);

    if(codec.codeSizeMultiplier() > 1.0)
        coded.reserve((size_type)::ceil(size() * codec.codeSizeMultiplier()));

    mimetic::code(begin(), end(), codec, back_inserter(coded) );
    this->assign(coded);
    return true;
}

}

#endif
