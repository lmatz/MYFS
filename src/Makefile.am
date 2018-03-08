# what flags you want to pass to the C compiler & linker
CFLAGS = --pedantic -Wall -O2 -D_FILE_OFFSET_BITS=64
LIBS = `pkg-config fuse --cflags --libs`

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = nullfs
nullfs_SOURCES = null.c 
