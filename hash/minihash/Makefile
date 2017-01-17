CC= gcc
INC = -I ./ 
LIB = -lpthread
LIBOBJS = minihash.o
CFLAGS=-O2 -Wall

.c.o:
	$(CC) -c $< $(CFLAGS)

all: libminihash

libminihash: $(LIBOBJS)
	ar -r $@.a $(LIBOBJS)
	mv libminihash.a ../lib

clean:
	rm -f *.o *~ *.log demo libminihash.a 
	rm -f *.txt

dist:
	rm -f ../samples/libminihash.a ../samples/minihash.h
	cp -f ./libminihash.a ../samples/libminihash.a
	cp -f ./minihash.h ../samples/minihash.h
	rm -f ../../libshares/libs/libminihash.a ../../libshares/include/minihash.h
	cp -f ./libminihash.a ../../libshares/libs/
	cp -f ./minihash.h ../../libshares/include/
#	cp -f ../samples/vshell_demo.c ../../libshares/samples
#	cp -f ../samples/Makefile ../../libshares/samples/Makefile.vshell.demo

