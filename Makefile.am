#
# gap4ti2:
#
# This file is part of the build system of a GAP kernel extension.
# Requires automake.
#
ACLOCAL_AMFLAGS = -I m4

BINARCHDIR = bin/$(GAPARCH)
GAPINSTLIB = $(BINARCHDIR)/4ti2gap.so

lib_LTLIBRARIES = 4ti2gap.la

4ti2gap_la_SOURCES = src/4ti2files.cc src/4ti2gap.cc src/4ti2groebner.cc src/4ti2minimize.cc src/4ti2normalform.cc src/4ti2zsolve.cc src/basicxch.cc src/4ti2daco.cc
4ti2gap_la_CPPFLAGS = $(GAP_CPPFLAGS) -DCONFIG_H $(FTIT_CPPFLAGS) -std=c++11
4ti2gap_la_LDFLAGS = -R$(GMP_HOME)/lib -R$(FTIT_LDPATH)
4ti2gap_la_LDFLAGS += -module -avoid-version -z
4ti2gap_la_LIBADD = $(FTIT_LDFLAGS) -lzsolve
if WITH_GMPXX
4ti2gap_la_CPPFLAGS += $(GMP_FLAGS) -D_4ti2_WITH_GMP_ @INTBITS@
4ti2gap_la_LIBADD += -l4ti2gmp
else
4ti2gap_la_CPPFLAGS += @INTBITS@
4ti2gap_la_LIBADD += @FTITLIB@
endif

all-local: $(GAPINSTLIB)
	@echo "SUCCESS!"

$(GAPINSTLIB): 4ti2gap.la
	$(mkdir_p) $(BINARCHDIR)
	cp .libs/4ti2gap.so $(GAPINSTLIB)

clean-local:
	rm -rf $(BINARCHDIR)

distclean-local:
	rm -rf bin
	rm -f src/.deps/*
	rm -f *.la
	rm -f Makefile.in
	rm -rf autom4te.cache
	rm -rf cnf

doc:	doc/manual.six

#clean-doc:
#	rm -f doc/manual.*
#	rm -f doc/*.{idx,bbl,aux,blg,ilg,ind,log,out,pnr,tex,toc,bib,css,html,js,txt}
#	find doc/ -type f -name *.xml -and \( -not -name '4ti2gap.xml' \) -delete

DOC_FILES = \
	PackageInfo.g \
	doc/*.autodoc \
	doc/*.xml \
	src/*.cc \
	lib/*.gd \
	makedoc.g
#    examples/*.g

doc/manual.six: $(DOC_FILES)
	($(GAPROOT)/bin/gap.sh -A makedoc.g)

AM_COLOR_TESTS = always
TESTS_ENVIRONMENT = GAP_EXECUTABLE='$(GAPROOT)/bin/gap.sh'
TESTS = tst/run-tap.test
TEST_LOG_DRIVER = $(SHELL) $(top_srcdir)/tst/tap-driver.sh


.PHONY: doc
