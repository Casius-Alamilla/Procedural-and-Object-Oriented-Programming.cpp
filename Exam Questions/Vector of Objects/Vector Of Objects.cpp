
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct stock {
    string name;   // Name
    float d1;      // Price on Day 1
    float d2;      // Price on Day 2
    float pchange; // Percent change
};

int main() {

    int vec_size;

    cout << "Enter the number of stocks: ";
    cin >> vec_size;

    vector<stock> stocks(vec_size);

    // Input data and compute percent change
    for (int i = 0; i < vec_size; i++) {

        cout << "\nEnter data for stock " << i + 1 << ":\n";

        cout << "Name: ";
        cin >> stocks[i].name;

        cout << "Price on Day 1: ";
        cin >> stocks[i].d1;

        cout << "Price on Day 2: ";
        cin >> stocks[i].d2;

        // percent change = ((Day2 - Day1) / Day1) * 100
        stocks[i].pchange = ((stocks[i].d2 - stocks[i].d1) / stocks[i].d1) * 100.0f;
    }

    // Find stock with greatest positive percent change
    int maxIndex = 0;
    for (int i = 1; i < vec_size; i++) {
        if (stocks[i].pchange > stocks[maxIndex].pchange) {
            maxIndex = i;
        }
    }

    cout << "\nStock with greatest positive percent change:\n\n";
    cout << "Name: " << stocks[maxIndex].name << endl;
    cout << "Price on Day 1: " << stocks[maxIndex].d1 << endl;
    cout << "Price on Day 2: " << stocks[maxIndex].d2 << endl;

    cout << "Percent change: " << stocks[maxIndex].pchange << "%" << endl;

    return 0;
}
