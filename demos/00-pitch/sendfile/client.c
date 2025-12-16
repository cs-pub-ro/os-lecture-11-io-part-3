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
#include <sys/sendfile.h>
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
			rc = sendfile(sockfd, fd, NULL, SIZE_TO_WRITE-bytes_sent);
			DIE(rc < 0, "sendfile");
			bytes_sent += rc;
		}
	}
	clock_gettime(CLOCK_REALTIME, &time_after);
	printf("client: time passed %ld microseconds\n", diff_us(time_after, time_before));
}

int main(void)
{
	int rc;
	int fd;
	int sockfd;

	/* Create socket. */
	fd = socket(PF_INET, SOCK_STREAM, 0);
	DIE(fd < 0, "socket");

	sockfd = tcp_connect_to_server("localhost", remote_port);
	DIE(sockfd < 0, "tcp_connect_to_server");

	send_data(sockfd);

	close(sockfd);

	return 0;
}
