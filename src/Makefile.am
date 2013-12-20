# Makefile.am -- automake script for the car plugin
# Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003,
#     2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011  Free Software Foundation,
#     Inc.
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

# We're building a library that gets loaded by dlopen, not a standard .so.
AM_LDFLAGS = \
	-avoid-version -module -shared -export-dynamic

lib_LTLIBRARIES = libv_repExtAutomobile.la
libv_repExtAutomobile_la_SOURCES = \
	$(srcdir)/automobile.cpp \
	$(srcdir)/automobile.h \
	$(srcdir)/csv.cpp \
	$(srcdir)/csv.h \
	$(srcdir)/csv-inl.h \
	$(srcdir)/main.cpp \
	$(srcdir)/main.h \
	$(srcdir)/measurement.cpp \
	$(srcdir)/measurement.h \
	$(srcdir)/measurement-inl.h \
	$(srcdir)/noise.cpp \
	$(srcdir)/noise.h \
	$(srcdir)/noise-inl.h \
	$(srcdir)/vrep.cpp \
	$(srcdir)/vrep.h \
	$(srcdir)/vrep-inl.h \
	$(srcdir)/vrepFfi.cpp \
	$(srcdir)/vrepFfi.h \
	$(srcdir)/vrepFfi-inl.h
libv_repExtAutomobile_la_CPPFLAGS = \
	$(BOOST_CPPFLAGS)
libv_repExtAutomobile_la_CXXFLAGS = \
	-Wall \
	-Wextra \
	-pedantic \
	@VREP_CXXFLAGS@
libv_repExtAutomobile_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	-export-symbols-regex '^v_rep' \
	$(BOOST_FILESYSTEM_LDFLAGS)
libv_repExtAutomobile_la_LIBADD = \
	$(BOOST_FILESYSTEM_LIBS)

# In addition to these sources, we also need to bundle a special object built
# from code in the V-REP distribution.  Ideally, I'd just add that source file
# to 'libv_repExtAutomobile_la_SOURCES', but it's not exactly
# standards-compliant and generates a ton of warnings.  To get around this, use
# per-object flags emulation (see "Per-Object Flags Emulation" in the Automake
# manual).
libv_repExtAutomobile_la_LIBADD += libv_repLib.la
noinst_LTLIBRARIES = libv_repLib.la
libv_repLib_la_SOURCES = @VREP@/programming/common/v_repLib.cpp
libv_repLib_la_CXXFLAGS = @VREP_CXXFLAGS@

# Override install and uninstall targets to stick the libraries in the V-REP
# directory.
install-libLTLIBRARIES: $(lib_LTLIBRARIES)
	@$(NORMAL_INSTALL)
	@list='$(lib_LTLIBRARIES)'; test -n "$(libdir)" || list=; \
	list2=; for p in $$list; do \
	  if test -f $$p; then \
	    list2="$$list2 $$p"; \
	  else :; fi; \
	done; \
	test -z "$$list2" || { \
	  echo " $(MKDIR_P) '$(VREP)'"; \
	  $(MKDIR_P) "$(VREP)" || exit 1; \
	  echo " $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install --no-warn $(INSTALL) $(INSTALL_STRIP_FLAG) $$list2 '$(VREP)'"; \
	  $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install --no-warn $(INSTALL) $(INSTALL_STRIP_FLAG) $$list2 "$(VREP)"; \
	}
uninstall-libLTLIBRARIES:
	@$(NORMAL_UNINSTALL)
	@list='$(lib_LTLIBRARIES)'; test -n "$(libdir)" || list=; \
	for p in $$list; do \
	  $(am__strip_dir) \
	  echo " $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=uninstall rm -f '$(VREP)/$$f'"; \
	  $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=uninstall rm -f "$(VREP)/$$f"; \
	done
