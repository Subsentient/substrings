CC = gcc
AR = ar
RANLIB = ranlib
CFLAGS = -std=gnu89 -pedantic -Wall -O3 -fPIC
PREFIX = /usr
all:
	@echo ""
	@echo "Building libsubstrings.so.0.1..."
	$(CC) substrings.c -o libsubstrings.so.0.1 $(CFLAGS) -shared -Wl,-soname,libsubstrings.so.0 -nostdlib
	ln -sf libsubstrings.so.0.1 libsubstrings.so.0
	ln -sf libsubstrings.so.0.1 libsubstrings.so
	@echo ""
	@echo "Building libsubstrings.a..."
	$(CC) substrings.c -c $(CFLAGS)
	$(AR) rc libsubstrings.a substrings.o
	$(RANLIB) libsubstrings.a
install:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/include/
	mkdir -p $(PREFIX)/lib/
	install -v -m 0644 substrings.h $(PREFIX)/include/
	install -v -m 0644 libsubstrings.a $(PREFIX)/lib/
	install -v -m 0644 libsubstrings.so.0.1 $(PREFIX)/lib/
	ln -sf libsubstrings.so.0.1 $(PREFIX)/lib/libsubstrings.so.0
	ln -sf libsubstrings.so.0.1 $(PREFIX)/lib/libsubstrings.so
uninstall:
	rm -vf $(PREFIX)/include/substrings.h
	rm -vf $(PREFIX)/lib/libsubstrings.*

clean:
	rm -vf libsubstrings.*
	rm -vf substrings.o
