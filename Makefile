#CC=clang
CC=gcc
CFLAGS=-Wall -g

all: pam

pam: clean_pam pam_module

pam_install: pam install

clean: clean_pam

clean_pam:
	rm -rf src/*.so src/*.o

pam_module:
	${CC} -c -fPIC src/cencode.c -o src/cencode.o -g -Wall
	${CC} -c -fPIC src/b64enc.c -o src/b64enc.o -g -Wall
	${CC} -c -fPIC src/req.c -o src/req.o -g -Wall
	${CC} -c -fPIC src/config.c -o src/config.o -g -Wall
	${CC} -c -fPIC src/pam_stormpath.c -o src/pam_stormpath.o -g -Wall

	${CC} -fPIC -shared -o src/pam_stormpath.so src/pam_stormpath.o src/b64enc.o src/cencode.o src/req.o src/config.o -lpam -ljson -lcurl -Wall

	${CC} -fPIC -o pam_stormpath src/pam_stormpath.o src/b64enc.o src/cencode.o src/req.o src/config.o -lpam -ljson -lcurl -Wall

install:
	cp src/pam_stormpath.so /lib/x86_64-linux-gnu/security/
.PHONY: all pam clean clean_pam pam_module pam_install install
