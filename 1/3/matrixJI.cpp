#include <stdlib.h>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[])
{
  int n, i, j, k, x, y;
  char* p;
  
  if (argc == 1) 
  {
    n = 4000;
  }
  else
  {
    n = strtol(argv[1], &p, 10);
  }
  int** A = new int*[n];
  int** B = new int*[n];
  int** C = new int*[n];
  cout << "Initializing for N = " << to_string(n) << endl;

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

    for (j=0; j < n; j++){ //Loop J

      for(i=0; i < n;i++){ // Loop I

          for(k=0; k < n; k++){ //Loop K

            C[i][j] = C[i][j] + A[i][k]*B[k][j];

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
