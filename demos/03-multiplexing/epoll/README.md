# epoll Multiplexing

Multiplexing means the ability to serve multiple connections simultaneously.
The standard call in Linux for multiplexing is [`epoll`](https://man7.org/linux/man-pages/man7/epoll.7.html).

With `epoll` multiple different connections can be handled in a single thread.
We use a classical server implementation and an `epoll` server implementation that serve a file.

To test how it works, first build a classical and an e-poll server:

```console
make
```

Create the file to serve:

```console
./create_file.sh
```

Now, on one console, run the server:

```console
./server
```

Next, you can download the file using clients in a serial manner (and measure the time):

```console
\time -v ./download_serial.sh
```

or in a parallel manner:

```console
\time -v ./download_parallel.sh
```

Repeat the experiment, but use and epoll server:

```console
./epoll_server
```
