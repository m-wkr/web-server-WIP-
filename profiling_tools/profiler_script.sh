#!/bin/bash

perf record -g -F 99 ../build/web_server
perf script > out.perf

./stackcollapse-perf.pl out.perf > out.folded
./flamegraph.pl out.folded > kernel.svg
firefox kernel.svg 