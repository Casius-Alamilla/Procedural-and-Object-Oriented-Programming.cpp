/* Write a program that prompts the user to input a positive integer called num. This program
will use a loop in order to validate the user input. This means that the program will continue
to prompt the user to enter a positive integer until the user has inputted a positive integer for
num.
The program will then use a loop to output every even integer from 0 up to but not including
num. You can assume that the user will only enter integers into the Console Window (not
letters, punctuation marks, are special characters).
*/

#include <iostream>
using namespace std;

int main () {
    
   int num; // variable to hold user input 

    do {
        cout << "Please enter a positive integer: "; // Prompt user for input
        cin >> num; // reads User Input 
    } 
    while (num <= 0); // Validate input to ensure it's positive
   
    cout << "Even integers from 0 to " << num << " are:" << endl; // Output even integers
   
    for (int i = 0; i < num; i+= 2) // Loop to output even integers from 0 to num
    {
        cout << i << " "; // Output even integer

    }
    cout << endl; // New line for better output formatting
    return 0;

}

