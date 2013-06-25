AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

#lib_LTLIBRARIES = libpam_stormpath.la
#libpam_stormpath_la_SOURCES = src/b64enc.c src/cencode.c src/config.c \
#			      src/req.c src/pam_stormpath.c

bin_PROGRAMS = pam_stormpath
pam_stormpath_SOURCES = src/b64enc.c src/cencode.c src/spconfig.c \
			src/req.c src/pam_stormpath.c
LDADD = -lpam -ljson -lcurl
