/*
    Exercise 1:
    Write a program that uses srand (), time(NULL), and rand() in order to:
    1. Output a random number between 1 and 6 (1 and 6 inclusive) to the console screen.
    2. Output a random letter between A and Z (A and Z inclusive) to the console screen.

*/

#include <iostream>
#include <ctime>
#include <random>

using namespace std;


int main () {

    srand (time(NULL));
    int random_num, min_value, max_value;
    char first_letter, last_letter, random_letter;

    // Random Numbers 
    cout << "What is your range of numbers (ex:1 - 10): " << endl;
    cout << "Enter min Value: " << endl;
    cin >> min_value;
    cout << "Enter max value: " << endl;
    cin >> max_value;
    
    //Random Number Formula
    random_num = (rand () % (max_value - min_value + 1 )) + min_value;
    
    //Random Number Output
    cout << "Your numbers are " << min_value << " and " << max_value << endl;
    cout << random_num << endl;

    //Random Letter
    cout << "\nWhat is your range of Letter (ex: A - Z): " << endl;
    cout << "Enter First Letter: " << endl;
    cin >> first_letter;
    cout << "Enter Last Letter " << endl;
    cin >> last_letter;
    
    //Random Letter Formula
    random_letter = random_letter = (rand() % (last_letter - first_letter + 1)) + first_letter;

    //Random Letter Output
    cout << "Your Letters are " << first_letter << " and " << last_letter << endl;
    cout << random_letter << endl;

    return 0;
}
