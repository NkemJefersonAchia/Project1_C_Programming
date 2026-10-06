// Smart parking indicator - one parking space
// HC-SR04 ultrasonic sensor, green/red LEDs and a piezo buzzer

const int TRIG_PIN   = 9;
const int ECHO_PIN   = 10;
const int GREEN_LED  = 4;
const int RED_LED    = 5;
const int BUZZER_PIN = 6;

// anything closer than this counts as a parked car
const int THRESHOLD_CM = 50;

long duration;
int distanceCm;

void setup()
{
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Smart parking system started");
}

int readDistance()
{
  // short clean LOW first, then a 10 us pulse to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // time for the echo to come back, in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  // sound travels ~0.034 cm/us, and the pulse goes there and back
  return duration * 0.034 / 2;
}

void loop()
{
  distanceCm = readDistance();

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm -> ");

  if (distanceCm > 0 && distanceCm <= THRESHOLD_CM) {
    // car in the space
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    tone(BUZZER_PIN, 1000);
    Serial.println("OCCUPIED");
  } else {
    // space is free
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    noTone(BUZZER_PIN);
    Serial.println("AVAILABLE");
  }

  delay(300);
}
