Goal: Learn how to traverse two-dimensional arrays.

Assignment: As a data analyst, you're working with a `5x7` array of type double named `tempData`. The array holds temperature data from `5` geographical zones over `7` days. Each row in the array represents a zone, and each column represents a day of the week.

Your task is to write code to find the highest temperature in the array and identify the specific zone (row) and day (column) where it occurred.

Make sure your code defines the following variables:

a `double` named `highestTemp`
an `int` named `zoneIndex`
an `int` named `dayIndex`
When your code has finished executing, `highestTemp` should contain the highest temperature in the array, `zoneIndex` should contain the subscript of the row holding the highest temperature, and `dayIndex` should contain the subscript of the column holding the highest temperature.

Note: Your code does not display any output. Make sure the variables specified above contain the required values.


```c++


    double highestTemp = tempData[0][0];
    int zoneIndex = 0;
    int dayIndex = 0;

    for (int row = 0; row < 5; row++){
        for (int col = 0; col < 7;col++){
            if (tempData[row][col] > highestTemp){
                highestTemp = tempData[row][col];
                zoneIndex = row;
                dayIndex = col;

            }
        }
    }

```
