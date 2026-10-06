// Smart parking indicator: HC-SR04 sensor, green/red LEDs and a buzzer

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int GREEN_LED = 4;
const int RED_LED = 5;
const int BUZZER_PIN = 6;

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

int readDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  // sound travels 0.034 cm/us, halved because the pulse goes there and back
  return duration * 0.034 / 2;
}

void loop()
{
  int distance = readDistance();

  Serial.print("Distance: ");
  Serial.print(distance);

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

  delay(300);
}
