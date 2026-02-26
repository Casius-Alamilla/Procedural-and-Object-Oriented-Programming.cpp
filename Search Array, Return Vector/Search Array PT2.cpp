#include <iostream>
#include <vector>

using namespace std;

void search_even (const vector<vector<int>> &arr, vector<int> &locations ){
    for (int row = 0; row < (int)arr.size(); row++) {
        for (int col = 0; col < (int)arr[row].size(); col++) {
            if (arr[row][col] % 2 == 0 ) {
            locations.push_back(row);
            locations.push_back(col);
            }
        }
    }
    if (locations.size() == 0) {
        locations.push_back(-1);
    }
}

int main () {

    vector<vector<int>>arr = {
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
    };

    vector<int> locations;
    search_even(arr, locations);
    if (locations.size() == 1 && locations[0] == -1) {
        cout << "No Even Numbers in array. " << endl;
    } else {
        for (int i = 0; i < (int)locations.size(); i += 2 ) {
            cout << locations[i] << " " << locations[i + 1] << endl;
        }
    }

    return 0;
}
