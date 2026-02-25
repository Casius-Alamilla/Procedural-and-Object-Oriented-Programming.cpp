#include <iostream>
#include <vector>

using namespace std;

const int R = 5;
const int C = 5;

// Search_Even: returns a vector holding (row, col) pairs for every even number.
// If there are no even numbers, it returns a vector with one value: -1.
vector<int> Search_Even(int arr[R][C])
{
    vector<int> locations;

    for (int row = 0; row < R; row++) {
        for (int col = 0; col < C; col++) {
            if (arr[row][col] % 2 == 0) {
                locations.push_back(row);
                locations.push_back(col);
            }
        }
    }

    // If none found, store -1 only
    if (locations.size() == 0) {
        locations.push_back(-1);
    }

    return locations;
}

int main()
{
    int arr[R][C] = {
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
    };

    vector<int> locations = Search_Even(arr);

    if (locations.size() == 1 && locations[0] == -1) {
        cout << "The array contained no even numbers." << endl;
    } else {
        for (int i = 0; i < (int)locations.size(); i += 2) {
            cout << locations[i] << " " << locations[i + 1] << endl;
        }
    }

    return 0;
}
