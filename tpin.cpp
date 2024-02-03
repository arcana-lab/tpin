#include <cstdio>
#include <vector>
#include <thread>
#include <sched.h>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[]) {
  int nthreads_per_core = THREADS_PER_CORE;
  int ndomains = NUMA_DOMAINS;
  int ncores = thread::hardware_concurrency() / nthreads_per_core;
  int ncores_per_domain = ncores / ndomains;

  vector<vector<int>> pin_counter(ndomains, vector<int>(ncores_per_domain, 0));

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int cpu = sched_getcpu();
    int n = omp_get_num_threads();
    int sock = cpu % ndomains;
    int core = (cpu / ndomains) % ncores_per_domain;
    int hw_thread_idx = cpu / ncores;

    #pragma omp atomic
    pin_counter[sock][core]++;

    string graphical = "";

    for (int j = 0; j < ndomains; j++) {
      string s(ncores_per_domain, '-');
      if (j == sock) {
        s[core] = '0' + hw_thread_idx;
      }
      graphical += "[" + s + "] ";
    }

    #pragma omp for ordered
    for (int i = 0; i < n; i++) {
      #pragma omp ordered
      if (tid == i) {
        printf("%3i on %3i %s\n", tid, cpu, graphical.c_str());
      }
    }
  }

  string pinned = "";
  for (int i = 0; i < ndomains; i++) {
    string tmp = "";
    for (auto c : pin_counter[i]) {
      if (c == 0) {
        tmp += "-";
      } else if (c < 10) {
        tmp += to_string(c);
      } else {
        tmp += "#";
      }
    }
    pinned += "[" + tmp + "] ";
  }
  printf("    counts %s\n", pinned.c_str());

  return 0;
}
