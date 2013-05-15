dnl
dnl $Id$
dnl

PHP_ARG_WITH(fribidi, for FriBidi support,
[  --with-fribidi[=DIR]    Include FriBidi support (requires FriBidi >= 0.19.1).])

if test "$PHP_FRIBIDI" != "no"; then
  AC_PATH_PROG(FRIBIDI_PKG_CONFIG,pkg-config,,[/usr/local/bin:$PATH])

  if test ! -x "$FRIBIDI_PKG_CONFIG"; then
    AC_MSG_ERROR([pkg-config not found.])
  fi

  if test "$PHP_FRIBIDI" = "yes"; then
    FRIBIDI_PKG_CONFIG_CMD="$FRIBIDI_PKG_CONFIG fribidi"
  else
    FRIBIDI_PKG_CONFIG_CMD="$FRIBIDI_PKG_CONFIG $PHP_FRIBIDI/lib/pkgconfig/fribidi.pc"
  fi

  dnl check for fribidi version
  AC_MSG_CHECKING([for FriBidi version])

  fribidi_version_full=`$FRIBIDI_PKG_CONFIG_CMD --modversion`
  fribidi_version=`echo ${fribidi_version_full} | awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`

  if test "$fribidi_version" -ge 19001; then
    AC_MSG_RESULT([$fribidi_version_full])
  else
    AC_MSG_ERROR([FriBidi version 0.19.1 or later required.])
  fi

  dnl Get the paths
  FRIBIDI_LIBS=`$FRIBIDI_PKG_CONFIG_CMD --libs`
  FRIBIDI_INCS=`$FRIBIDI_PKG_CONFIG_CMD --cflags`

  if  test -n "$FRIBIDI_INCS" && test -n "$FRIBIDI_LIBS"; then
    PHP_EVAL_INCLINE($FRIBIDI_INCS)
    PHP_EVAL_LIBLINE($FRIBIDI_LIBS, FRIBIDI_SHARED_LIBADD)

    PHP_NEW_EXTENSION(fribidi, fribidi.c, $ext_shared)
    PHP_SUBST(FRIBIDI_SHARED_LIBADD)
    AC_DEFINE(HAVE_FRIBIDI, 1, [ ])
  else
    AC_MSG_ERROR([Could not find the required paths. Please check your FriBidi installation.])
  fi
fi
