#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define RNG_MOD 0x80000000
 
using namespace std;

int state;

int rng_int(void);
double rng_doub(double range);

int main(int argc, char *argv[]){
    // runs, mc iterations
    
    int points=1000; 
    int threads = 1;
     
    switch (argc) 
    {
    case 3:
        threads = atoi(argv[2]); 
    case 2:
        points = atoi(argv[1]);
        break;
    case 1:
        break;
    default:
        cout << "Invalid number of arguments" << endl;
        return 1;
    }
    
    double x,y;
    int hit, i;
    
    //cout << "points: " << points << ", threads: " << threads  << endl;
    
    hit=0;
    
    #pragma omp threadprivate(state)
    #pragma omp parallel private(x, y) reduction(+:hit) num_threads(threads)
    {
        state = 25234 + 17 * omp_get_thread_num();
        
        #pragma omp for
        for(i = 0; i < points; i++)
        {
            x = (double) rng_doub (1.0);
            y = (double) rng_doub (1.0);
            
            if (x*x + y*y <= 1)
            { 
                hit++;
            } 
        }              
    }
    //      "PI:"
    cout << "" << 4 * double (hit) / double (points) << endl; 
    
    return 0;
}

int rng_int(void) {
   // & 0x7fffffff is equivalent to modulo with RNG_MOD = 2^31
   return (state = (state * 1103515245 + 12345) & 0x7fffffff);
}

double rng_doub(double range) {
    return ((double)rng_int()) / (((double)RNG_MOD)/range);
}

