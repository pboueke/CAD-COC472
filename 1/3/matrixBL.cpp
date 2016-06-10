#include <stdlib.h>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[])
{
  int n, i, j, k, x, y, jj, ii, kk, nb;
  char* p;
  
  if (argc == 1) 
  {
    nb = 4000;
  }
  else
  {
    nb = strtol(argv[1], &p, 10);
  }
  n = 2000;
  int** A = new int*[n];
  int** B = new int*[n];
  int** C = new int*[n];

  cout << "Initializing for N = " << to_string(n) << endl;
  cout << "Block size NB = " << to_string(nb) << endl;

  high_resolution_clock::time_point t1 = high_resolution_clock::now();

  for(x = 0; x < n; x++){

      A[x] = new int[n];
      B[x] = new int[n];
      C[x] = new int[n];

      for(y = 0; y < n; y++){
          A[x][y] = 1;
          B[x][y] = 1;
          C[x][y] = 1;
      }

    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

  for(ii = 0; ii < n ; ii += nb){
    for(jj = 0; jj < n ; jj += nb){
      for(kk = 0; kk < n; kk += nb){
        for(i = ii; i < std::min(n, ii + nb); i++){
          for(j = jj; j < std::min(n, jj + nb); j++){
            for(k = kk; k < std::min(n, kk +nb); k++) {
              C[i][j] = C[i][j] + A[j][k] * B[k][i];
            }
          }
        }
      }
    }
  }


  high_resolution_clock::time_point t3 = high_resolution_clock::now();

  duration<double> setup = duration_cast<duration<double>>(t2 - t1);
  duration<double> run = duration_cast<duration<double>>(t3 - t2);

  cout << "Setup Time  = " << to_string(setup.count()) << endl;
  cout << "Run Time = " << to_string(run.count()) << "\n" << endl;

  return 0;
}
