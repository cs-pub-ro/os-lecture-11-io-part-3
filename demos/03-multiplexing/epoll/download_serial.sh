#!/bin/bash

mkdir out-data > /dev/null 2>&1
for i in $(seq -f "%04g" 0 99); do
    echo "file_to_send.dat" | nc localhost 42424 > /dev/null
done
