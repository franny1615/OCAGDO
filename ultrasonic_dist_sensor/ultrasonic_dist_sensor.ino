// references: 
// https://docs.arduino.cc/language-reference/en/functions/digital-io/pinMode/
// https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1
// https://docs.arduino.cc/learn/microcontrollers/digital-pins/
// https://www.arduino.cc/reference/cs/language/functions/communication/serial/begin/
// https://docs.arduino.cc/language-reference/en/functions/digital-io/digitalwrite/

const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 13;

float duration;

void setup() {
  pinMode(trigPin, OUTPUT); // tells board that on this pin we will be serving out current.
  pinMode(ledPin, OUTPUT);
  pinMode(echoPin, INPUT);  // tells board that on this pin to report minute changes in current, serves as a way to read data in. 
  Serial.begin(9600); // baud, its a frequency int bits per second
}

void loop() {
  digitalWrite(trigPin, LOW);   // send 0V to trigger pin, basically resets state
  delayMicroseconds(2);         // wait
  digitalWrite(trigPin, HIGH);  // send 5V to output pin, since it was neutral, this tells sensor, do something
  delayMicroseconds(10);        // wait for sound to travel and hit sensor again
  digitalWrite(trigPin, LOW);   // reset state

  duration = pulseIn(echoPin, HIGH); // at this point sensor has some current stored in it representing distance traveled, read it in, measured in microseconds
  Serial.println(duration);

  if (duration >= 1350 && duration <= 1500) // about 9 inches away, threshold
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  
  delay(100);
}
