CFLAGS=-c -Wall -O2

all: libccs811.a

libccs811.a: ccs811.o
	ar -rc libccs811.a ccs811.o -o ;\
	sudo cp libccs811.a /usr/local/lib ;\
	sudo cp ccs811.h /usr/local/include

ccs811.o: ccs811.c
	$(CC) $(CFLAGS) ccs811.c

clean:
	rm *.o libccs811.a
