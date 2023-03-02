#ifndef UTIL
#define UTIL

#include <iostream>
using namespace std;

template <typename T> void print_vector(T *vector, int n) {
  for (int i = 0; i < n; i++) {
    cout << *vector << "\t";
    vector++;
  }
  cout << endl;
}

template <typename T> void print_matrix(T *matrix, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      cout << matrix[i * n + j] << "\t";
    }
    cout << endl;
  }
}

template <typename T> void print_matrix(T *matrix, int m) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      cout << matrix[i * m + j] << "\t";
    }
    cout << endl;
  }
}
#endif // !UTIL
