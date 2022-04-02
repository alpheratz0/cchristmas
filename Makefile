VERSION = 0.1.0
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
INCS = -I/usr/include
CFLAGS = -pedantic -Wall -Wextra -Os ${INCS} -DVERSION=\"${VERSION}\"
CC = cc

SRC = src/cchristmas.c \
	  src/debug.c


OBJ = ${SRC:.c=.o}

all: cchristmas

${OBJ}:	src/debug.h

cchristmas: ${OBJ}
	${CC} -o $@ ${OBJ}

install: all
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f cchristmas ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/cchristmas
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@cp -f man/cchristmas.1 ${DESTDIR}${MANPREFIX}/man1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/cchristmas.1

dist: clean
	@mkdir -p cchristmas-${VERSION}
	@cp -R LICENSE Makefile README man src cchristmas-${VERSION}
	@tar -cf cchristmas-${VERSION}.tar cchristmas-${VERSION}
	@gzip cchristmas-${VERSION}.tar
	@rm -rf cchristmas-${VERSION}

uninstall:
	@rm -f ${DESTDIR}${PREFIX}/bin/cchristmas
	@rm -f ${DESTDIR}${MANPREFIX}/man1/cchristmas.1

clean:
	@rm -f cchristmas cchristmas-${VERSION}.tar.gz ${OBJ}

.PHONY: all clean install uninstall dist
