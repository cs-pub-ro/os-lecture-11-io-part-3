CPPFLAGS = -I../../utils
CFLAGS = -Wall -Wextra -Wno-unused -Wno-unused-parameter -g

.PHONY: all clean

all: server epoll_server

server: server.o sock_util.o #log.o

server.o: server.c ../../utils/utils.h ../../utils/debug.h ../../utils/sock/sock_util.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

epoll_server: epoll_server.o sock_util.o #log.o

epoll_server.o: epoll_server.c ../../utils/utils.h ../../utils/debug.h ../../utils/sock/sock_util.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

sock_util.o: ../../utils/sock/sock_util.c ../../utils/sock/sock_util.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

#log.o: ../../utils/log/log.c ../../utils/log/log.h
#	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *~
	-rm -f server.o server
	-rm -f epoll_server.o epoll_server
	-rm -f sock_util.o
#	-rm -f log.o
