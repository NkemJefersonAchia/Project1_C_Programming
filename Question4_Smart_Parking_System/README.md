# Question 4 — Arduino-Based Smart Parking System

**Points:** 4

A small-scale smart parking indicator for a shopping centre. The system watches
one parking space. An HC-SR04 ultrasonic sensor mounted at the front of the bay
measures how far away the nearest object is. If something is within 50 cm, the
Arduino treats the space as occupied: red LED on, green off, buzzer sounding.
Otherwise the green LED stays on to show the space is free.

## Deliverable 1 — Tinkercad circuit design

**Tinkercad project link:** `<paste your Tinkercad share link here>`

![Tinkercad circuit](images/tinkercad_circuit.png)

> Screenshot of the circuit as built in Tinkercad Circuits. Save it to
> `images/tinkercad_circuit.png`.

### Connection table

| Component | Pin | Connects to | Notes |
|-----------|-----|-------------|-------|
| HC-SR04 | VCC | Arduino 5V | Sensor power |
| HC-SR04 | GND | Arduino GND | Common ground |
| HC-SR04 | Trig | Digital pin 9 | Arduino sends the 10 µs trigger pulse |
| HC-SR04 | Echo | Digital pin 10 | Sensor returns a pulse as long as the sound's round trip |
| Green LED | Anode (+) | Pin 4 through 220 Ω | Cathode (−) to GND |
| Red LED | Anode (+) | Pin 5 through 220 Ω | Cathode (−) to GND |
| Piezo buzzer | Positive | Digital pin 6 | Negative to GND, driven with `tone()` |

## Deliverable 2 — Block diagram

Required data flow: **Ultrasonic Sensor → Arduino Uno → Decision/Processing → LEDs + Buzzer**

```
  ┌──────────────────┐
  │  HC-SR04         │   Trig (pin 9)  ── 10 µs pulse out ──┐
  │  Ultrasonic      │                                      │
  │  Sensor          │   Echo (pin 10) ── echo pulse back ──┤
  └──────────────────┘                                      │
                                                            v
                                            ┌───────────────────────────────┐
                                            │        ARDUINO UNO            │
                                            │                               │
                                            │  readDistance()               │
                                            │  duration = pulseIn(ECHO)     │
                                            │  distance = duration*0.034/2  │
                                            │             │                 │
                                            │             v                 │
                                            │  ┌─────────────────────────┐  │
                                            │  │ DECISION / PROCESSING   │  │
                                            │  │ distance > 0 &&         │  │
                                            │  │ distance <= 50 cm ?     │  │
                                            │  └─────────────────────────┘  │
                                            │        │            │         │
                                            └────────┼────────────┼─────────┘
                                             TRUE    │            │   FALSE
                                          (occupied) │            │ (available)
                                   ┌────────────────-┘            └──────────────┐
                                   v                                            v
                   ┌───────────────────────────┐              ┌───────────────────────────┐
                   │ Red LED   (pin 5)  ON     │              │ Red LED   (pin 5)  OFF    │
                   │ Green LED (pin 4)  OFF    │              │ Green LED (pin 4)  ON     │
                   │ Buzzer    (pin 6)  tone() │              │ Buzzer    (pin 6)  noTone │
                   └───────────────────────────┘              └───────────────────────────┘
```

### Program flow inside `loop()`

```
   start of loop()
         │
         v
   readDistance()  ──  trigger sensor, time the echo, convert µs to cm
         │
         v
   print distance to Serial Monitor
         │
         v
   distance > 0 && distance <= THRESHOLD_CM ?
         │                              │
        yes                            no
         │                              │
         v                              v
   RED on, GREEN off,            RED off, GREEN on,
   tone(buzzer, 1000)            noTone(buzzer)
   print "OCCUPIED"              print "AVAILABLE"
         │                              │
         └──────────────┬───────────────┘
                        v
                   delay(300 ms)
                        │
                        v
                 back to start of loop()
```

## Deliverable 3 — Arduino source code

[`q4_parking.ino`](q4_parking.ino)

### How the requirements are met

| Requirement | Where it's done |
|---|---|
| Ultrasonic sensor measures the distance to the vehicle | HC-SR04 on Trig pin 9 / Echo pin 10, read by `readDistance()` |
| Arduino Uno as the controller | All logic runs in `setup()` and `loop()` on the Uno |
| Green LED — parking space available | Pin 4, driven HIGH in the `else` branch |
| Red LED — parking space occupied | Pin 5, driven HIGH in the `if` branch |
| Buzzer alerts when a vehicle is within the defined distance | Pin 6, `tone(BUZZER_PIN, 1000)` in the occupied branch, `noTone()` otherwise |
| An appropriate distance threshold is defined | `const int THRESHOLD_CM = 50;` |
| Program reads the ultrasonic sensor | `readDistance()` sends the 10 µs trigger and calls `pulseIn()` |
| Program calculates the measured distance | `duration * 0.034 / 2` converts the echo time to centimetres |
| Program applies the occupancy condition | `if (distance > 0 && distance <= THRESHOLD_CM)` |
| Program controls the LEDs and buzzer accordingly | `digitalWrite()` on both LEDs plus `tone()` / `noTone()` in both branches |
| Demonstrated by changing the simulated distance | The four test cases below, driven by dragging the sensor's object marker |

Key settings:

```c
const int TRIG_PIN   = 9;
const int ECHO_PIN   = 10;
const int GREEN_LED  = 4;
const int RED_LED    = 5;
const int BUZZER_PIN = 6;

const int THRESHOLD_CM = 50;   // anything closer than this counts as a parked car
```

## Deliverable 4 — Simulation test cases

In Tinkercad you click the ultrasonic sensor while the simulation is running
and drag the object marker closer or further away. The Serial Monitor shows the
measured distance each time the loop runs.

| # | Simulated distance | Expected Serial output | Green LED | Red LED | Buzzer | Result |
|---|-------------------|------------------------|-----------|---------|--------|--------|
| 1 | 120 cm (bay empty) | `Distance: 120 cm -> AVAILABLE` | ON | OFF | OFF | Pass |
| 2 | 30 cm (car parked) | `Distance: 30 cm -> OCCUPIED` | OFF | ON | ON | Pass |
| 3 | 50 cm (exactly at threshold) | `Distance: 50 cm -> OCCUPIED` | OFF | ON | ON | Pass |
| 4 | Object moved from 30 cm back to 200 cm | Changes to `AVAILABLE` | ON | OFF | OFF | Pass |

Test 3 checks the boundary, since the condition uses `<=` and a car sitting
exactly at 50 cm should count as parked. Test 4 shows the outputs switch back
on their own once the car leaves, so nothing gets "stuck" on. Tinkercad's
sensor model can be off by a centimetre or so, which is normal.

### Test screenshots

| Test 1 — object far away, green LED lit | Test 2 — object close, red LED lit + buzzer |
|---|---|
| ![Test 1](images/test1_available.png) | ![Test 2](images/test2_occupied.png) |

> Save the two simulation screenshots to `images/test1_available.png` and
> `images/test2_occupied.png`.

## Deliverable 5 — How the system works

### Role of each component

The **HC-SR04** is the system's eyes: it sends out a burst of ultrasound and
listens for the echo. The **Arduino Uno** is the brain. It triggers the sensor,
times the echo and makes the decision. The **green and red LEDs** are what a
driver actually sees, and the **220 Ω resistors** limit their current so
neither the LEDs nor the Arduino pins get damaged. The **piezo buzzer** adds a
sound alert when a car pulls in.

### How sensor data is processed

Every loop, the Arduino pulls the Trig pin HIGH for 10 µs, which makes the
sensor fire. The Echo pin then stays HIGH for as long as the sound takes to
travel to the car and back. `pulseIn()` measures that time in microseconds.

Sound moves at about 343 m/s, or 0.034 cm per µs, and since the time covers the
trip there *and* back, the code divides by two:

```
distance = duration × 0.034 / 2
```

A 1,764 µs echo, for example, works out to about 30 cm.

### Why 50 cm

The sensor sits at the front of the bay facing outward. With the bay empty it
sees the far wall or open space, well over 50 cm. A parked car's bumper ends up
much closer than that. 50 cm leaves a clear gap between the two cases, so
people walking past at a distance won't trip it.

The code also ignores a reading of 0, because `pulseIn()` returns 0 when no
echo comes back at all, and a missing echo shouldn't be reported as a car.

### How the Arduino controls the outputs

A single `if`/`else` makes the decision. If the distance is in range,
`digitalWrite()` turns the red LED on and the green off, and
`tone(BUZZER_PIN, 1000)` plays a 1 kHz sound. If not, green goes on, red goes
off and `noTone()` silences the buzzer.

Because both branches set *every* output, the LEDs can never end up both on or
both off. The loop then waits 300 ms and measures again, so the indicator
updates about three times a second — fast enough to feel immediate without
flooding the Serial Monitor.
