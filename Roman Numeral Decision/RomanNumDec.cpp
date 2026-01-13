/* Write a program that asks the user to enter a number within the range of 1
through 10 (inclusive). Use if, else, or else if statements to display the Roman
numeral equivalent of that number.
Input Validation: Do not accept a number less than 1 or greater than 10.
1 - I
2 - II
3 - III
4 - IV
5 - V
6 - VI
7 - VII
8 - VIII
9 - IX
10 - X

Sample Run 1:
    Enter a number between 1 and 10: 88
    This number is out of range
Sample Run 2:
    Enter a number between 1 and 10: 5
    Roman numeral equivalent = V
*/


#include <iostream>
using namespace std;


int main() {

    int n; // Variable to store user input
    cout << "Enter a number between 1 and 10: "; // Prompt user for a number between 1 and 10
    
    if (!(cin >> n)|| n < 1 || n > 10)  // Validate input and convert to Roman numeral if valid
    cout << "This number is out of range\n"; // Display error message if input is invalid
    
    else 
    {
    const char * r[] = {"","I","II","III","IV","V","VI","VII","VIII","IX","X"}; // Array of Roman numerals
    cout << "Roman numeral equivalent = " << r[n]<<'\n'; // Output the Roman numeral equivalent
    } 

    return 0;

}
