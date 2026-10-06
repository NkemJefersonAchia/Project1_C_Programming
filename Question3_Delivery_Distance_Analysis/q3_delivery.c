/* Delivery distance analysis for a logistics company. */

#include <stdio.h>

#define MAX_ROUTES 100

/* Adds every route distance. */
int total_distance(int distances[], int n)
{
    int sum = 0;

    for (int i = 0; i < n; i++)
        sum += distances[i];

    return sum;
}

/* Calculates the average route distance. */
float average_distance(int distances[], int n)
{
    return (float)total_distance(distances, n) / n;
}

/* Finds the longest route. */
int longest_route(int distances[], int n)
{
    int longest = distances[0];

    for (int i = 1; i < n; i++)
        if (distances[i] > longest)
            longest = distances[i];

    return longest;
}

/* Counts routes above a distance limit. */
int count_above(int distances[], int n, int limit)
{
    int count = 0;

    for (int i = 0; i < n; i++)
        if (distances[i] > limit)
            count++;

    return count;
}

/* Adds route distances recursively. */
int recursive_sum(int distances[], int n)
{
    /* Stop when every route has been included. */
    if (n == 0)
        return 0;

    return distances[n - 1] + recursive_sum(distances, n - 1);
}

int main(void)
{
    int distances[MAX_ROUTES];
    int n, limit;

    printf("\n--------------------------------------------\n");
    printf("        DELIVERY DISTANCE ANALYSIS\n");
    printf("--------------------------------------------\n");
    printf("Enter the route information below.\n\n");

    /* Read the number of routes before filling the array. */
    printf(" Number of routes [1-%d]: ", MAX_ROUTES);
    scanf("%d", &n);

    if (n < 1 || n > MAX_ROUTES) {
        printf("\n[!] Enter between 1 and %d routes.\n", MAX_ROUTES);
        return 1;
    }

    printf("\n Enter the distance for each route in km.\n");
    for (int i = 0; i < n; i++) {
        printf(" Route %d distance (km): ", i + 1);
        scanf("%d", &distances[i]);
    }

    printf("\n Distance limit (km): ");
    scanf("%d", &limit);

    /* Prepare the values used in the report. */
    int total = total_distance(distances, n);
    float average = average_distance(distances, n);
    int average_limit = (int)average;

    printf("\n\n============================================\n");
    printf("          DELIVERY ANALYSIS REPORT\n");
    printf("============================================\n");
    printf(" Total distance       : %8d km\n", total);
    printf(" Average distance     : %8.2f km\n", average);
    printf(" Longest route        : %8d km\n", longest_route(distances, n));
    printf(" Recursive total      : %8d km\n", recursive_sum(distances, n));
        printf(" Above limit count    : %8d (over %d km)\n",
            count_above(distances, n, limit), limit);
        printf(" Above average count  : %8d (over %d km)\n",
                count_above(distances, n, average_limit), average_limit);
    printf("============================================\n");

    return 0;
}
