#include <stdio.h>
#include <stdlib.h>

int main()
{
  int i, j, k, sum = 0;
  int n = 1000;
  int** first;
  int** second;
  int** multiply;

  first = (int**) malloc(n*sizeof(int*));
  second = (int**) malloc(n*sizeof(int*));
  multiply = (int**) malloc(n*sizeof(int*));

  for ( i = 0 ; i < n ; i++ )
  {
    first[i] = (int*) malloc(n*sizeof(int));
    second[i] = (int*) malloc(n*sizeof(int));
    multiply[i] = (int*) malloc(n*sizeof(int));
  }

  for ( i = 0 ; i < n ; i++ )
  {
    for ( j = 0 ; j < n ; j++ )
    {
      first[i][j] = 1;
      second[i][j] = 2;
      multiply[i][j] = 0;
    }
  }

  printf("matrizes inicializadas\n");

    for ( i = 0 ; i < n ; i++ )
    {
      for ( j = 0 ; j < n ; j++ )
      {
        for ( k = 0 ; k < n ; k++ )
        {
          sum = sum + first[i][k]*second[k][j];
        }

        multiply[i][j] = sum;
        sum = 0;
      }
    }

  printf("acabou\n");  
  return 0;
}
