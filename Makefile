PREFIX = /usr/local

aclinherit: aclinherit.c
	$(CC) -o aclinherit aclinherit.c

.PHONY: install
install: aclinherit
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 0755 -s aclinherit $(DESTDIR)$(PREFIX)/bin/aclinherit

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/aclinherit

.PHONY: clean
clean:
	rm -f aclinherit
