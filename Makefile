PREFIX = /usr

install:
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f tree.sh ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/tree.sh

uninstall:
	@rm -f ${DESTDIR}${PREFIX}/bin/tree.sh

.PHONY: install uninstall
