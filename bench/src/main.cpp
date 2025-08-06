#include <benchmark/benchmark.h>
#include <iostream>


static request exampleReq;

static void BM_oldParser(benchmark::State& state) {

  for (auto _ : state) {
    requestParser(exampleReq);
  }
}

BENCHMARK(BM_oldParser)->Repetitions(5);


BENCHMARK_MAIN();