/*
 * Smart parking indicator for a single parking space.
 *
 * An HC-SR04 ultrasonic sensor at the front of the bay measures how far
 * away the nearest object is. If something is closer than the threshold
 * the space counts as occupied: red LED on and the buzzer sounds.
 * Otherwise the green LED shows the space is free.
 *
 * Data flow:  sensor -> Arduino -> decision -> LEDs + buzzer
 */

const int TRIG_PIN = 9;     // sends the pulse that fires the sensor
const int ECHO_PIN = 10;    // goes HIGH for the length of the echo
const int GREEN_LED = 4;    // space available
const int RED_LED = 5;      // space occupied
const int BUZZER_PIN = 6;

// anything closer than this counts as a parked car
const int THRESHOLD_CM = 50;

void setup()
{
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
}

/*
 * Measures the distance to the nearest object, in centimetres.
 *
 * The sensor fires on a 10 microsecond trigger pulse, then holds the
 * echo pin HIGH for however long the sound takes to travel out and
 * back. pulseIn() times that in microseconds.
 */
int readDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  // sound covers 0.034 cm per microsecond, halved for the return trip
  return duration * 0.034 / 2;
}

void loop()
{
  int distance = readDistance();

  Serial.print("Distance: ");
  Serial.print(distance);

  /*
   * A reading of 0 means no echo came back at all, so it is ignored
   * rather than treated as a car sitting right against the sensor.
   */
  if (distance > 0 && distance <= THRESHOLD_CM) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    tone(BUZZER_PIN, 1000);
    Serial.println(" cm -> OCCUPIED");
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    noTone(BUZZER_PIN);
    Serial.println(" cm -> AVAILABLE");
  }

  // measure again about three times a second
  delay(300);
}
