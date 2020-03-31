/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: file.h,v 1.8 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_OS_FILE_H
#define _MIMETIC_OS_FILE_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <string>
#include <iterator>
#include <mimetic/libconfig.h>
#include <mimetic/os/stdfile.h>
#ifdef HAVE_MMAP
#include <mimetic/os/mmfile.h>
#endif

namespace mimetic
{

#if HAVE_MMAP == 1
typedef MMFile File;
#else
typedef StdFile File;
#endif

}



#endif

