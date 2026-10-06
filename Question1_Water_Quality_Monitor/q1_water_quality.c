#include <stdio.h>
#include <math.h>

float calculate_index(float temperature, float turbidity)
{
    return 100.0f - (fabsf(temperature - 25.0f) + turbidity / 2.0f);
}

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

    printf("Enter temperature (C): ");
    scanf("%f", &temperature);

    printf("Enter turbidity (NTU): ");
    scanf("%f", &turbidity);

    float index = calculate_index(temperature, turbidity);

    printf("\n===== WATER QUALITY MONITORING REPORT =====\n\n");
    printf("Temperature : %.2f C\n", temperature);
    printf("Turbidity   : %.2f NTU\n", turbidity);
    printf("Index       : %.2f\n", index);
    printf("Status      : %s\n", classify(index));

    return 0;
}
