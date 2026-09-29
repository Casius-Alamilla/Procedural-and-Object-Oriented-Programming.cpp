
Goal: Operate with parallel arrays.

Assignment: You are writing a program that analyzes student test scores. Assume the following parallel arrays have been declared and initialized with values:

`scores` is an array of double values. This array contains student test scores.
`names` is an array of string values. This array contains student names.
The arrays are parallel, so `scores[0`] contains the test score for the student whose name is stored in `names[0]`, and so on. The size of both arrays is stored in the constant `SIZE`, which has already been declared.

Write some code that does the following:

Calculate the average test score.
Print the names of all the students whose scores are above the average.
Note: Your code should print only the names of the students whose scores are above the average. Print each name on a separate line.



Assume Headers and main are initialized

```c++
double total = 0.0;

for (int i = 0; i < SIZE; i++) {
    total += scores[i];
}

double average = total / SIZE;

for (int i = 0; i < SIZE; i++) {
    if (scores[i] > average) {
        cout << names[i] << endl;
    }
}
```
