/*
 * Delivery distance analysis for a logistics company.
 *
 * Reads the distances of N delivery routes into an array, then uses a
 * separate function for each question about the routes: the total, the
 * average, the longest, and how many are above a given limit. The same
 * total is also computed recursively as a second approach.
 */

#include <stdio.h>

#define MAX_ROUTES 100

/* Adds up every distance using a loop. */
int total_distance(int distances[], int n)
{
    int sum = 0;

    for (int i = 0; i < n; i++)
        sum += distances[i];

    return sum;
}

/*
 * Works out the mean distance.
 *
 * Reuses total_distance() rather than writing a second loop to add the
 * same numbers again. The cast makes the division produce a decimal
 * result instead of truncating to a whole number.
 */
float average_distance(int distances[], int n)
{
    return (float)total_distance(distances, n) / n;
}

/* Finds the longest route: starts at the first one and keeps whatever
   is bigger as it walks through the rest. */
int longest_route(int distances[], int n)
{
    int longest = distances[0];

    for (int i = 1; i < n; i++)
        if (distances[i] > longest)
            longest = distances[i];

    return longest;
}

/* Counts how many routes are longer than the given limit. */
int count_above(int distances[], int n, int limit)
{
    int count = 0;

    for (int i = 0; i < n; i++)
        if (distances[i] > limit)
            count++;

    return count;
}

/*
 * Adds up the distances recursively instead of with a loop.
 *
 * The idea: the sum of n distances is the last distance plus the sum of
 * the first n - 1. Each call peels off one element and passes a smaller
 * problem down, so n is guaranteed to reach the base case.
 */
int recursive_sum(int distances[], int n)
{
    /* base case: nothing left to add, so stop recursing */
    if (n == 0)
        return 0;

    return distances[n - 1] + recursive_sum(distances, n - 1);
}

int main(void)
{
    int distances[MAX_ROUTES];
    int n, limit;

    printf("Number of routes: ");
    scanf("%d", &n);

    /* guard the array: longest_route() reads the first element and
       average_distance() divides by n, so neither works with 0 routes */
    if (n < 1 || n > MAX_ROUTES) {
        printf("Please enter between 1 and %d routes.\n", MAX_ROUTES);
        return 1;
    }

    printf("Distances: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &distances[i]);

    printf("Distance limit: ");
    scanf("%d", &limit);

    printf("\n===== DELIVERY DISTANCE ANALYSIS =====\n\n");
    printf("Total distance: %d km\n", total_distance(distances, n));
    printf("Average distance: %.2f km\n", average_distance(distances, n));
    printf("Longest route: %d km\n", longest_route(distances, n));
    printf("Routes above %d km: %d\n", limit, count_above(distances, n, limit));

    printf("\nRecursive sum: %d km\n", recursive_sum(distances, n));

    /* count_above() reused a second time, now with the average as the
       limit, which shows how many routes are longer than typical */
    int average = (int)average_distance(distances, n);
    printf("\nRoutes above average (%d km): %d\n", average,
           count_above(distances, n, average));

    return 0;
}
