# Question 3 — Delivery Distance Analysis

**Points:** 4

A logistics company wants to analyse the distances recorded for a group of
delivery routes. The program reads N route distances into an integer array and
passes that array to a set of small functions. Each function answers one
question about the routes.

## Deliverable 1 — Source code

[`q3_delivery.c`](q3_delivery.c)

### How the requirements are met

| Requirement | Where it's done |
|---|---|
| Stores the distances of N routes in an integer array | `int distances[MAX_ROUTES]` in `main()` |
| User-defined function: calculate the total distance | `total_distance()` |
| User-defined function: calculate the average distance | `average_distance()` |
| User-defined function: find the longest route | `longest_route()` |
| User-defined function: count routes above a specified limit | `count_above()` |
| Recursive function that sums the distances | `recursive_sum()` |
| Function reuse — a function used inside another calculation | `average_distance()` calls `total_distance()` instead of re-looping |
| Function reuse — a function called more than once with different arguments | `count_above()` is called with the user's limit, then again with the average |
| Displays the calculated results clearly | The report block in `main()` |
| Recursion: clear base case | `if (n == 0) return 0;` |
| Recursion: reduces the problem on every call | Each call passes `n - 1` |
| Recursion: returns the result to the caller | `return d[n - 1] + recursive_sum(d, n - 1);` |

### Build and run

```bash
gcc -Wall -Wextra q3_delivery.c -o q3
./q3
```

Compiles with zero warnings under `-Wall -Wextra`.

## Deliverable 2 — Sample input/output

```
Number of routes: 6
Distances: 12 25 18 40 15 30
Distance limit: 20

===== DELIVERY DISTANCE ANALYSIS =====

Total distance: 140 km
Average distance: 23.33 km
Longest route: 40 km
Routes above 20 km: 3

Recursive sum: 140 km
Check: recursive and loop totals match.

Routes above average (23 km): 3
```

The first block reproduces the expected output in the brief line for line —
total 140 km, average 23.33 km, longest 40 km, 3 routes above 20 km, and a
recursive sum of 140 km. The two lines after it are the extra function-reuse
demonstration the brief also asks for, kept below the required block so the
required format stays intact.

To reproduce the run without typing:

```bash
printf '6\n12 25 18 40 15 30\n20\n' | ./q3
```

### Extra test cases

| Input | Expected | Result |
|-------|----------|--------|
| `1` route of `42` km, limit `20` | Total 42, average 42.00, longest 42, 1 above limit, recursive sum 42 (recursion depth 1) | Pass |
| `4` routes of `20 20 20 20`, limit `20` | 0 routes above limit (the check is strictly greater) | Pass |
| `0` routes | Rejected with a message, exits with status 1 | Pass |
| `101` routes (above `MAX_ROUTES`) | Rejected with a message, exits with status 1 | Pass |

The `n <= 0` guard matters because `longest_route()` reads `d[0]` straight away
— with an empty array that would be reading memory that was never written. The
upper guard stops the input loop from writing past the end of the array.

## Deliverable 3 — How the program is divided into functions

| Function | Parameters | Returns | Job |
|----------|-----------|---------|-----|
| `total_distance` | array, n | `int` | Adds up every distance with a loop. |
| `average_distance` | array, n | `float` | Calls `total_distance` and divides by n. Also guards against n = 0. |
| `longest_route` | array, n | `int` | Starts with the first route and keeps the largest value it sees. |
| `count_above` | array, n, limit | `int` | Counts routes longer than whatever limit it's given. |
| `recursive_sum` | array, n | `int` | Same total as `total_distance`, but done recursively. |
| `main` | none | `int` | Reads input, calls the functions and prints the report. |

Function reuse shows up in three places:

1. `average_distance()` calls `total_distance()` instead of writing a second
   loop to add the same numbers up again.
2. `count_above()` is called **twice with different arguments** — once with the
   user's 20 km limit, and once with the computed average (23 km), which tells
   the company how many routes are longer than typical.
3. The recursive sum is compared against `total_distance()` at the end as a
   quick self-check.

None of the functions print anything, they just take parameters and return
values, so any of them could be dropped into another program as-is.

## Deliverable 4 — How the recursive function works

```c
int recursive_sum(int d[], int n)
{
    if (n == 0)                 /* base case: nothing left to add */
        return 0;

    return d[n - 1] + recursive_sum(d, n - 1);
}
```

`recursive_sum(d, n)` treats "the sum of n distances" as "the last distance
plus the sum of the first n − 1". Each call peels off one element and hands a
smaller problem to the next call.

- **Base case:** when `n == 0` there's nothing left to add, so it returns 0 and
  the calls stop going deeper.
- **Moving toward the base case:** every call passes `n - 1`, so n drops by one
  each time and is guaranteed to reach 0 after exactly n calls.
- **Returning the result:** once the base case returns 0, each waiting call
  adds its own element and passes the total back up to its caller.

With the sample data it unwinds like this:

```
recursive_sum(d,6) = 30 + recursive_sum(d,5)
                        = 15 + recursive_sum(d,4)
                             = 40 + recursive_sum(d,3)
                                  = 18 + recursive_sum(d,2)
                                       = 25 + recursive_sum(d,1)
                                            = 12 + recursive_sum(d,0)
                                                 = 0   <- base case
```

Adding back up: 0 + 12 = 12, + 25 = 37, + 18 = 55, + 40 = 95, + 15 = 110,
+ 30 = **140**.

## Deliverable 5 — Advantage and limitation of recursion here

**Advantage.** The code reads almost exactly like the mathematical definition
of a sum. It's two lines, there's no loop counter or index to get wrong, and
it's easy to convince yourself it's correct just by reading it.

**Limitation.** Every call takes up a new frame on the stack until the base
case is reached. For 6 routes that's nothing. But with tens of thousands of
routes it could overflow the stack and crash, and it's a bit slower than a loop
because of all the function-call overhead. On a microcontroller with only a
couple of kilobytes of RAM, that risk shows up much sooner. For a simple sum,
the loop in `total_distance()` is the more practical choice.
