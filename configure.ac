# configure.ac -- autoconf script for the car plugin
# Copyright (C) 2013  Galois, Inc.
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
#
# To contact Galois, complete the Web form at <http://corp.galois.com/contact/>
# or write to Galois, Inc., 421 Southwest 6th Avenue, Suite 300, Portland,
# Oregon, 97204-1622.

# Fire up Autoconf.
AC_PREREQ([2.69])
AC_INIT([libv_repExtAutomobile], [1.0.1], [bbarenblat@galois.com])
AC_CANONICAL_TARGET

# This package will build but not function on OS X, so fail fast if we're on
# Darwin.
case "$target_os" in
    darwin*)
        AC_MSG_ERROR([this plugin will not function on OS X])
        ;;
esac

# Fire up Automake.
AM_INIT_AUTOMAKE([-Wall -Wno-gnu -Wno-override -Werror foreign no-dist-gzip dist-bzip2])
AC_CONFIG_MACRO_DIR([m4])

# Build quietly.
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Build a C++ library.
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])
# Automake 1.12 seems to require this, but automake 1.11 doesn't recognize it.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT
AC_PROG_LN_S

# Dependencies
AC_ARG_VAR([VREP], [path to V-REP installation])
if test -z "$VREP"; then
    AC_MSG_ERROR([empty VREP environment variable; please set it to the path to the V-REP installation directory])
fi
VREP_CXXFLAGS=-I$VREP/programming/include
AC_SUBST([VREP])
AC_SUBST([VREP_CXXFLAGS])

BOOST_REQUIRE
BOOST_FILESYSTEM
BOOST_OPTIONAL([1.34])

# C++11 initializer list syntax is nice!  Sadly, it is not universally
# supported.  Notably, Clang <3.1 lacks support for initializer lists, and a
# number of systems--e.g., Debian Wheezy machines--have pre-3.1 Clang.
# TODO: Once Clang 3.0 and earlier disappear from the ecosystem, get rid of
# HAVE_CXX11_INITIALIZER_LISTS and make this check fatal on error.
AC_LANG_PUSH([C++])
AC_MSG_CHECKING([for initializer list support])
# Double brackets go around the argument to AC_LANG_SOURCE, lest m4 replace the
# square brackets in the C++ code.
AC_COMPILE_IFELSE(
    [AC_LANG_SOURCE([[#include <vector>
int main(int argc, char *argv[]) {
    std::vector<int> test {1, 2, 3};
    return 0;
}]])],
    [AC_DEFINE(
        [HAVE_CXX11_INITIALIZER_LISTS],
        [1],
        [Define to 1 if your compiler supports C++11 initializer lists.])
     AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])])
AC_LANG_POP([C++])

# Same for lambda expressions.
AC_LANG_PUSH([C++])
AC_MSG_CHECKING([for C++11 closure support])
AC_COMPILE_IFELSE(
    [AC_LANG_SOURCE([[int main(int argc, char *argv[]) {
    auto test = [](int &n){ n++; };
    return 0;
}]])],
    [AC_DEFINE(
        [HAVE_CXX11_CLOSURES],
        [1],
        [Define to 1 if your compiler supports C++11 closure syntax.])
     AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])])
AC_LANG_POP([C++])

# Generate Makefiles.
AC_CONFIG_FILES([
    Makefile
    src/Makefile])

# All done.
AC_OUTPUT
