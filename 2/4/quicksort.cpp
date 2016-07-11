#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

using namespace std;

int state;

void quickSort(int * a, int p, int r);
int rng_int(void);

int main(int argc, char *argv[]) {

    int threads = 1;
    int k = 10;

    switch (argc)
    {
        case 3:
            threads = atoi(argv[2]);
        case 2:
            k = atoi(argv[1]);
            break;
        case 1:
            break;
        default:
            cout << "Invalid number of arguments" << endl;
            return 1;
    }
    
    if (k < 6) {cout << "===> Runnig for k: " << k << " and threads: " << threads << endl;}

    int i;
    long n = (long) pow(2, k);
    int* arr = new int[n];

    #pragma omp threadprivate(state)
    #pragma omp parallel num_threads(1)
    {
        state = 25234 + 17 * omp_get_thread_num();
        #pragma omp for
        for(i = 0; i < n; i++)
        {
            arr[i] = rng_int();
        }
    }
        
    if (k < 6) 
    {
        for (int i = n - 1; i >= 0; i--) 
        {
            cout << arr[i] << " ";
        }
        cout << endl << "===> Sorted array:" << endl;
    }
    
    quickSort(arr, 0, n-1);
    
    if (k < 6) 
    {
        for (int i = n - 1; i >= 0; i--) 
        {
            cout << arr[i] << " ";
        }
        cout << endl << "<===/Done/===>" << endl << endl;
    }

    return 0;
}

void quickSort(int arr[], int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = arr[(left + right) / 2];


    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    };

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            if (left < j)
            {
                quickSort(arr, left, j);
            }
        }
        #pragma omp section
        {
            if (i < right)
            {
                quickSort(arr, i, right);
            }
        }
    }
}

int rng_int(void) {
    // & 0x7fffffff is equivalent to modulo with RNG_MOD = 2^31
    return (state = (state * 1103515245 + 12345) & 0x7fffffff);
}
