/* Water quality monitoring system. */
#include <stdio.h>
#include <math.h>

/* Calculates the quality index from both sensor readings. */
float calculate_index(float temperature, float turbidity)
{
    return 100.0f - (fabsf(temperature - 25.0f) + turbidity / 2.0f);
}

/* Converts the index into a readable status. */
const char *classify(float index)
{
    if (index >= 80)
        return "Good";
    if (index >= 60)
        return "Warning";

    return "Critical";
}

/* Reads one sensor value and reports invalid input. */
int read_value(const char *prompt, float *value)
{
    printf("%s", prompt);

    if (scanf("%f", value) != 1) {
        printf("\n[!] Invalid reading. Please enter a number.\n");
        return 0;
    }

    return 1;
}

int main(void)
{
    float temperature, turbidity;

    printf("\n--------------------------------------------\n");
    printf("        WATER QUALITY MONITOR\n");
    printf("--------------------------------------------\n");
    printf("Enter the latest sensor readings below.\n\n");

    /* Collect the two sensor readings. */
    if (!read_value(" Temperature (C)  : ", &temperature))
        return 1;

    if (!read_value(" Turbidity (NTU)  : ", &turbidity))
        return 1;

    float index = calculate_index(temperature, turbidity);

    /* Display the completed monitoring report. */
    printf("\n\n============================================\n");
    printf("          WATER QUALITY REPORT\n");
    printf("============================================\n");
    printf(" Temperature  : %8.2f C\n", temperature);
    printf(" Turbidity    : %8.2f NTU\n", turbidity);
    printf(" Quality index: %8.2f\n", index);
    printf(" Status       : %8s\n", classify(index));
    printf("============================================\n");

    return 0;
}
