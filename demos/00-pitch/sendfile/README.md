# Using Sendfile

[`sendfile()`]() is a [zero-copy mechanism]() mechanism for improving transfer speed.


The buffer size used by `send()` and `recv()` calls affects I/O performance.

This example demonstrates how using `sendfile()` improves the transfer speed.

Build with:

```console
make
```

First run the server on one console and pass it the buffer size as argument:

```console
$ ./server 8192
server: time passed 339311 microseconds

$ ./server 4096
server: time passed 438920 microseconds

$ ./server 2048
server: time passed 615585 microseconds

$ ./server 16384
server: time passed 293296 microseconds

$ ./server 32768
server: time passed 287239 microseconds
```

And, on another console run the client:

```console
$ ./client
```

We can see that a larger buffer size on the server side results in a larger performance.
Up to a time that the larger memory allocation doesn't improve the transfer speed that much.

Moreover we can see improvements of using `sendfile()` on the client side.
The transfer speed is doubled.

Also, see below a comparison of times when running a standard client and one with `sendfile()`:

```console
$ \time -v ./client 8192
client: time passed 705216 microseconds
        Command being timed: "./client 8192"
        User time (seconds): 0.03
        System time (seconds): 0.66
        Percent of CPU this job got: 99%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.70
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 2144
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 91
        Voluntary context switches: 2
        Involuntary context switches: 9
        Swaps: 0
        File system inputs: 0
        File system outputs: 0
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0

$ \time -v ./client
client: time passed 333214 microseconds
        Command being timed: "./client"
        User time (seconds): 0.00
        System time (seconds): 0.13
        Percent of CPU this job got: 41%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.33
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 2016
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 91
        Voluntary context switches: 2245
        Involuntary context switches: 8
        Swaps: 0
        File system inputs: 0
        File system outputs: 0
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

We can see that the `sendfile()`-based client spend much less time in computation: 41% CPU usage vs 99% and 0.13s time in kernel mode vs 0.66s time in in kernel mode.
Also `sendfile()` manages to quickly fill the socket buffer, resulting in many more voluntary context switches (`2245` vs `2`).
Overall, the `sendfile()` approach provides increased transfer speed and reduced CPU consumption.
