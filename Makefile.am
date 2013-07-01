#AUTOMAKE_OPTIONS = subdir-objects
#ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
libdir = $(SECUREDIR)

lib_LTLIBRARIES = pam_stormpath.la
pam_stormpath_la_SOURCES = src/b64enc.c src/cencode.c src/spconfig.c \
			   src/req.c src/pam_stormpath.c
pam_stormpath_la_LDFLAGS = -module -avoid-version

bin_PROGRAMS = sptest
sptest_SOURCES = src/sptest.c
sptest_LDADD = pam_stormpath.la
