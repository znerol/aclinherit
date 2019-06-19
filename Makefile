PREFIX = /usr/local
CFLAGS = -Wall -Wextra -Werror -pedantic -O2 -fPIC -mmacosx-version-min=10.8

aclinherit: aclinherit.c
	$(CC) $(CFLAGS) -o aclinherit aclinherit.c

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
