
/*
Write a C++ program that uses a function called IsPrime to determine if a number is prime or not. 
IsPrime takes in an integer as an argument. IsPrime returns true if the number inputted is prime. It returns false otherwise. 
The main function will call IsPrime in a loop and use the returned value to output a message indicating if the number is prime or not. 
The main function will continue to use the loop until the user indicates that they want to quit. You can assume that the user will always enter a positive integer.
*/

#include <iostream>
using namespace std;
    

bool IsPrime(int num) { 
    if (num <= 1) { // 1st Check for numbers less than or equal to 1, if <= 1, number is not prime
        return false; 
    }
    for (int divisor = 2; divisor <= sqrt(num); ++divisor) { // Then 2nd, It Checks if divisors from 2 to the square root of num
        if (num % divisor == 0) { // If num is divisible any divisor return false, 
            return false; 
        }
    }
    return true; // Else returns true, if no divisors are found
}

int main() {
    char response; // Variable to store user response
    unsigned int num; // Variable to store the input number

    do {
        cout << "Enter a positive integer" << endl; 
        cin >> num; 
        if (IsPrime(num)) { // Call IsPrime and check the result
            cout << "This number is prime" << endl; // Output if the number is prime
        } else {
            cout << "This number is not prime" << endl; // Output if the number is not prime
        }

        cout << "The prime numbers up to " << num << " are: "; // Output prime numbers up to the input number
        for (unsigned int i = 2; i <= num; ++i) { // Start from 2, the first prime number
            if (IsPrime(i)) { // call and Check if i is prime
                cout << i << ","; // Output the prime number
            }
        }
        cout << endl;
        cout << "Do you want to quit? (Y/N)" << endl; // Ask user if they want to quit
        cin >> response;
    } while ((response != 'Y') && (response != 'y')); // Continue until user chooses to quit
    return 0;
}





