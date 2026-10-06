# Question 3 — Functions & Recursive Problem Solving

**Points:** 4

A logistics company analyses the distances recorded for a group of delivery
routes. The program reads N route distances into an integer array and passes
that array to a set of small functions, each answering one question about the
routes.

## Deliverable 1 — Complete C source code

[`q3_delivery.c`](q3_delivery.c)

| Requirement | Where it's done |
|---|---|
| Stores the distances of N routes in an integer array | `int distances[MAX_ROUTES]` in `main()` |
| Function: calculate the total distance | `total_distance()` |
| Function: calculate the average distance | `average_distance()` |
| Function: find the longest route | `longest_route()` |
| Function: count routes above a specified limit | `count_above()` |
| Recursive function that sums the distances | `recursive_sum()` |
| Reuse — a function used inside another calculation | `average_distance()` calls `total_distance()` instead of re-looping |
| Reuse — a function called more than once with different arguments | `count_above()` is called with the user's limit, then again with the average |
| Displays the calculated results clearly | The report block in `main()` |
| Recursion: clear base case | `if (n == 0) return 0;` |
| Recursion: reduces the problem on every call | Each call passes `n - 1` |
| Recursion: returns the result to the caller | `return distances[n - 1] + recursive_sum(distances, n - 1);` |

### Build and run

```bash
gcc -Wall -Wextra -std=c11 -o q3 q3_delivery.c
./q3
```

Compiles with **zero warnings** under `-Wall -Wextra`.

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

Routes above average (23 km): 3
```

The first block reproduces the expected output in the brief line for line. The
last line is the function-reuse demonstration, kept below the required block so
the required format stays intact. Both the loop total and the recursive total
come to 140 km.

To reproduce the run without typing:

```bash
printf '6\n12 25 18 40 15 30\n20\n' | ./q3
```

### Extra test cases

| Input | Expected | Result |
|-------|----------|--------|
| `1` route of `42` km, limit `20` | Total 42, average 42.00, longest 42, 1 above limit, recursive sum 42 | Pass |
| `4` routes of `20 20 20 20`, limit `20` | 0 routes above limit — the check is strictly greater | Pass |
| `0` routes | Rejected with a message, exits with status 1 | Pass |
| `101` routes (above `MAX_ROUTES`) | Rejected with a message, exits with status 1 | Pass |

The `n < 1` guard matters because `longest_route()` reads `distances[0]`
immediately, and `average_distance()` divides by `n` — with an empty array both
would misbehave. The upper guard stops the input loop writing past the end of
the array.

## Deliverable 3 — How the program is divided into functions

| Function | Parameters | Returns | Job |
|----------|-----------|---------|-----|
| `total_distance` | array, n | `int` | Adds up every distance with a loop |
| `average_distance` | array, n | `float` | Calls `total_distance()` and divides by n |
| `longest_route` | array, n | `int` | Starts at the first route and keeps the largest value it sees |
| `count_above` | array, n, limit | `int` | Counts routes longer than whatever limit it is given |
| `recursive_sum` | array, n | `int` | The same total as `total_distance`, computed recursively |
| `main` | none | `int` | Reads input, calls the functions, prints the report |

Each function answers exactly one question, takes the array and its length as
parameters, and returns a value rather than printing anything — so any of them
could be dropped into another program unchanged.

Function reuse shows up twice:

1. `average_distance()` calls `total_distance()` instead of writing a second
   loop to add the same numbers again.
2. `count_above()` is called **twice with different arguments** — once with the
   agent's 20 km limit, and once with the computed average of 23 km, which
   tells the company how many routes are longer than typical.

## Deliverable 4 — How the recursive function works

```c
int recursive_sum(int distances[], int n)
{
    if (n == 0)
        return 0;

    return distances[n - 1] + recursive_sum(distances, n - 1);
}
```

It treats "the sum of n distances" as "the last distance plus the sum of the
first n − 1". Each call peels off one element and hands a smaller problem to
the next call.

- **Base case:** when `n == 0` there is nothing left to add, so it returns 0 and
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
of a sum. It is two lines, there is no loop counter or index to get wrong, and
it is easy to convince yourself it is correct just by reading it.

**Limitation.** Every call takes a new frame on the stack until the base case
is reached. For 6 routes that is nothing. But with tens of thousands of routes
it could overflow the stack and crash, and it is slower than a loop because of
the function-call overhead. On a microcontroller with a couple of kilobytes of
RAM that risk appears much sooner. For a simple sum, the loop in
`total_distance()` is the more practical choice.
