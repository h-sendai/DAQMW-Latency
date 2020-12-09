#!/bin/sh

do_run()
{
    local data_size
    rm -f /tmp/*.dat
    data_size=$1
    run.py -l latency-sender-logger-${data_size}kB.xml
    sleep 5
    daqcom http://localhost/daqmw/scripts/ -c
    sleep 2
    daqcom http://localhost/daqmw/scripts/ -b 1
    sleep 10
    daqcom http://localhost/daqmw/scripts/ -e
    sleep 2
    cp /tmp/*.dat log/${data_size}.dat
    pkill -f Comp
}

for i in {0..12}; do
    data_size=$((2**${i}))
    echo "---> $data_size"
    do_run $data_size
done
