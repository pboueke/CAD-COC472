#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>

 
using namespace std;

int main(int argc, char *argv[]){
    // runs, mc iterations
    
    int runs=1000; 
    int mcits=1000; 
     
    switch (argc) 
    {
    case 3:
        mcits = atoi(argv[2]);
    case 2:
        runs = atoi(argv[1]);
        break;
    case 1:
        break;
    default:
        cout << "Invalid number of arguments" << endl;
        return 1;
    }
    
    double x,y;
    int hit;
    
    srand(time(0));
    
    ostringstream s;
    s << "runs: " << runs << ", mcits: " << mcits;
    cout << s.str() << endl;
    
    for (int j = 0; j < runs; j++){
        
        hit=0;
        x=0; y=0;
        
        for(int i = 0; i < mcits; i++)
        {
            x= double (rand ()) / double (RAND_MAX);
            y= double (rand ()) / double (RAND_MAX);
            
            if (y <= sqrt (1 - pow(x, 2))) 
            { 
                hit+=1;
            } 
        }        
          
        cout << "" << 4 * double (hit) / double (mcits) << endl; 
    }
    
    return 0;
}
