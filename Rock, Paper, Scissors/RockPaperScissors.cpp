/*
Write a C++ program that will complete the following steps:
    0. Declare variables
        a. P1_int_val, P2_int_val as type int
        b. P1_string_val, P2_string_val as type string

    1. Use srand(), time(NULL), and rand() to assign a random integer between [0,2] to
        1_int_val and P2_int_val

    2. Based off of the numerical values stored inside of P1_int_val and P2_int_val, use if, else, and else if
        statements to store the following strings in P1_string_val, P2_string_val

int_val         string_val
0               rock
1               paper
2               scissors

3. Output the string_vals of both players to the console.

4. Use a function to compute and return an integer value to determine who won. Try to complete this
    step using less than 6 operations.

    This function will take in two integers as parameters:
        1. P1 – this will store a copy of P1_int_val
        2. P2 – this will store a copy of P2_int_val
    
        This function will return an integer value to indicate a tie or which player won. The following defines
        the possible return values:
        0 – Tie
        1 – Player 1
        2 – Player 2

    5. Based off of what the function returned in Step 4, Output either “Tie” or a message stating who won.
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>


int determineWinner(int P1, int P2) {
    // 0: Tie, 1: Player 1 wins, 2: Player 2 wins
    if (P1 == P2) return 0;
    return (P1 - P2 + 3) % 3 == 1 ? 1 : 2;
}

int main() {
    const char* options[] = {"Rock", "Paper", "Scissors"};
    srand(time(NULL));

    int P1_int = rand() % 3;
    int P2_int = rand() % 3;

    std::cout << "Player 1: " << options[P1_int] << std::endl;
    std::cout << "Player 2: " << options[P2_int] << std::endl;

    int winner = determineWinner(P1_int, P2_int);

    if (winner == 0)
        std::cout << "It's a Tie!" << std::endl;
    else
        std::cout << "Player " << winner << " wins!" << std::endl;

    return 0;
}
