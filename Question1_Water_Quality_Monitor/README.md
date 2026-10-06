# Question 1 — C Program Development and Compilation for a Sensor Monitoring System

**Points:** 3

A water-quality monitoring device reads a temperature sensor (°C) and a
turbidity sensor (NTU), calculates a quality index, and displays a formatted
report with the water-quality status.

```
Index = 100 - (TemperatureDeviation + TurbidityPenalty)

TemperatureDeviation = abs(Temperature - 25)
TurbidityPenalty     = Turbidity / 2
```

Classification: **Good** if Index >= 80, **Warning** if Index >= 60 and < 80,
**Critical** if Index < 60.

## Deliverable 1 — Complete C source code

[`q1_water_quality.c`](q1_water_quality.c) — 64 lines, three functions besides
`main()`.

| Requirement | Where it's done |
|---|---|
| Declares variables for temperature (°C) and turbidity (NTU) | `float temperature, turbidity;` in `main()` |
| Calculates the index with the given formula | `calculate_index()` |
| Classifies Good / Warning / Critical | `classify()` |
| Prints a formatted report with readings, index and status | The report block in `main()` |
| Uses at least one function other than `main()` | Three: `calculate_index()`, `classify()` and `read_value()` |

| Function | Returns | Job |
|----------|---------|-----|
| `calculate_index(temperature, turbidity)` | `float` | Applies the index formula to the two readings |
| `classify(index)` | `const char *` | Maps the index onto "Good", "Warning" or "Critical" |
| `read_value(prompt, value)` | `int` | Prompts for one reading and returns 0 if the input was not a number |
| `main()` | `int` | Collects both readings, calls the functions, prints the report |

`read_value()` means the program never calculates an index from a value that
was never successfully read — on bad input it reports the problem and exits
with status 1 instead.

### Build and run

```bash
gcc -Wall -Wextra -std=c11 -o q1 q1_water_quality.c -lm
./q1
```

`-lm` is needed because `fabsf()` comes from `math.h`. Compiles with **zero
warnings** under `-Wall -Wextra`.

## Deliverable 2 — Sample output from one test run

Readings of 29.5 °C and 18 NTU:

```
--------------------------------------------
        WATER QUALITY MONITOR
--------------------------------------------
Enter the latest sensor readings below.

 Temperature (C)  : 29.5
 Turbidity (NTU)  : 18


============================================
          WATER QUALITY REPORT
============================================
 Temperature  :    29.50 C
 Turbidity    :    18.00 NTU
 Quality index:    86.50
 Status       :     Good
============================================
```

Checking by hand: deviation = |29.5 − 25| = 4.5, penalty = 18 / 2 = 9, so the
index is 100 − 13.5 = 86.5. That is at or above 80, so **Good**.

### Confirming all three bands

The run above only reaches the Good band, so I ran the program again with other
readings, including exactly on both boundaries:

| Temperature | Turbidity | Deviation | Penalty | Index | Status | What it checks |
|---|---|---|---|---|---|---|
| 29.5 °C | 18 NTU | 4.5 | 9.0 | 86.50 | Good | the sample run above |
| 30 °C | 30 NTU | 5.0 | 15.0 | 80.00 | Good | lower edge of Good |
| 32 °C | 40 NTU | 7.0 | 20.0 | 73.00 | Warning | inside Warning |
| 35 °C | 60 NTU | 10.0 | 30.0 | 60.00 | Warning | lower edge of Warning |
| 40 °C | 70 NTU | 15.0 | 35.0 | 50.00 | Critical | below 60 |

The boundary rows matter most. The spec says Index **>= 80** is Good and
**>= 60** is Warning, so exactly 80.00 has to come out Good and exactly 60.00
has to come out Warning. Both do, because `classify()` uses `>=` and not `>`.

Typing something that isn't a number is rejected rather than being used as a
reading:

```
 Temperature (C)  : abc

[!] Invalid reading. Please enter a number.
```

## Deliverable 3 — Short technical explanation

### (a) Real-world application

A good example is firmware for microcontroller-based devices — the
water-quality monitor in this question, a motorcycle crash detector, or a car's
engine control unit. These run on chips with a few kilobytes of RAM and no
operating system.

C suits this work for four reasons. It compiles to small, fast machine code,
which matters when the whole program has to fit in flash measured in kilobytes.
It gives direct access to memory and hardware registers through pointers, so
you can set a GPIO pin or read an ADC with nothing in between. It has no
garbage collector, so nothing pauses the program unpredictably and timing stays
deterministic — essential when a sensor must be sampled on a fixed schedule.
And practically every microcontroller vendor ships a C compiler for their chip,
which makes it the default language for embedded work.

### (b) Error analysis

**Syntax error — a missing semicolon** after the declaration
`float temperature, turbidity;`. Real compiler output:

```
$ gcc -Wall -Wextra -std=c11 syn.c -o q1 -lm
syn.c:37:33: error: expected ';' at end of declaration
   37 |     float temperature, turbidity
      |                                 ^
      |                                 ;
1 error generated.
```

This is a **syntax error** because the code breaks C's grammar rules. A
declaration must end in a semicolon, so the compiler cannot parse the statement
at all. It never gets as far as working out what the program means — it reports
the error, produces no object file, and the build stops.

**Semantic error — `>` where `>=` was meant**, writing `if (index > 80)`
instead of `if (index >= 80)` in `classify()`. The compiler accepts it without
complaint:

```
$ gcc -Wall -Wextra -std=c11 sem.c -o q1 -lm
$                                  # no warnings, no errors
```

But the program now answers wrongly at the boundary. With 30 °C and 30 NTU the
index is exactly 80.00:

| Version | Index | Status reported | Correct? |
|---|---|---|---|
| `index >= 80` | 80.00 | Good | yes |
| `index > 80` | 80.00 | Warning | no |

This is a **semantic error** because the code is valid C — correct grammar,
correct types, zero warnings even under `-Wall -Wextra`. What is wrong is the
*meaning*: the program runs happily but misclassifies water sitting exactly on
the threshold. That is what makes semantic errors harder to deal with than
syntax errors. The compiler cannot help, because it has no way of knowing which
comparison was intended. Only testing the boundary, or reading the spec against
the code, exposes it.

### (c) Compilation lifecycle

Turning `q1_water_quality.c` into an executable takes four stages. I ran each
one separately to see the file it produces:

| Stage | Command | Input | Output | What happens |
|-------|---------|-------|--------|--------------|
| 1. Preprocessing | `gcc -E` | `q1_water_quality.c` | `q1.i` — expanded C source | Acts on every `#` line. Pastes in the whole text of `stdio.h` and `math.h` and strips comments. Still C, just much larger. |
| 2. Compilation | `gcc -S` | `q1.i` | `q1.s` — assembly | Checks syntax and types, then translates the C into assembly for the target CPU. This stage rejects the missing semicolon from part (b). |
| 3. Assembly | `gcc -c` | `q1.s` | `q1.o` — object file | The assembler encodes the assembly into binary machine code. Real code now, but not runnable: library calls are left as named placeholders. |
| 4. Linking | `gcc q1.o -lm` | `q1.o` + C standard library + math library | `q1` — executable | The linker resolves those placeholders against the libraries and adds the startup code that runs before `main()`, producing one file the OS can load. |

Measured on each output:

```
stage 0  source        :   64 lines
stage 1  q1.i   (-E)   : 1086 lines      <- headers pasted in
stage 2  q1.s   (-S)   :  273 lines of assembly
stage 3  q1.o   (-c)   : 2720 bytes object file
stage 4  q1     (link) : 8624 bytes executable
```

The placeholder point in stage 3 is easy to confirm — `nm -u` lists what an
object file still needs from elsewhere:

```
$ nm -u q1.o
_printf
_scanf
```

Both are standard-library functions the program calls but does not define, and
they stay undefined until stage 4 links them in. (`fabsf` is absent because the
compiler turns it into a single floating-point instruction rather than a call.)

These figures come from Apple clang 17.0.0 on x86_64 macOS. Line counts and
file sizes differ between platforms, since each system ships its own headers —
but the four stages and their inputs and outputs are the same everywhere.

For an Arduino the pipeline is identical, except the compiler targets the AVR
chip and the last step produces a `.hex` file to flash onto the board instead
of an executable the OS runs.
