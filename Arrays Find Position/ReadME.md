
## CS 2411

Arrays – Find Position Exercise
Modify the following code so that it outputs to the console screen the position of where the lowest value in the array is located.

```
const int SIZE = 10;
int numbers[SIZE]={-5,-50,9,-2,-8,8,0,10,4,-35};
int count, lowest;
lowest = numbers[0];
for(count = 1; count < SIZE; count++)
{
if(numbers[count] < lowest)
lowest = numbers[count];
}
cout<< lowest<<endl;
```
Currently, this code outputs the following:
Modify the code so that it outputs the following:

