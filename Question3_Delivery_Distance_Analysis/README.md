# Question 3 — Delivery Distance Analysis

Source: [`q3_delivery.c`](q3_delivery.c)

This program analyses the distances recorded for a set of delivery routes. It
reads the number of routes, stores each distance in an integer array, and asks
for a distance limit, then reports the total distance covered, the average
route length, the longest single route, how many routes exceed the limit, and
the same total calculated again by recursion. Each question about the data is
answered by its own function that takes the array and its length as parameters
and returns a value rather than printing anything, which makes the functions
reusable: `average_distance()` calls `total_distance()` instead of looping over
the array a second time, and `count_above()` is called twice, once with the
limit the user supplied and once with the computed average. The recursive
`recursive_sum()` treats the sum of n distances as the last distance plus the
sum of the first n − 1, stopping at a base case of zero routes, and its result
serves as an independent check on the loop-based total.

Build and run with `gcc -Wall -Wextra -std=c11 -o q3 q3_delivery.c` then
`./q3`.
