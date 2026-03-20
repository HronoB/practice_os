CC := gcc
CFLAGS := -g -Wall -Werror -fPIC -Wl,-z,relro -Wl,-z,lazy -O0 -fsanitize=address,undefined

SOURCES := $(wildcard *.c *.S)

TEST_BINS := hello-world local big custom recoursive full-relro

.PHONY: all test tests clean

all: libvtrace.so test

test: tests
	python3 test.py

libvtrace.so:
	$(CC) $(CFLAGS) -shared -o libvtrace.so $(SOURCES)

tests: $(TEST_BINS)

SIMPLE_TEST_BINS := hello-world local big
$(SIMPLE_TEST_BINS):
	$(CC) $(CFLAGS) -o $@ tests/$@/$@.c

custom:
	$(CC) $(CFLAGS) -shared -o libcustom.so tests/custom/libcustom.c
	$(CC) $(CFLAGS) -Wl,-rpath,'$(CURDIR)' -L. -o $@ tests/custom/custom.c -lcustom

recoursive:
	$(CC) $(CFLAGS) -shared -o lib1.so tests/recoursive/lib1.c
	$(CC) $(CFLAGS) -shared -o lib2.so tests/recoursive/lib2.c
	$(CC) $(CFLAGS) -Wl,-rpath,'$(CURDIR)' -L. -o $@ tests/recoursive/recoursive.c -l1 -l2

full-relro:
	$(CC) $(CFLAGS) -Wl,-z,now -o $@ tests/$@/$@.c

clean:
	rm -rf $(TEST_BINS) libvtrace.so lib1.so lib2.so libcustom.so
