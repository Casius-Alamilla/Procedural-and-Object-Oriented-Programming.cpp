
Write a complete C++ program that meets the following requirements:

1. Declare a struct named stock.
	1. It must contain the following members:
		1. Name (string)
		2. Price on Day 1 (float)
		3. Price on Day 2 (float)
		4. Percent change (float)
	  
2. Ask the user to input how many stocks they will enter.
3. Input that number.
4. Declare a vector of stock objects using the number entered in step 
5. Use a loop to input stock data and compute percent change:
	1. Input:
	2. Name of stock
	3. Price on Day 1
	4. Price on Day 2
	5. Compute the percent change.
	   
6. Determine which stock has the greatest positive percent change.
7. Output the stock from the vector whose percent change increased the most.
8. Output the following for that stock:
	1. Name
	2. Price on Day 1
	3. Price on Day 2
	4. Percent change

---

Sample Run:

1. Enter the number of stocks: 3
	1. Enter data for stock 1:
	2. Name: CompanyA
	3. Price on Day 1: 23.67
	4. Price on Day 2: 25.78

2. Enter data for stock 2:
	1. Name: CompanyB
	2. Price on Day 1: 2.67
	3. Price on Day 2: 10.8

3. Enter data for stock 3:
	1. Name: CompanyC
	2. Price on Day 1: 50.8
	3. Price on Day 2: 120.88

4. Stock with greatest positive percent change:
	1. Name: CompanyB
	2. Price on Day 1: 2.67
	3. Price on Day 2: 10.8
	4. Percent change: 304.494%
