CPPFLAGS = -I../../utils
CFLAGS = -Wall -Wextra -Wno-unused -Wno-unused-parameter -g

.PHONY: all clean

all: server client

server: server.o sock_util.o

server.o: server.c ../../utils/utils.h ../../utils/sock/sock_util.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

client: client.o sock_util.o

client.o: client.c ../../utils/utils.h ../../utils/sock/sock_util.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

sock_util.o: ../../utils/sock/sock_util.c ../../utils/sock/sock_util.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *~
	-rm -f server.o server
	-rm -f client.o client
	-rm -f sock_util.o
