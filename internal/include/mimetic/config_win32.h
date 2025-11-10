/* mimetic/config_win32.h  */

#undef HAVE_DIRENT_H
#undef HAVE_GETPAGESIZE
#undef HAVE_MMAP
#undef HAVE_UNISTD_H
#undef HAVE_SYS_TIME_H

#undef STDC_HEADERS
#undef HAVE_SYS_STAT_H
#undef HAVE_SYS_TYPES_H

#define STDC_HEADERS 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1

#define PACKAGE "mimetic"
#define VERSION "0.9.8"


typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

