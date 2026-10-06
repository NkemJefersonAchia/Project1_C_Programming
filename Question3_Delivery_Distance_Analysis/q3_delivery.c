#include <stdio.h>

#define MAX_ROUTES 100

int total_distance(int d[], int n)
{
    int sum = 0;

    for (int i = 0; i < n; i++)
        sum += d[i];

    return sum;
}

/* reuses total_distance instead of looping again */
float average_distance(int d[], int n)
{
    if (n == 0)
        return 0;

    return (float)total_distance(d, n) / n;
}

int longest_route(int d[], int n)
{
    int longest = d[0];

    for (int i = 1; i < n; i++) {
        if (d[i] > longest)
            longest = d[i];
    }

    return longest;
}

int count_above(int d[], int n, int limit)
{
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (d[i] > limit)
            count++;
    }

    return count;
}

/* sum of the first n elements, done recursively */
int recursive_sum(int d[], int n)
{
    if (n == 0)                 /* base case: nothing left to add */
        return 0;

    return d[n - 1] + recursive_sum(d, n - 1);
}

int main(void)
{
    int distances[MAX_ROUTES];
    int n, limit;

    printf("Number of routes: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_ROUTES) {
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

    if (recursive_sum(distances, n) == total_distance(distances, n))
        printf("Check: recursive and loop totals match.\n");

    /* same function, different argument: routes above the average */
    int avg = (int)average_distance(distances, n);
    printf("\nRoutes above average (%d km): %d\n", avg, count_above(distances, n, avg));

    return 0;
}
