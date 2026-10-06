#include <stdio.h>

#define MAX_ROUTES 100

int total_distance(int distances[], int n)
{
    int sum = 0;

    for (int i = 0; i < n; i++)
        sum += distances[i];

    return sum;
}

float average_distance(int distances[], int n)
{
    return (float)total_distance(distances, n) / n;
}

int longest_route(int distances[], int n)
{
    int longest = distances[0];

    for (int i = 1; i < n; i++)
        if (distances[i] > longest)
            longest = distances[i];

    return longest;
}

int count_above(int distances[], int n, int limit)
{
    int count = 0;

    for (int i = 0; i < n; i++)
        if (distances[i] > limit)
            count++;

    return count;
}

int recursive_sum(int distances[], int n)
{
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

    /* count_above() reused, this time with the average as the limit */
    int average = (int)average_distance(distances, n);
    printf("\nRoutes above average (%d km): %d\n", average,
           count_above(distances, n, average));

    return 0;
}
