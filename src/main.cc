#include "../include/iterative_solvers/parallel.h"
#include "../include/iterative_solvers/serial.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 5) {
    if (strcmp(argv[1], "-s") == 0) {
      run_serial(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-p") == 0) {
      run_parallel(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-h") == 0) {
      cout << "TODO INSTRUCTIONS" << endl;
    } else {
      cout << "Select Computing Mode with -p (parallel) or -s (serial)!"
           << endl;
      exit(1);
    }
  }
  return 0;
}
