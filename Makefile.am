#
# gap4ti2: 
#
# This file is part of the build system of a GAP kernel extension.
# Requires automake.
#
ACLOCAL_AMFLAGS = -I m4

BINARCHDIR = bin/$(GAPARCH)
GAPINSTLIB = $(BINARCHDIR)/4ti2gap.so
GAPGMPINSTLIB = $(BINARCHDIR)/4ti2gapgmp.so


lib_LTLIBRARIES = 4ti2gap.la

if WITH_GMPXX
lib_LTLIBRARIES += 4ti2gapgmp.la
endif

4ti2gap_la_SOURCES = src/4ti2gap.cc
4ti2gap_la_CPPFLAGS = $(GAP_CPPFLAGS) $(GMP_CPPFLAGS) -DCONFIG_H $(FTIT_CPPFLAGS) @INTBITS@ 
4ti2gap_la_LDFLAGS = -R$(GMP_HOME)/lib -R$(FTIT_LDPATH)
4ti2gap_la_LDFLAGS += -module -avoid-version
4ti2gap_la_LIBADD = $(FTIT_LDFLAGS) -lzsolve @FTITLIB@
if WITH_GMPXX
4ti2gapgmp_la_SOURCES = src/4ti2gapgmp.cc
4ti2gapgmp_la_CPPFLAGS = $(GAP_CPPFLAGS) $(GMP_FLAGS) -DCONFIG_H $(FTIT_CPPFLAGS) -D_4ti2_GMP_
4ti2gapgmp_la_LDFLAGS = $(4ti2gap_la_LDFLAGS)
4ti2gapgmp_la_LIBADD = $(FTIT_LDFLAGS) -lzsolve -l4ti2gmp

all-local: $(GAPINSTLIB) $(GAPGMPINSTLIB)
	@echo "SUCCESS!"

else

all-local: $(GAPINSTLIB)
	@echo "SUCCESS!"

endif

$(GAPINSTLIB): 4ti2gap.la
	$(mkdir_p) $(BINARCHDIR)
	cp .libs/4ti2gap.so $(GAPINSTLIB)

if WITH_GMPXX
$(GAPGMPINSTLIB): 4ti2gapgmp.la
	cp .libs/4ti2gapgmp.so $(GAPGMPINSTLIB)
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
	doc/*.xml
#	src/4ti2gap.cc
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
