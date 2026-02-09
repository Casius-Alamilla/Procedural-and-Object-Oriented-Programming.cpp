# C++ Program Requirements (Rock–Paper–Scissors)

Write a C++ program that completes the following steps:

---

## 0. Declare Variables

Declare the following variables:

- `P1_int_val`, `P2_int_val` as type **int**
- `P1_string_val`, `P2_string_val` as type **string**

---

## 1. Generate Random Values

Use `srand()`, `time(NULL)`, and `rand()` to assign a random integer between **[0, 2]** to:

- `P1_int_val`
- `P2_int_val`

---

## 2. Map Integers to Strings Using if / else if / else

Based on the numerical values stored in `P1_int_val` and `P2_int_val`, use `if`, `else if`, and `else` statements to store the following strings in `P1_string_val` and `P2_string_val`:

| int_val | string_val |
|--------:|------------|
| 0       | rock       |
| 1       | paper      |
| 2       | scissors   |

---

## 3. Output Both Players’ Choices

Output the `string_val` of both players to the console.

---

## 4. Determine Winner Using a Function

Use a **function** to compute and return an **integer** to determine who won.

- Try to complete this step using **less than 6 operations**.

### Function Requirements

The function will take **two integers** as parameters:

1. `P1` — stores a copy of `P1_int_val`
2. `P2` — stores a copy of `P2_int_val`

The function will return an integer value to indicate a tie or which player won:

- `0` — Tie  
- `1` — Player 1  
- `2` — Player 2  

---

## 5. Output the Result

Based on what the function returned in Step 4, output either:

- `"Tie"`, or
- a message stating who won (Player 1 or Player 2)

---
