 /* 
Write a C++ program that will simulate a lottery.

1. Prompt the user to choose 3 numbers that he thinks will be the lottery numbers (between 0 and
    100, 100 is not included).
2. Store those numbers in 3 integer variables: num1, num2, num3.
3. Write a loop that will simulate a lottery commission choosing 3 random numbers. This loop will
    continue to run until the 3 numbers that the user chose gets selected. This loop will keep track of how
    many iterations it takes for the loop to generate those 3 numbers.
4. Output the number of iterations it took for the program to generate those numbers


Part 2:
Modify the Lottery Simulator such that it will continue to prompt the user to input 3 numbers
that are in the range of 0 – 99 (inclusive) until the user enters 3 numbers that are in that range.
The loop will output which numbers were out of range
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    // Step 1: Prompt the user to choose 3 numbers
    int choice1, choice2, choice3; 

    // Simple reprompt until all three are in [0, 99]
    bool inValid;
    do {
        inValid = false;
        cout << "Enter three integers between 0 and 99: " << endl;
        cin >> choice1 >> choice2 >> choice3;

        if (choice1 < 0 || choice1 > 99) 
            { cout << choice1 << " is out of range." << endl; inValid = true; }
        if (choice2 < 0 || choice2 > 99) 
            { cout << choice2 << " is out of range." << endl; inValid = true; }
        if (choice3 < 0 || choice3 > 99) 
            { cout << choice3 << " is out of range." << endl; inValid = true; }
            
    } while (inValid);
    cout << "You entered the numbers: " << choice1 << ", " << choice2 << ", " << choice3 << endl;

    //Loop until the lottery commission generates the same 3 numbers
    srand(time(NULL));
    int lottery1, lottery2, lottery3;
    unsigned long iterations = 0;

    do {
        lottery1 = rand() % 100;
        lottery2 = rand() % 100;
        lottery3 = rand() % 100;
        iterations++;
    } while (lottery1 != choice1 || lottery2 != choice2 || lottery3 != choice3);

    //Output the number of iterations
    cout << "Lottery matched after " << iterations << " iterations." << endl;
    
    return 0;
}

