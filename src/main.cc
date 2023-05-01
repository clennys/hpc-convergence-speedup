#include "../include/iterative_solvers/parallel.h"
#include "../include/iterative_solvers/serial.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 7) {
    if (string(argv[1]) == "-s") {
      if (string(argv[4]) == "-gd") {
        run_serial(argv[2], argv[3], atoi(argv[5]));
      } else {
        cout << "Specify grid dimension" << endl;
        exit(1);
      }
    } else if (string(argv[1]) == "-p") {
      if (string(argv[4]) == "-gd") {
        run_parallel(argv[2], argv[3], atoi(argv[5]));
      } else {
        cout << "Specify grid dimension" << endl;
        exit(1);
      }
    } else if (string(argv[1]) == "-h") {
      cout << "TODO INSTRUCTIONS" << endl;
    } else {
      cout << "Select Computing Mode with -p (parallel) or -s (serial)!"
           << endl;
      exit(1);
    }
  }
  return 0;
}
