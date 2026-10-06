# Question 4 — Arduino Smart Parking System

Source: [`q4_parking.ino`](q4_parking.ino)

This Arduino sketch drives a parking indicator for a single bay, built and
simulated in Tinkercad Circuits. An HC-SR04 ultrasonic sensor mounted at the
front of the bay is triggered by a ten-microsecond pulse on pin 9, and the
echo returning on pin 10 is timed and converted into a distance in centimetres
by multiplying the duration by 0.034 and halving it, since the sound travels
out and back. If the measured distance falls within fifty centimetres the bay
is treated as occupied, so the red LED on pin 5 lights and the buzzer on pin 6
sounds; otherwise the green LED on pin 4 shows the space is free. A reading of
zero is ignored, because that is what the sensor reports when no echo returns
at all and it would otherwise be mistaken for a very close object. Both
branches set all three outputs, so the indicator can never be left in a
contradictory state, and the measurement repeats roughly three times a second.

The circuit screenshots belong in the [`images/`](images) folder.
