#!/bin/bash
cd build
perf record -g -F max ./web_server
perf script > out.perf

../profiling_tools/stackcollapse-perf.pl out.perf > out.folded
../profiling_tools/flamegraph.pl out.folded > kernel.svg
firefox kernel.svg 