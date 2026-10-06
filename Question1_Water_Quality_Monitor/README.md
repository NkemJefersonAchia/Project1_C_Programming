# Question 1 — Water-Quality Sensor Monitoring

Source: [`q1_water_quality.c`](q1_water_quality.c)

This program models the processing stage of a water-quality monitoring device.
It reads a temperature reading in degrees Celsius and a turbidity reading in
NTU, then works out a quality index by subtracting two penalties from one
hundred: how far the temperature has strayed from the ideal twenty-five
degrees, and half the turbidity value. The resulting index is classified as
Good at eighty or above, Warning from sixty up to eighty, and Critical below
sixty, and the readings, index and status are printed as a formatted report.
The work is split across three functions besides `main()` — `read_value()`
collects one reading and rejects anything that is not a number,
`calculate_index()` applies the formula, and `classify()` turns the index into
its status label.

Build and run with `gcc -Wall -Wextra -std=c11 -o q1 q1_water_quality.c -lm`
then `./q1`.
