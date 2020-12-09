#!/bin/sh

for i in {0..12}; do
    data_size=$((2**${i}))
    sed -e s"|%data_size%|$data_size|" latency-sender-logger.xml.in > latency-sender-logger-${data_size}kB.xml
done
