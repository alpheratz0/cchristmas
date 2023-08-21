.POSIX:
.PHONY: all clean install uninstall dist

include config.mk

all: cchristmas

cchristmas: cchristmas.o
	$(CC) $(LDFLAGS) -o cchristmas cchristmas.o

clean:
	rm -f cchristmas cchristmas.o cchristmas-$(VERSION).tar.gz

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f cchristmas $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/cchristmas
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	cp -f cchristmas.1 $(DESTDIR)$(MANPREFIX)/man1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/cchristmas.1

dist: clean
	mkdir -p cchristmas-$(VERSION)
	cp -R COPYING config.mk Makefile README cchristmas.1 \
		cchristmas.c cchristmas-$(VERSION)
	tar -cf cchristmas-$(VERSION).tar cchristmas-$(VERSION)
	gzip cchristmas-$(VERSION).tar
	rm -rf cchristmas-$(VERSION)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/cchristmas
	rm -f $(DESTDIR)$(MANPREFIX)/man1/cchristmas.1
