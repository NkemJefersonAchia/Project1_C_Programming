# Project 1 — C Programming, Control Flow, Functions & Embedded Systems

**Student:** Nkem Jeferson Achia
**Programme:** BSc Software Engineering, African Leadership University
**Course:** C Programming (Trimester 6)
**Date:** October 2026

This repository holds the four programs I wrote for Project 1, along with the
sample runs and written explanations for each one. Every question lives in its
own folder with its own README, so each part can be read and built on its own.

## Contents

| # | Question | Folder | Points |
|---|----------|--------|--------|
| 1 | Sensor monitoring program and compilation lifecycle | [`Question1_Water_Quality_Monitor/`](Question1_Water_Quality_Monitor) | 3 |
| 2 | Mobile-money transaction processing (control flow) | [`Question2_Mobile_Money_System/`](Question2_Mobile_Money_System) | 4 |
| 3 | Delivery distance analysis (functions and recursion) | [`Question3_Delivery_Distance_Analysis/`](Question3_Delivery_Distance_Analysis) | 4 |
| 4 | Arduino smart parking system (Tinkercad) | [`Question4_Smart_Parking_System/`](Question4_Smart_Parking_System) | 4 |

## Repository layout

```
Project1_C_Programming/
├── README.md                                 <- you are here
├── Question1_Water_Quality_Monitor/
│   ├── q1_water_quality.c
│   └── README.md
├── Question2_Mobile_Money_System/
│   ├── q2_mobile_money.c
│   └── README.md
├── Question3_Delivery_Distance_Analysis/
│   ├── q3_delivery.c
│   └── README.md
└── Question4_Smart_Parking_System/
    ├── q4_parking.ino
    └── README.md
```

## Building and running everything

Questions 1 to 3 are plain C and build with `gcc`. Question 4 is an Arduino
sketch, so it runs in the Tinkercad simulator rather than on a PC.

```bash
# Question 1 - needs -lm because it uses fabsf() from math.h
gcc -Wall -Wextra -std=c11 Question1_Water_Quality_Monitor/q1_water_quality.c -o q1 -lm
./q1

# Question 2
gcc -Wall -Wextra -std=c11 Question2_Mobile_Money_System/q2_mobile_money.c -o q2
./q2

# Question 3
gcc -Wall -Wextra -std=c11 Question3_Delivery_Distance_Analysis/q3_delivery.c -o q3
./q3
```

All three programs read from standard input, so you can also feed them a
scripted run instead of typing:

```bash
printf '29.5\n18\n' | ./q1                        # temperature, turbidity
printf '1\n50000\n3\n5\n' | ./q2                 # deposit, check balance, exit
printf '6\n12\n25\n18\n40\n15\n30\n20\n' | ./q3   # N, one distance per line, limit
```

Question 3 asks for each distance on its own prompt, so the scripted version
needs one value per line rather than a single space-separated line.

## Toolchain

All three C programs were compiled with `gcc -Wall -Wextra` and produced **zero
warnings**. Nothing in the programs is platform-specific, so they build the same
way with gcc or clang on macOS, Linux or WSL.

## A note on the test runs

Beyond the one sample run each question asks for, I also ran every program
against its edge cases — both water-quality band boundaries (an index of
exactly 80 and exactly 60), a withdrawal equal to the whole balance, a
single-element array, and inputs that should be rejected outright.

The README in each question folder gives a short description of what that
program does. The full write-up for every question — sample runs, the technical
explanations, the circuit diagrams and the test cases — is in the project
document submitted alongside this repository.

The programs are deliberately short. Each function does one job, takes what it
needs as parameters and returns a value, and comments are only there where the
reason for a line isn't obvious from the line itself.
