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
#define FILE_TO_SEND	"file_to_send.dat"

#define diff_us(ta, tb)         \
	(((ta).tv_sec - (tb).tv_sec) * 1000 * 1000 + \
	 ((ta).tv_nsec - (tb).tv_nsec) / 1000)

unsigned short remote_port = 4242;
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

static void send_data(int sockfd)
{
	int rc;
	unsigned long i, bytes_sent;
	struct timespec time_before, time_after;
	int fd;

	fd = open(FILE_TO_SEND, O_RDONLY);
	DIE(fd < 0, "open");

	/* Write data to file. */
	clock_gettime(CLOCK_REALTIME, &time_before);
	for (i = 0; i < NUM_ROUNDS; i++) {
		lseek(fd, 0, SEEK_SET);
		for (bytes_sent = 0; bytes_sent < SIZE_TO_WRITE; ) {
			rc = read(fd, buffer, bufsize);
			DIE(rc < 0, "read");
			rc = send(sockfd, buffer, rc, 0);
			DIE(rc < 0, "send");
			bytes_sent += rc;
		}
	}
	clock_gettime(CLOCK_REALTIME, &time_after);
	printf("client: time passed %ld microseconds\n", diff_us(time_after, time_before));
}

int main(int argc, char **argv)
{
	int rc;
	int fd;
	int sockfd;

	if (argc != 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	parse_bufsize(argv[1]);
	initialize_buffer();

	/* Create socket. */
	fd = socket(PF_INET, SOCK_STREAM, 0);
	DIE(fd < 0, "socket");

	sockfd = tcp_connect_to_server("localhost", remote_port);
	DIE(sockfd < 0, "tcp_connect_to_server");

	send_data(sockfd);

	close(sockfd);

	return 0;
}
