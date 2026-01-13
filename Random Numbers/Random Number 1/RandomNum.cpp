/* 
    Write a program that uses srand (), time(NULL), and rand() in order to:
    1. Output a random number between 1 and 6 (1 and 6 inclusive) to the console screen.
    2. Output a random letter between A and Z (A and Z inclusive) to the console screen
*/

#include <iostream>
#include <ctime> 

using namespace std;

int main () {
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate a random number between 1 and 6
    int randomNumber = (rand() % 6) + 1;
    cout << "Random number between 1 and 6: " << randomNumber << endl;

    // Generate a random letter (A-Z or a-z) using a single rand
    int r = rand() % 52;
    cout << "Random letter (A-Z or a-z): " << char((r < 26 ? 'A' : 'a' - 26) + r) << endl;

    return 0;
} 


