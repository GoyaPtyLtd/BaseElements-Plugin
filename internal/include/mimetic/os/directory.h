#ifndef _MIMETIC_OS_DIRECTORY_H_
#define _MIMETIC_OS_DIRECTORY_H_
#include <string>
#include <iterator>
#include <mimetic/libconfig.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#include <unistd.h>
#include <sys/stat.h>

namespace mimetic
{

class Directory
{
public:
    struct DirEntry
    {
        enum Type { Unknown, RegularFile, Directory, Link };
        DirEntry(): type(Unknown) {}
        std::string name;
        Type type;
    };
    friend class iterator;
    struct iterator: public std::iterator<std::forward_iterator_tag, DirEntry>
    {
        iterator() // end() it
        : m_dirp(0), m_dirh(0), m_eoi(true)
        {
        }
        iterator(Directory* dirp) // begin() it
        : m_dirp(dirp), m_eoi(false)
        {
            m_dirh = opendir(m_dirp->m_path.c_str());
            if(m_dirh)
            {
                m_dirent = readdir(m_dirh);
                setDirent(m_dirent);
            } else {
                // opendir error, set equal to end()
                m_dirp = 0;
                m_dirh = 0;
                m_eoi = true;
            }
        }
        ~iterator()
        {
            if(m_dirh)
                closedir(m_dirh);
        }
        const DirEntry& operator*() const
        {
            return m_de;
        }
        const DirEntry* operator->() const
        {
            return &m_de;
        }
        iterator& operator++()
        {
            if((m_dirent = readdir(m_dirh)) == NULL)
            {
                m_eoi = true;
                return *this;
            }
            setDirent(m_dirent);
            return *this;
        }
        iterator operator++(int) // postfix
        {
            iterator it = *this;
            ++*this;
            return it;
        }
        bool operator==(const iterator& right)
        {
            if(m_eoi && right.m_eoi)
                return true;
            
            return 
            m_eoi == right.m_eoi &&
            m_dirp->m_path == right.m_dirp->m_path &&
            m_dirent && right.m_dirent &&
            #ifdef _DIRENT_HAVE_D_TYPE
            m_dirent->d_type == right.m_dirent->d_type &&
            #endif
            std::string(m_dirent->d_name) == right.m_dirent->d_name;
        }
        bool operator!=(const iterator& right)
        {
            return !operator==(right);
        }
    private:
        void setDirent(struct dirent* dent)
        {
            m_de.name = dent->d_name;
            m_de.type = DirEntry::Unknown;
            #ifdef _DIRENT_HAVE_D_TYPE
            switch(dent->d_type)
            {
            case DT_DIR:
                m_de.type = DirEntry::Directory;
                break;
            case DT_REG:
                m_de.type = DirEntry::RegularFile;
                break;
            case DT_LNK:
                m_de.type = DirEntry::Link;
                break;
            }
            #endif
        }
        Directory* m_dirp;
        DIR* m_dirh;
        bool m_eoi;
        DirEntry m_de;
        struct dirent* m_dirent;
    };

    Directory(const std::string& dir)
    : m_path(dir)
    {
    }
    ~Directory()
    {
    }
    iterator begin()
    {    return iterator(this);    }
    iterator end()
    {    return iterator();    };
    bool exists() const
    {
        struct stat st;
        return stat(m_path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
    }
    static bool exists(const std::string& dname)
    {
        struct stat st;
        return stat(dname.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
    }
    static bool create(const std::string& dname)
    {
        if(!exists(dname))
            return mkdir(dname.c_str(), 0755) == 0;
        else
            return 0;
    }
    static bool remove(const std::string& dname)
    {
        if(!exists(dname))
            return 0;
        else
            return rmdir(dname.c_str()) == 0;
    }
    const std::string& path() const
    {
        return m_path;
    }
private:
    std::string m_path;
};

}

#endif

