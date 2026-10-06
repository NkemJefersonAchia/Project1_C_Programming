/*
 * q1_water_quality.c
 * -----------------------------------------------------------
 * Water-quality monitoring device firmware logic.
 *
 * The device reads a temperature sensor (degrees Celsius) and a
 * turbidity sensor (NTU), computes a simplified water-quality
 * index, classifies the result, and prints a monitoring report.
 *
 *   Index = 100 - (TemperatureDeviation + TurbidityPenalty)
 *   TemperatureDeviation = abs(Temperature - 25)
 *   TurbidityPenalty     = Turbidity / 2
 *
 * Build: gcc -Wall -Wextra -std=c11 -o q1 q1_water_quality.c -lm
 * Run:   ./q1
 * -----------------------------------------------------------
 */

#include <stdio.h>
#include <math.h>

#define IDEAL_TEMPERATURE   25.0f   /* reference temperature in degrees C */
#define GOOD_THRESHOLD      80.0f   /* index at or above this is "Good"    */
#define WARNING_THRESHOLD   60.0f   /* index at or above this is "Warning" */

/* Computes the simplified water-quality index from the two readings. */
float calculateIndex(float temperature, float turbidity)
{
    float temperatureDeviation = fabsf(temperature - IDEAL_TEMPERATURE);
    float turbidityPenalty     = turbidity / 2.0f;

    return 100.0f - (temperatureDeviation + turbidityPenalty);
}

/* Maps a water-quality index onto its status label. */
const char *classifyWater(float index)
{
    if (index >= GOOD_THRESHOLD)
        return "Good";
    else if (index >= WARNING_THRESHOLD)
        return "Warning";
    else
        return "Critical";
}

/* Prints the formatted monitoring report sent to the operator. */
void printReport(float temperature, float turbidity, float index, const char *status)
{
    printf("\n===== WATER QUALITY MONITORING REPORT =====\n\n");
    printf("Temperature reading : %6.2f C\n", temperature);
    printf("Turbidity reading   : %6.2f NTU\n", turbidity);
    printf("-------------------------------------------\n");
    printf("Water quality index : %6.2f\n", index);
    printf("Water quality status: %s\n", status);
    printf("===========================================\n\n");
}

int main(void)
{
    float temperature;   /* sensor reading in degrees Celsius */
    float turbidity;     /* sensor reading in NTU             */
    float index;         /* calculated water-quality index    */

    printf("Enter temperature reading (C)  : ");
    if (scanf("%f", &temperature) != 1) {
        printf("Invalid temperature reading. Aborting.\n");
        return 1;
    }

    printf("Enter turbidity reading (NTU)  : ");
    if (scanf("%f", &turbidity) != 1) {
        printf("Invalid turbidity reading. Aborting.\n");
        return 1;
    }

    index = calculateIndex(temperature, turbidity);
    printReport(temperature, turbidity, index, classifyWater(index));

    return 0;
}
