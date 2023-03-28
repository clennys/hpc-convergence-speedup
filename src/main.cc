#include "../include/iterative_solvers/parallel.h"
#include "../include/iterative_solvers/serial.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 4) {
    if (strcmp(argv[1], "-s") == 0) {
      run_serial(argv[2]);
    } else if (strcmp(argv[1], "-p") == 0) {
      run_parallel(argv[2]);
    } else {
      cout << "Select Computing Mode with -p (parallel) or -s (serial)!"
           << endl;
    }
  }
  return 0;
}
