/*
    Write a program that simulates 2 users rolling 2 dice each.
    1. Declare two integer variables in order to store both user's rolls. These values should be between 2 and 12.
    2. Use srand(), time(NULL), and rand() in order to generate numbers between 2 and 12 for
    both players.
    3. Output to the console:
    A. The numerical value of each player's roll
    B. Which player had the higher roll or display a message stating that the rolls were
    equal.
    Sample Run:
    Player 1 rolled: 4
    Player 2 rolled: 7
    Player 2 won
*/


// Headers and namespaces
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {

    // Seed the random number generator
    srand(time(NULL));

    // Generate random rolls for both players (between 2 and 12)
    int player1 = rand() % 11 + 2, player2 = rand() % 11 + 2;
    
    // Output the rolls
   cout << "Player 1 Rolled: " << player1 << "\n";
   cout << "Player 2 Rolled: " << player2 << "\n";

    // Determine and output the winner
   if (player1 > player2) {
    cout << "Player 1 Won!" << "\n";
   }
    else if (player2 > player1) {
        cout << "Player 2 Won!" << "\n";
    }
    else {
        cout << "It's a Tie!" << "\n";
    }

    return 0;
}


