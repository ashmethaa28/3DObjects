CC = gcc
CFLAGS = -ansi -Wall

all: a1

a1: a1.o
	$(CC) a1.o -o a1 $(CFLAGS) -L/usr/lib/python3.7/config-3.7m-x86_64-linux-gnu -L/usr/lib -lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions

a1.o: a1.c a1.h
	$(CC) a1.c -o a1.o $(CFLAGS) -c -I/usr/include/python3.7m -I/usr/include/python3.7m  -Wno-unused-result -Wsign-compare -g -fdebug-prefix-map=/build/python3.7-3.7.3=. -specs=/usr/share/dpkg/no-pie-compile.specs -fstack-protector -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O3 -Wall -fPIC

clean:
	rm *.o core