/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: mmfile.h,v 1.12 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_OS_MMFILE_H
#define _MIMETIC_OS_MMFILE_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string>
#include <cstring>
#include <mimetic/os/fileop.h>

namespace mimetic
{

/// Memory mapped file
struct MMFile: public FileOp
{
    typedef char* iterator;
    typedef const char* const_iterator;
    MMFile();
    MMFile(const std::string&, int mode = O_RDONLY);
    ~MMFile();
    operator bool() const;
    bool open(const std::string&, int mode = O_RDONLY);
    void close();
    uint read(char*, int);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

protected:
    bool map();
    bool open(int flags);
    bool stat();

    std::string m_fqn;
    bool m_stated;
    struct stat m_st;
    int m_fd;

    char *m_beg, *m_end;
};

}


#endif

