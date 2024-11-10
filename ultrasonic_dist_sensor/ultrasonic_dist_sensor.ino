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

const int parkedLow = 0; 
const int parkedHigh = 700;
const int triggerDoorOpen = 800;
const int triggerDoorClosed = 43000;
const int gracePeriod = 100; // ms

bool garageDoorIsOpen = false;
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

  if (duration >= parkedLow && duration <= parkedHigh) // about 9 inches away, threshold
  {
    digitalWrite(ledPin, HIGH);
    triggerDoor(2, duration);
  }
  else if (duration > triggerDoorOpen && duration < triggerDoorClosed)
  {
    digitalWrite(ledPin, LOW);
    triggerDoor(1, duration); // door open
  }
  else if (duration > triggerDoorClosed)
  {
    digitalWrite(ledPin, LOW);
    triggerDoor(2, duration); // door closed
  }

  // enable when calibrating
  // Serial.println(duration);
}

void triggerDoor(int eventType, float duration)
{

  bool shouldTriggerEvent = false;
  switch (eventType)
  {
    case 1: // door open
      shouldTriggerEvent = !garageDoorIsOpen;
      if (shouldTriggerEvent)
      {
        Serial.print("door was opened:");
        Serial.print(duration);
        Serial.println();
      }
      break;
    case 2: // door closed
      shouldTriggerEvent = garageDoorIsOpen;
      if (shouldTriggerEvent)
      {
        Serial.print("door was closed:");
        Serial.print(duration);
        Serial.println();
      }
      break;
  }

  if (shouldTriggerEvent)
  {
    garageDoorIsOpen = !garageDoorIsOpen;
    digitalWrite(doorPin, HIGH);
    delay(gracePeriod); // milliseconds
    digitalWrite(doorPin, LOW);
  }
  else
  {
    delay(gracePeriod); // milliseconds
  }
}
