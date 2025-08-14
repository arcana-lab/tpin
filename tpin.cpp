#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <regex>
#include <sched.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#ifndef __clang__
#error "This code must be compiled with clang++"
#endif

using namespace std;
namespace fs = std::filesystem;

int get_number_of_numa_domains() {
  const std::string numa_path = "/sys/devices/system/node";
  std::regex node_regex("node[0-9]+");
  int count = 0;

  for (const auto &entry : fs::directory_iterator(numa_path)) {
    if (fs::is_directory(entry)) {
      std::string name = entry.path().filename().string();
      if (std::regex_match(name, node_regex)) {
        ++count;
      }
    }
  }

  return count;
}

int get_threads_per_core() {
  std::ifstream file("/sys/devices/system/cpu/cpu0/topology/core_cpus_list");
  std::string line;
  if (std::getline(file, line)) {
    int count = 1;
    for (char c : line) {
      if (c == ',') {
        count++;
      }
    }
    return count;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  // Check for -l or --list arguments
  bool csv_print = false;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {
      csv_print = true;
      break;
    }
  }

  int nthreads_per_core = get_threads_per_core();
  int ndomains = get_number_of_numa_domains();
  int ncores = thread::hardware_concurrency() / nthreads_per_core;
  int ncores_per_domain = ncores / ndomains;

  vector<vector<int>> pin_counter(ndomains, vector<int>(ncores_per_domain, 0));

#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    int cpu = sched_getcpu();
    int N = omp_get_num_threads();
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
    for (int i = 0; i < N; i++) {
#pragma omp ordered
      if (tid == i) {
        if (csv_print) {
          printf("%i%s", cpu, (i == N - 1) ? "\n" : ",");
        } else {
          printf("%3i on %3i %s\n", tid, cpu, graphical.c_str());
        }
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
  if (!csv_print) {
    printf("    counts %s\n", pinned.c_str());
  }

  return 0;
}
