# Question 1 — C Program Development and Compilation for a Sensor Monitoring System

**Points:** 3

A water-quality monitoring device processes sensor readings before transmitting
them to a monitoring system. The device receives a temperature reading and a
turbidity reading, calculates a water-quality status, and displays a formatted
report.

```
Index = 100 - (TemperatureDeviation + TurbidityPenalty)

TemperatureDeviation = abs(Temperature - 25)
TurbidityPenalty     = Turbidity / 2
```

Classification: **Good** if Index >= 80, **Warning** if Index >= 60 and < 80,
**Critical** if Index < 60.

## Deliverable 1 — Complete C source code

[`q1_water_quality.c`](q1_water_quality.c)

### How the requirements are met

| Requirement | Where it's done |
|---|---|
| Declares variables for temperature (°C) and turbidity (NTU) | `temperature` and `turbidity` in `main()`, both `float` |
| Calculates the index with the given formula | `calculateIndex()` |
| Classifies Good / Warning / Critical | `classifyWater()` |
| Prints a formatted report with readings, index and status | `printReport()` |
| Uses at least one function other than `main()` | Three: `calculateIndex()`, `classifyWater()`, `printReport()` |

The three thresholds and the 25 °C reference are `#define` constants rather than
numbers buried in the code, so the classification bands can be changed in one
place.

| Function | Returns | Job |
|----------|---------|-----|
| `calculateIndex(temperature, turbidity)` | `float` | Applies the index formula to the two readings |
| `classifyWater(index)` | `const char *` | Maps the index onto "Good", "Warning" or "Critical" |
| `printReport(...)` | `void` | Prints the formatted monitoring report |
| `main()` | `int` | Reads the two readings, calls the functions, returns an exit status |

### Build and run

```bash
gcc -Wall -Wextra -std=c11 -o q1 q1_water_quality.c -lm
./q1
```

`-lm` is required because `fabsf()` comes from `math.h`. The program compiles
with **zero warnings** under `-Wall -Wextra`.

## Deliverable 2 — Sample output from one test run

Readings of 29.5 °C and 18 NTU:

```
Enter temperature reading (C)  : 29.5
Enter turbidity reading (NTU)  : 18

===== WATER QUALITY MONITORING REPORT =====

Temperature reading :  29.50 C
Turbidity reading   :  18.00 NTU
-------------------------------------------
Water quality index :  86.50
Water quality status: Good
===========================================
```

Checking by hand: deviation = |29.5 − 25| = 4.5, penalty = 18 / 2 = 9, so the
index is 100 − 13.5 = 86.5. That is at or above 80, so **Good**. The program
agrees.

### Confirming all three classification bands

The run above only lands in the Good band, so I ran the program again with
other readings to confirm every branch of `classifyWater()` behaves correctly,
including exactly on the two boundaries:

| Temperature | Turbidity | Deviation | Penalty | Index | Status | What it checks |
|---|---|---|---|---|---|---|
| 29.5 °C | 18 NTU | 4.5 | 9.0 | 86.50 | Good | the sample run above |
| 30 °C | 30 NTU | 5.0 | 15.0 | 80.00 | Good | lower edge of Good |
| 32 °C | 40 NTU | 7.0 | 20.0 | 73.00 | Warning | inside Warning |
| 35 °C | 60 NTU | 10.0 | 30.0 | 60.00 | Warning | lower edge of Warning |
| 40 °C | 70 NTU | 15.0 | 35.0 | 50.00 | Critical | below 60 |

The boundary rows are the ones that matter. The spec says Index **>= 80** is
Good and **>= 60** is Warning, so an index of exactly 80.00 has to come out
Good and exactly 60.00 has to come out Warning. Both do, because
`classifyWater()` uses `>=` and not `>`.

Typing something that isn't a number is rejected rather than being treated as
a reading:

```
Enter temperature reading (C)  : abc
Invalid temperature reading. Aborting.
```

The program exits with status 1 in that case.

## Deliverable 3 — Short technical explanation

### (a) Real-world application

A good example is firmware for microcontroller-based devices — the
water-quality monitor in this question, a motorcycle crash detector, or a car's
engine control unit. These devices run on chips with a few kilobytes of RAM and
no operating system underneath them.

C suits this job for four reasons. It compiles straight to small, fast machine
code, which matters when the whole program has to fit in flash measured in
kilobytes. It gives direct access to memory and hardware registers through
pointers, so you can set a GPIO pin or read an ADC without any layer in
between. It has no garbage collector, so nothing pauses the program at an
unpredictable moment and timing stays deterministic — essential when a sensor
has to be sampled on a fixed schedule. And practically every microcontroller
vendor ships a C compiler for their chip, which makes it the default language
for embedded work.

### (b) Error analysis

**Syntax error — a missing semicolon.** Dropping the `;` after the declaration
`float turbidity;` in `main()`. This is the real compiler output:

```
$ gcc -Wall -Wextra -std=c11 syntax_bad.c -o q1 -lm
syntax_bad.c:61:20: error: expected ';' at end of declaration
   61 |     float turbidity      /* sensor reading in NTU */
      |                    ^
      |                    ;
1 error generated.
```

This is a **syntax error** because the code breaks C's grammar rules. A
declaration has to end in a semicolon, so the compiler cannot parse the
statement at all. It never gets as far as working out what the program means —
it reports the error, produces no object file, and the build stops. Nothing
runs.

**Semantic error — `>` where `>=` was meant.** Writing
`if (index > GOOD_THRESHOLD)` instead of `if (index >= GOOD_THRESHOLD)` inside
`classifyWater()`. The compiler is completely happy with it:

```
$ gcc -Wall -Wextra -std=c11 semantic_bad.c -o q1 -lm
$                                  # no warnings, no errors, binary produced
```

But the program now gives the wrong answer at the boundary. With a reading of
30 °C and 30 NTU the index is exactly 80.00:

| Version | Index | Status reported | Correct per the spec? |
|---|---|---|---|
| `index >= 80` | 80.00 | Good | yes |
| `index > 80` | 80.00 | Warning | no |

This is a **semantic error** because the code is perfectly valid C — the
grammar is fine, the types are fine, and it compiles with zero warnings even
under `-Wall -Wextra`. What's wrong is the *meaning*: the program runs happily
but misclassifies water that sits exactly on the threshold. That's what makes
semantic errors harder to deal with than syntax errors. The compiler cannot
help, because it has no way of knowing which comparison was intended. Only
testing the boundary case, or reading the spec carefully against the code,
exposes it.

A related semantic slip is using `abs(temperature - 25)` instead of
`fabsf(...)`, since the formula in the brief is written as "abs". `abs()` takes
an `int`, so a reading of 24.5 °C would truncate to `abs(0)` = 0 instead of
0.5, and the index would come out as 100.00 rather than 99.50. Worth noting
that clang does catch this particular one with `-Wabsolute-value`, so it is
only a *silent* semantic error on compilers that don't carry that warning — the
`>` versus `>=` bug above is the cleaner example, because no compiler can
detect it.

### (c) Compilation lifecycle

Turning `q1_water_quality.c` into an executable goes through four stages. I ran
each one on its own so I could see the file it produces:

| Stage | Command | Input | Output | What happens |
|-------|---------|-------|--------|--------------|
| 1. Preprocessing | `gcc -E` | `q1_water_quality.c` | `q1.i` — expanded C source | Acts on every line starting with `#`. It pastes in the whole text of `stdio.h` and `math.h`, substitutes the `#define` constants such as `IDEAL_TEMPERATURE` for their values, and strips the comments. Still C, just much larger. |
| 2. Compilation | `gcc -S` | `q1.i` | `q1.s` — assembly | Parses the source and checks syntax and types, then translates it into assembly for the target CPU. This is the stage that rejects the missing semicolon from part (b). |
| 3. Assembly | `gcc -c` | `q1.s` | `q1.o` — object file | The assembler encodes the assembly mnemonics into binary machine code. The code is real now but not yet runnable: calls to library functions are left as named placeholders. |
| 4. Linking | `gcc q1.o -lm` | `q1.o` + C standard library + math library | `q1` — executable | The linker resolves those placeholders against the libraries, adds the startup code that runs before `main()`, and writes a single file the operating system can load and execute. |

Running the stages and measuring each output:

```
stage 0  source         :   80 lines
stage 1  q1.i   (-E)    : 1085 lines      <- headers pasted in
stage 2  q1.s   (-S)    :  259 lines of assembly
stage 3  q1.o   (-c)    : 2616 bytes object file
stage 4  q1     (link)  : 8632 bytes executable
```

The unresolved-placeholder point in stage 3 is easy to confirm — `nm -u` lists
the symbols an object file still needs from somewhere else:

```
$ nm -u q1.o
_printf
_scanf
```

Both are standard-library functions the program calls but does not define, and
they stay undefined until stage 4 links them in. (`fabsf` doesn't appear in that
list because the compiler turns it into a single floating-point instruction
inline rather than a function call.)

These figures come from Apple clang 17.0.0 on x86_64 macOS. The line counts and
file sizes differ from one platform to another, because each system ships its
own standard headers and `stdio.h` is a very different size on macOS than on
Linux — but the four stages, and the inputs and outputs of each, are the same
everywhere.

For an Arduino the pipeline is identical, except the compiler targets the AVR
chip and the final step produces a `.hex` file that gets flashed onto the board
instead of an executable the OS runs.
