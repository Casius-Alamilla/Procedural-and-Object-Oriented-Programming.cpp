#include <iostream>
using namespace std;

int main() {

    int sideLength, perimeter;
    cout << "Enter the length of a side of the square: ";
    cin >> sideLength;
    perimeter = 4 * sideLength;
    cout << "The perimeter of the square is: " << perimeter << endl;
    return 0;
}
