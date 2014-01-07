#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([vstgui], [3.6])
AC_CONFIG_AUX_DIR([autotools-aux])
AC_CONFIG_SRCDIR([vstgui/winfileselector.cpp])
#AC_CONFIG_HEADERS([vstgui/config.h])

AM_INIT_AUTOMAKE([foreign -Wnone])

AC_CANONICAL_TARGET
AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CXX
# AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_CXXCPP
AC_PROG_RANLIB
AC_PROG_LN_S
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h malloc.h stddef.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])
AC_C_PROTOTYPES

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_STRTOD
AC_CHECK_FUNCS([floor memset munmap pow strcasecmp strrchr strstr])

# Args and such
AC_PRESERVE_HELP_ORDER
AM_SILENT_RULES
AC_ARG_ENABLE([deprecated-methods],
  [AS_HELP_STRING([--enable-deprecated-methods],
                  [support old deprecated methods @<:@default=no@:>@])],
  [case "${enableval}" in
     yes) AC_DEFINE([VSTGUI_ENABLE_DEPRECATED_METHODS], [1]) ;; #_AC_PROG_CXX_G
     no)  AC_DEFINE([VSTGUI_ENABLE_DEPRECATED_METHODS], [0]) ;;
     *)   AC_MSG_ERROR([bad value ${enableval} for --enable-deprecated-methods]) ;;
     esac],
  [AC_DEFINE([VSTGUI_ENABLE_DEPRECATED_METHODS])]
)
AC_ARG_ENABLE([debug], 
  [AS_HELP_STRING([--enable-debug],
                  [turn on debugging @<:@default=no@:>@])],
  [case "${enableval}" in
     yes) AC_DEFINE([_DEBUG])
          AC_DEFINE([DEVELOPMENT], [1]) ;;
     no)  AC_DEFINE([RELEASE], [1])
          AC_DEFINE([NDEBUG]) ;;
     *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
     esac],
  [AC_DEFINE([RELEASE], [1])
   AC_DEFINE([NDEBUG])]
)
AC_ARG_ENABLE([mmBOOL-patch],
  [AS_HELP_STRING([--enable-mmBOOL-patch],
                  [apply patch for OBJC++ "forgetting" BOOL; explicitly setting this to no will unpatch; NOTE THAT THIS PERSISTS THROUGH RECONFIGURATIONS @<:@default=nochange@:>@])],
  [case "${enableval}" in
     yes) patch --unified --input=../mmBOOL.patch --backup --quiet --batch --forward --directory=./vstgui ;;
     no)  patch --unified --input=../mmBOOL.patch --backup --quiet --batch --forward --reverse --directory=./vstgui ;;
     nochange) ;;
     *)   AC_MSG_ERROR([bad value ${enableval} for --enable-mmBOOL-patch]) ;;
     esac],
  []
)
AC_ARG_WITH([cocoa],
  [AS_HELP_STRING([--with-cocoa],
                  [build with support for cocoa (requires objc++)])],
  [AC_SUBST([COCOA], [0])],
  [AC_PROG_OBJCXXCPP
  AC_PROG_OBJCXX
  AC_SUBST([COCOA], [1])]
)
AM_CONDITIONAL(COCOASUPPORT, [test x"$COCOA" == x1])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
