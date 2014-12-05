#
# gap4ti2: 
#
# This file is part of the build system of a GAP kernel extension.
# Requires automake.
#
ACLOCAL_AMFLAGS = -I m4

BINARCHDIR = bin/$(GAPARCH)
GAPINSTALLLIB = $(BINARCHDIR)/gap4ti2.so

lib_LTLIBRARIES = gap4ti2.la

gap4ti2_la_SOURCES = src/gap4ti2.cc
gap4ti2_la_CPPFLAGS = $(GAP_CPPFLAGS) -I$(GMP_HOME)/include -DCONFIG_H $(FTIT_CPPFLAGS) -D_4ti2_INT64_
gap4ti2_la_LDFLAGS = -R$(GMP_HOME)/lib -R$(FTIT_LDPATH)
gap4ti2_la_LDFLAGS += -module -avoid-version
if SYS_IS_CYGWIN
gap4ti2_la_LDFLAGS += -no-undefined -version-info 0:0:0 -Wl,$(GAPROOT)/bin/$(GAPARCH)/gap.dll
endif
gap4ti2_la_LIBADD = $(FTIT_LDFLAGS) -l4ti2int64

all-local: $(GAPINSTALLLIB)
	@echo "SUCCESS!"

$(GAPINSTALLLIB): gap4ti2.la
	$(mkdir_p) $(BINARCHDIR)
if SYS_IS_CYGWIN
	cp .libs/gap4ti2.dll $(GAPINSTALLLIB)
else
	cp .libs/gap4ti2.so $(GAPINSTALLLIB)
endif

clean-local: clean-doc
	rm -rf $(BINARCHDIR)

distclean-local: clean-doc
	rm -rf bin

doc:	doc/manual.six

clean-doc:
	rm -f doc/manual.*
	rm -f doc/gap4ti2.*
	rm -f doc/*.{bib,css,html,js,txt}
	find doc/ -type f -name *.xml -and \( -not -name 'gap4ti2-bib.xml' \) -delete

DOC_FILES = \
	PackageInfo.g \
	doc/*.autodoc \
	doc/*.xml \
	src/gap4ti2.cc
#	lib/gap4ti2.gd
#    examples/*.g \
#	makedoc.g

doc/manual.six: $(DOC_FILES)
	($(GAPROOT)/bin/gap.sh -A makedoc.g)

AM_COLOR_TESTS = always
TESTS_ENVIRONMENT = GAP_EXECUTABLE='$(GAPROOT)/bin/gap.sh'
TESTS = tst/run-tap.test
TEST_LOG_DRIVER = $(SHELL) $(top_srcdir)/tst/tap-driver.sh


.PHONY: doc
