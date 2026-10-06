/*
 * Water-quality monitoring device.
 *
 * Reads a temperature and a turbidity sensor, works out a quality
 * index from them, and prints a report saying whether the water is
 * Good, Warning or Critical.
 */

#include <stdio.h>
#include <math.h>

/*
 * Works out the water-quality index from the two readings.
 *
 *   index = 100 - (temperature deviation + turbidity penalty)
 *
 * The deviation is how far the temperature is from the ideal 25 C,
 * in either direction, which is why fabsf() is used. The penalty is
 * half the turbidity reading.
 */
float calculate_index(float temperature, float turbidity)
{
    return 100.0f - (fabsf(temperature - 25.0f) + turbidity / 2.0f);
}

/* Turns an index into its status label. */
const char *classify(float index)
{
    if (index >= 80)
        return "Good";
    if (index >= 60)
        return "Warning";

    return "Critical";
}

int main(void)
{
    float temperature, turbidity;

    /* collect the two sensor readings */
    printf("Enter temperature (C): ");
    scanf("%f", &temperature);

    printf("Enter turbidity (NTU): ");
    scanf("%f", &turbidity);

    float index = calculate_index(temperature, turbidity);

    /* print the monitoring report */
    printf("\n===== WATER QUALITY MONITORING REPORT =====\n\n");
    printf("Temperature : %.2f C\n", temperature);
    printf("Turbidity   : %.2f NTU\n", turbidity);
    printf("Index       : %.2f\n", index);
    printf("Status      : %s\n", classify(index));

    return 0;
}
