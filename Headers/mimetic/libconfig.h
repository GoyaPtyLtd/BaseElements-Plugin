/***************************************************************************
    copyright            : (C) by 2002-2004 Stefano Barbato
    email                : stefano@codesink.org

    $Id: libconfig.h,v 1.10 2009-02-16 18:08:59 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_LIB_CONFIG_H_
#define _MIMETIC_LIB_CONFIG_H_
#if defined(__unix__) || defined(__linux__) || defined(__unix) || defined(_AIX)
#ifdef HAVE_MIMETIC_CONFIG
#include "config.h"
#endif
#define CONFIG_UNIX
#endif

/* Mac OS X */
#if defined(__APPLE__) && defined(__MACH__)
typedef unsigned int uint;
#ifdef HAVE_MIMETIC_CONFIG
#include "config.h"
#endif
#define CONFIG_UNIX
#endif

/* Windows */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <mimetic/config_win32.h>
#include <process.h>
#include <io.h>
#include <ctime>
#include <cstdio>
typedef unsigned int uint;
#define CONFIG_WIN32
#endif

#if !defined(CONFIG_WIN32) && !defined(CONFIG_UNIX)
#error "I'm unable to guess platform type. please define CONFIG_WIN32 or CONFIG_UNIX"
#endif
#if defined(CONFIG_WIN32) && defined(CONFIG_UNIX)
#error "I'm unable to guess platform type. please define CONFIG_UNIX or CONFIG_WIN32"
#endif

#ifdef CONFIG_UNIX
#include <cstdlib>
#define PATH_SEPARATOR '/'
typedef unsigned int uint32;
struct newline_traits
{
    enum { lf = 0xA, cr = 0xD };
    enum { size = 1 };
    enum { ch0 = lf, ch1 = 0 };
};
#endif

#ifdef CONFIG_WIN32
#define PATH_SEPARATOR '\\'
typedef unsigned int uint32;
struct newline_traits
{
    enum { lf = 0xA, cr = 0xD };
    enum { size = 2 };
    enum { ch0 = cr, ch1 = lf };
};
#endif

#endif
