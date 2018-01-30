/*
 BEMimeticMIMEEntity.h
 BaseElements Plug-In
 
 Copyright 2018 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */

#ifndef BEMimeticMIMEEntity_h
#define BEMimeticMIMEEntity_h


/*
 A simple class to allow overriding write so that (long) header lines are not folded.
 */


#include <mimetic/mimetic.h>


class BEMimeticMIMEEntity : public mimetic::MultipartMixed {
    
public:
    
    std::ostream& write ( std::ostream& os, const char* eol ) const;

protected:
    
};




#endif /* BEMimeticMIMEEntity_h */
