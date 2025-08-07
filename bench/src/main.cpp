#include <benchmark/benchmark.h>
#include <iostream>
//#include "requestParser.hpp"
#include "requests.hpp"


static request exampleReq;

static void BM_oldParser(benchmark::State& state) {

  for (auto _ : state) {
    requestParser(exampleReq);
  }
}

BENCHMARK(BM_oldParser)->Repetitions(50);

static void BM_newParser(benchmark::State& state) {

  for (auto _ : state) {
    parser(exampleReq);
  }
}

BENCHMARK(BM_newParser)->Repetitions(50);

BENCHMARK_MAIN();