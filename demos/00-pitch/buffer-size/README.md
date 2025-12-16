# Buffer Size for Socket Transfers

The buffer size used by `send()` and `recv()` calls affects I/O performance.

This example demonstrates how using a different buffer size results in different run durations.

Build with:

```console
make
```

First run the server on one console and pass it the buffer size as argument:

```console
$ ./server 1024
server: time passed 2163791 microseconds

$ ./server 2048
server: time passed 1327605 microseconds

$ ./server 4096
server: time passed 900413 microseconds

$ ./server 8192
server: time passed 687157 microseconds

$ ./server 16384
server: time passed 577017 microseconds

$ ./server 32768
server: time passed 554110 microseconds
```

And, on another console run the client:

```console
$ ./client 1024
client: time passed 2163797 microseconds

$ ./client 2048
client: time passed 1327646 microseconds

$ ./client 4096
client: time passed 900494 microseconds

$ ./client 8192
client: time passed 687146 microseconds

$ ./client 16384
client: time passed 577021 microseconds

$ ./client 32768
client: time passed 554130 microseconds
```

We can see that a larger buffer size results in a larger performance up to a time that the larger memory allocation doesn't improve the transfer speed that much.

You can also combine different buffer sizes on the server and on the client.
The slowest component is going to be the deciding point on the actual speed.
