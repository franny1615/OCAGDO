// references:
// https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode/
// https://projecthub.arduino.cc/IsaacgracePeriod/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1
// https://docs.arduino.cc/learn/microcontrollers/digital-pins/
// https://www.arduino.cc/reference/cs/language/functions/communication/serial/begin/
// https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalwrite/

const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 13;
const int doorPin = 7;

const float parkedLow = 0;
const float parkedLightLow = 100;
const float parkedLight = 500;
const float parkedHigh = 700.0;
const float openHigh = 1000.0;
const int gracePeriod = 100; // ms
const float getOutGracePeriod = 30000; // ms

bool isOpen = false;

float duration;

void setup() {
  pinMode(trigPin, OUTPUT); // tells board that on this pin we will be serving out current.
  pinMode(ledPin, OUTPUT);
  pinMode(echoPin, INPUT);  // tells board that on this pin to report minute changes in current, serves as a way to read data in.
  pinMode(doorPin, OUTPUT);
  Serial.begin(9600); // baud, its a frequency int bits per second
}

void loop() {
  digitalWrite(doorPin, LOW);

  digitalWrite(trigPin, LOW);   // send 0V to trigger pin, basically resets state
  delayMicroseconds(2);         // wait
  digitalWrite(trigPin, HIGH);  // send 5V to output pin, since it was neutral, this tells sensor, do something
  delayMicroseconds(10);        // wait for sound to travel and hit sensor again
  digitalWrite(trigPin, LOW);   // reset state

  duration = pulseIn(echoPin, HIGH); // at this point sensor has some current stored in it representing distance traveled, read it in, measured in microseconds

  if (duration > openHigh)
  {
    digitalWrite(ledPin, LOW);
    delay(gracePeriod);
  }

  if (duration <= parkedLight && duration >= parkedLightLow)
  {
    digitalWrite(ledPin, HIGH);
    delay(gracePeriod);
  }

  if (duration >= parkedLow && duration <= parkedHigh && isOpen)
  {
    triggerDoor(2, duration); // door closed
    isOpen = false;
  }

  if (duration > parkedHigh && duration <= openHigh && !isOpen)
  {
    triggerDoor(1, duration); // door open
    delay(getOutGracePeriod); // give myself time to make sure I left
    triggerDoor(2, duration); // door closed
    isOpen = true;
  }
}

void triggerDoor(int eventType, float duration)
{
  switch (eventType)
  {
    case 1: // door open
      Serial.print("door was opened:");
      Serial.print(duration);
      Serial.println();
      break;
    case 2: // door closed
      Serial.print("door was closed:");
      Serial.print(duration);
      Serial.println();
      break;
  }

  digitalWrite(doorPin, HIGH);
  delay(gracePeriod); // milliseconds
  digitalWrite(doorPin, LOW);
}
