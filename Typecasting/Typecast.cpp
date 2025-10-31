/*
    Write a complete C++ program that meets the following requirements:
     1. Prompt the user to input a lowercase letter.
     2. Input and store that letter in memory.
     3. Output: "The Uppercase version of that letter is:"______

    Sample Run:
     Input a lowercase letter a
     The Uppercase version of this letter is A
*/


#include <iostream>
using namespace std;

int main (){

 // Declare variables, Use char data type, do not use string beacuse we are only dealing with a single character
    char lowercase, uppercase;
    cout << "Lowercase letter: ";
    cin >> lowercase;

 /* Convert to uppercase using ASCII values, the difference between lowercase and uppercase letters in ASCII is 32, 
    so we subtract 32 from the lowercase letter to get the uppercase letter  */
    uppercase = lowercase - 32;

 // Output the result
    cout << " Uppercase Letter is: " << uppercase << endl;
    return 0;
}


