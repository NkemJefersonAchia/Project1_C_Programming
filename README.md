# Project 1 — C Programming, Control Flow, Functions & Embedded Systems

**Student:** Nkem Jeferson Achia
**Programme:** BSc Software Engineering, African Leadership University
**Course:** C Programming — Trimester 6
**Date:** October 2026

This repository holds the source code for the four programs written for
Project 1. Each question lives in its own folder with a short README describing
what that program does.

> **Where the answers are.** The written answers to all four questions — the
> sample runs, the real-world application of C, the syntax-versus-semantic
> error analysis, the compilation lifecycle, the function and recursion
> explanations, the circuit and block diagrams, and the simulation test cases —
> are in the project report, `Project1_Nkem_Jeferson_Achia.pdf`, submitted with
> this assignment. The READMEs in this repository describe the code only; they
> do not repeat the report.

## Repository structure

```
Project1_C_Programming/
├── README.md                                   <- you are here
│
├── Question1_Water_Quality_Monitor/
│   ├── q1_water_quality.c                      sensor monitoring program
│   └── README.md
│
├── Question2_Mobile_Money_System/
│   ├── q2_mobile_money.c                       transaction processing system
│   └── README.md
│
├── Question3_Delivery_Distance_Analysis/
│   ├── q3_delivery.c                           array analysis and recursion
│   └── README.md
│
└── Question4_Smart_Parking_System/
    ├── q4_parking.ino                          Arduino sketch for Tinkercad
    ├── images/                                 circuit and simulation screenshots
    └── README.md
```

| # | Question | Language | Marks |
|---|----------|----------|-------|
| 1 | [Water-quality sensor monitoring](Question1_Water_Quality_Monitor) | C | 3 |
| 2 | [Mobile-money transaction system](Question2_Mobile_Money_System) | C | 4 |
| 3 | [Delivery distance analysis](Question3_Delivery_Distance_Analysis) | C | 4 |
| 4 | [Arduino smart parking system](Question4_Smart_Parking_System) | Arduino C | 4 |

## Requirements

Questions 1 to 3 need a C compiler supporting C11 — either `gcc` or `clang`.
Question 4 needs a browser, since it runs in the Tinkercad Circuits simulator
rather than on a PC.

## Getting the code

```bash
git clone https://github.com/NkemJefersonAchia/Project1_C_Programming.git
cd Project1_C_Programming
```

## Running Questions 1 to 3

Each program is a single source file with no dependencies beyond the C standard
library, so each compiles in one command. Build it, then run it.

### Question 1 — water-quality monitor

```bash
gcc -Wall -Wextra -std=c11 -o q1 Question1_Water_Quality_Monitor/q1_water_quality.c -lm
./q1
```

It asks for two readings: a temperature in degrees Celsius, then a turbidity
value in NTU. Entering `29.5` and `18` reports a quality index of 86.50 and a
status of Good.

### Question 2 — mobile-money transaction system

```bash
gcc -Wall -Wextra -std=c11 -o q2 Question2_Mobile_Money_System/q2_mobile_money.c
./q2
```

It shows a menu and keeps running until you choose option 5. Enter a menu
number, then an amount when prompted. Try depositing `50000`, then withdrawing
`70000` to see a transaction rejected for insufficient funds.

### Question 3 — delivery distance analysis

```bash
gcc -Wall -Wextra -std=c11 -o q3 Question3_Delivery_Distance_Analysis/q3_delivery.c
./q3
```

It asks how many routes there are, then for each distance on its own prompt,
then for a distance limit. Entering `6` routes of `12 25 18 40 15 30` with a
limit of `20` reports a total of 140 km and an average of 23.33 km.

## Running them without typing

All three read from standard input, so a whole session can be piped in. This is
useful for re-checking a result quickly or for reproducing the sample runs in
the report.

```bash
printf '29.5\n18\n' | ./q1                          # temperature, turbidity
printf '1\n50000\n2\n70000\n3\n5\n' | ./q2          # deposit, withdraw, balance, exit
printf '6\n12\n25\n18\n40\n15\n30\n20\n' | ./q3     # count, each distance, limit
```

Question 3 prompts for each distance separately, so the piped version needs one
value per line rather than a single space-separated line.

## Running Question 4

Question 4 is an Arduino sketch and is not compiled with `gcc`. To run it:

1. Open [Tinkercad Circuits](https://www.tinkercad.com/circuits) and create a
   new circuit.
2. Build the circuit: an Arduino Uno, an HC-SR04 ultrasonic sensor, a green and
   a red LED each with a 220 Ω resistor, and a piezo buzzer. The full
   connection list and wiring diagram are in the report.
3. Open the code editor, switch it to **Text** mode, and paste in the contents
   of [`q4_parking.ino`](Question4_Smart_Parking_System/q4_parking.ino).
4. Click **Start Simulation**, then open the **Serial Monitor** to watch the
   measured distance.
5. Click the ultrasonic sensor and drag the object marker closer or further
   away. Inside 50 cm the red LED lights and the buzzer sounds; beyond it the
   green LED shows the bay is free.

## Build flags

The flags above are not decoration, so it is worth saying what each does:

- `-Wall -Wextra` turn on the two main warning sets. All three programs compile
  with **zero warnings** under them, which is the standard each was held to.
- `-std=c11` fixes the language standard, so the build does not depend on
  whichever default the installed compiler happens to use.
- `-lm` links the maths library. Question 1 calls `fabsf()` from `math.h`.
  Some toolchains compile that to a single CPU instruction and link without the
  flag, but others emit a real library call and fail at the linking stage
  without it, so it is included for portability. It is harmless where it is not
  needed.

Nothing in the three programs is platform-specific. They were built and tested
with Apple clang 17.0.0 on macOS and build the same way with gcc or clang on
Linux or WSL.
