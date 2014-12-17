#
# gap4ti2: 
#
# This file is part of the build system of a GAP kernel extension.
# Requires automake.
#
ACLOCAL_AMFLAGS = -I m4

BINARCHDIR = bin/$(GAPARCH)
GAPINSTALLLIB = $(BINARCHDIR)/4ti2gap.so

lib_LTLIBRARIES = 4ti2gap.la

4ti2gap_la_SOURCES = src/4ti2gap.cc
4ti2gap_la_CPPFLAGS = $(GAP_CPPFLAGS) -I$(GMP_HOME)/include -DCONFIG_H $(FTIT_CPPFLAGS) -D_4ti2_INT64_
4ti2gap_la_LDFLAGS = -R$(GMP_HOME)/lib -R$(FTIT_LDPATH)
4ti2gap_la_LDFLAGS += -module -avoid-version
if SYS_IS_CYGWIN
4ti2gap_la_LDFLAGS += -no-undefined -version-info 0:0:0 -Wl,$(GAPROOT)/bin/$(GAPARCH)/gap.dll
endif
4ti2gap_la_LIBADD = $(FTIT_LDFLAGS) -l4ti2int64 -lzsolve

all-local: $(GAPINSTALLLIB)
	@echo "SUCCESS!"

$(GAPINSTALLLIB): 4ti2gap.la
	$(mkdir_p) $(BINARCHDIR)
if SYS_IS_CYGWIN
	cp .libs/4ti2gap.dll $(GAPINSTALLLIB)
else
	cp .libs/4ti2gap.so $(GAPINSTALLLIB)
endif

clean-local: clean-doc
	rm -rf $(BINARCHDIR)

distclean-local: clean-doc
	rm -rf bin

doc:	doc/manual.six

clean-doc:
	rm -f doc/manual.*
	rm -f doc/4ti2gap.*
	rm -f doc/*.{bib,css,html,js,txt}
	find doc/ -type f -name *.xml -and \( -not -name '4ti2gap-bib.xml' \) -delete

DOC_FILES = \
	PackageInfo.g \
	doc/*.autodoc \
	doc/*.xml \
	src/4ti2gap.cc
#	lib/4ti2gap.gd
#    examples/*.g \
#	makedoc.g

doc/manual.six: $(DOC_FILES)
	($(GAPROOT)/bin/gap.sh -A makedoc.g)

AM_COLOR_TESTS = always
TESTS_ENVIRONMENT = GAP_EXECUTABLE='$(GAPROOT)/bin/gap.sh'
TESTS = tst/run-tap.test
TEST_LOG_DRIVER = $(SHELL) $(top_srcdir)/tst/tap-driver.sh


.PHONY: doc
