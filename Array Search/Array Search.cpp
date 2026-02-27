
#include <iostream>
#include <vector>

using namespace std;

const int R = 5;
const int C = 5;

vector<int> search_even (int arr [R][C]) {
   vector<int> locations;
   int a = 5;
      for (int row = 0; row < R; row++){
         for (int col = 0; col < C; col++){
            if (arr[row][col] % 2 == 0) {
                locations.push_back(row);
                locations.push_back(col);
            }
         }
      }
      if (locations.empty()) {
         locations.push_back(-1);
      }
   return locations;
} 


int main () {
   int arr [R][C] = {
      {1,2,3,4,5},
      {1,2,3,4,5},
      {1,2,3,4,5},
      {1,2,3,4,5},
      {1,2,3,4,5},
   };

   vector<int> locations = search_even(arr);
      if (locations.size() == 0 && locations[0] == -1) {
         cout << "There are no even numbers" << endl;
      } else {
         for (int i = 0; i + 1 < locations.size(); i+=2 ) {
            cout << locations[i] << " " << locations[i + 1] << endl;
         }
      }

   return 0;

}
