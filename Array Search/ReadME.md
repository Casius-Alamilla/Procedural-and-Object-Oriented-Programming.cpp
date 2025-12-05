
Search Array – Return Vector Exercise
Write a C++ function that will search a 5 x 5 2D array of integers. Name this function Search_Even. The 5 x 5 array will be passed as an argument to that function. The function will return a vector that contains every
location of where all of the even numbers are located.

If the array contains no even numbers, the vector will only contain the value -1. Write a complete C++ program that will demonstrate this function working. Output to the console screen a
message indicating that the array contained no even numbers if this is the case. Alternatively, output to the console screen the locations of each even number (one x and one y coordinate per line).


Making an algorithm before attempting to code this program will be helpful to you.

Example 1:

Array Contents->

1 2 3 4 5

1 2 3 4 5

1 2 3 4 5

1 2 3 4 5

1 2 3 4 5

The contents of the vector will be-> 0,1,0,3,1,1,1,3,2,1,2,3,3,1,3,3,4,1,4,3

Console Output:

0 1

0 3

1 1

1 3

2 1

2 3

3 1

3 3

4 1

4 3

Example 2:

Array Contents->

1 3 5 7 9

1 3 5 7 9

1 3 5 7 9

1 3 5 7 9

1 3 5 7 9

The contents of the vector will be-> -1

Console Output:

This array did not have any even numbers.


