#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([myfs], [0.1], [lmatz823@gmail.com])
AM_INIT_AUTOMAKE(myfs, 0.1)

AC_CONFIG_SRCDIR([src/null.c])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([string.h unistd.h unistd.h time.h errno.h])

# Check for FUSE development environment
PKG_CHECK_MODULES(FUSE, fuse)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UID_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_STRUCT_ST_BLOCKS
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([ftruncate mkdir mkfifo realpath rmdir strerror utime])

# Not all systems that support FUSE also support fdatasync (notably freebsd)
AC_CHECK_FUNCS([fdatasync])


AC_OUTPUT(Makefile src/Makefile)
AC_OUTPUT
