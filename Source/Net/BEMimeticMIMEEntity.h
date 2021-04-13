/*
 BEMimeticMIMEEntity.h
 BaseElements Plug-In

 Copyright 2018-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */

#ifndef BEMimeticMIMEEntity_h
#define BEMimeticMIMEEntity_h


/*
 A simple class to allow overriding write so that (long) header lines are not folded.
 */

#ifdef FMX_WIN_TARGET

#pragma warning(push, 0)
#include <mimetic/mimetic.h>
#pragma warning(pop)

#else

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-register"
#pragma GCC diagnostic ignored "-Wunused-private-field"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wconversion"
#include <mimetic/mimetic.h>
#pragma GCC diagnostic pop

#endif


class BEMimeticMIMEEntity : public mimetic::MultipartMixed {

public:

    std::ostream& write ( std::ostream& os, const char* eol ) const;

protected:

};




#endif /* BEMimeticMIMEEntity_h */
