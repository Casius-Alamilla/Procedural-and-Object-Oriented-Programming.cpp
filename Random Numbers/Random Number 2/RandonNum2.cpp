// Minimal version: prints a random even number from {2,4,6,8,10}
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;



int main()
{ 
    srand(time(NULL)); // seed the random number generator
    
    // print a random even number from the set {2,4,6,8,10}
    cout << 2 * ((rand() % 5) + 1) << endl;

    return 0;
}

