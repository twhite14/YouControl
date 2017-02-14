/* Basic USB Joystick Example
   Teensy becomes a USB joystick

   You must select Joystick from the "Tools > USB Type" menu

   Pushbuttons should be connected to digital pins 0 and 1.
   Wire each button between the digital pin and ground.
   Potentiometers should be connected to analog inputs 0 to 1.

   This example code is in the public domain.
*/

void setup() {
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {
  // Simulate button presses with a half second delay between changes.
  Joystick1.button(1, 0);
  Joystick2.button(2, 0);
  Joystick3.button(3, 0);
  Joystick4.button(4, 0);
  digitalWrite(13, LOW);
  delay(500);
  
  Joystick1.button(1, 1);
  Joystick2.button(2, 1);
  Joystick3.button(3, 1);
  Joystick4.button(4, 1);
  digitalWrite(13, HIGH);
  delay(500);
}


