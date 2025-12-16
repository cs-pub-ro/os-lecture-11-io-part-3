/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "utils.h"
#include "sock/sock_util.h"

#define NUM_ROUNDS      5*1024
#define SIZE_TO_WRITE   (1024*1024)

#define diff_us(ta, tb)         \
	(((ta).tv_sec - (tb).tv_sec) * 1000 * 1000 + \
	 ((ta).tv_nsec - (tb).tv_nsec) / 1000)

#define LISTEN_BACKLOG	10

unsigned short port = 4242;
static char *buffer;
static unsigned long bufsize;

static void usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s <bufsize>\n", argv0);
	fprintf(stderr, "    bufsize >= 1, bufsize <= 128K\n");
}

static void parse_bufsize(const char *arg)
{
	char *endp;

	bufsize = strtoul(arg, &endp, 10);
	if (*endp != '\0' || errno == ERANGE || bufsize > 128 * 1024 || bufsize == 0) {
		fprintf(stderr, "Argument is invalid or out of range\n");
		exit(EXIT_FAILURE);
	}
}

static void initialize_buffer(void)
{
	buffer = malloc(bufsize);
	DIE(buffer == NULL, "malloc");
	memset(buffer, 0, bufsize);
}

static void receive_data(int fd)
{
	int rc;
	unsigned long i, bytes_recvd;
	struct timespec time_before, time_after;

	/* Write data to file. */
	clock_gettime(CLOCK_REALTIME, &time_before);
	for (i = 0; i < NUM_ROUNDS; i++) {
		for (bytes_recvd = 0; bytes_recvd < SIZE_TO_WRITE; ) {
			rc = recv(fd, buffer, bufsize, 0);
			DIE(rc < 0, "recv");
			if (rc == 0)
				goto out;
			bytes_recvd += rc;
		}
	}
out:
	clock_gettime(CLOCK_REALTIME, &time_after);
	printf("server: time passed %ld microseconds\n", diff_us(time_after, time_before));
}

int main(int argc, char **argv)
{
	int rc;
	int listenfd, sockfd;
	struct sockaddr_in raddr;
	socklen_t raddrlen;

	if (argc != 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	parse_bufsize(argv[1]);
	initialize_buffer();

	/* Create socket. */
	listenfd = tcp_create_listener(port, LISTEN_BACKLOG);
	DIE(listenfd < 0, "tpc_create_listener");

	/* Accept connection. */
	raddrlen = sizeof(raddr);
	sockfd = accept(listenfd, (SSA *) &raddr, &raddrlen);
	DIE(sockfd < 0, "accept");

	receive_data(sockfd);

	close(sockfd);

	return 0;
}
