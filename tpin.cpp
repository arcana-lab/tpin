#include <cstdio>
#include <vector>
#include <thread>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s <nthreads_per_core> <nsockets>\n", argv[0]);
    return 1;
  }

  int nthreads_per_core = atoi(argv[1]);
  int nsockets = atoi(argv[2]);
  int ncores = thread::hardware_concurrency() / nthreads_per_core;
  int ncores_per_socket = ncores / nsockets;

  vector<vector<int>> pin_counter(nsockets, vector<int>(ncores_per_socket, 0));

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int cpu = sched_getcpu();
    int n = omp_get_num_threads();
    int sock = cpu % nsockets;
    int core = (cpu / nsockets) % ncores_per_socket;
    int hw_thread_idx = cpu / ncores;

    #pragma omp atomic
    pin_counter[sock][core]++;

    string graphical = "";

    for (int j = 0; j < nsockets; j++) {
      string s(ncores_per_socket, '-');
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
  for (int i = 0; i < nsockets; i++) {
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
  printf("           %s\n", pinned.c_str());

  return 0;
}
