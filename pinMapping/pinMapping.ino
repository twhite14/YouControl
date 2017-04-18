#include "Joystick.h"

//Controller pin mapping definitions
#define BUTTON_A      8
#define BUTTON_B      9
#define BUTTON_X      5
#define BUTTON_Y      10
#define BUTTON_RB     A2
#define BUTTON_LB     A3
#define BUTTON_JOY    4
#define BUTTON_START  6
#define BUTTON_SELECT 12
#define NRF_CE        7
#define NRF_CSN       11
#define DEBUG_LED     13
#define JOY_X         A1
#define JOY_Y         A0

#define NUM_BUTTONS   9

//Dongle joystick config definitions
#define JOY0_ID         3
#define JOY1_ID         4
#define JOY2_ID         5
#define JOY3_ID         6
#define JOY_TYPE        JOYSTICK_TYPE_GAMEPAD
#define HAT_SWITCHES    0
#define HAS_X_AXIS      true
#define HAS_Y_AXIS      true
#define HAS_Z_AXIS      false
#define HAS_RX_AXIS     false
#define HAS_RY_AXIS     false
#define HAS_RZ_AXIS     false
#define HAS_RUDDER      false
#define HAS_THROTTLE    false
#define HAS_ACCELERATOR false
#define HAS_BRAKE       false
#define HAS_STEERING    false

#define NUM_JOYSTICKS   4

//Joystick objects
Joystick_ Joystick[] = {
  Joystick_(JOY0_ID, JOY_TYPE, NUM_BUTTONS, HAT_SWITCHES, HAS_X_AXIS, HAS_Y_AXIS, HAS_Z_AXIS, HAS_RX_AXIS, HAS_RY_AXIS, HAS_RZ_AXIS, HAS_RUDDER, HAS_THROTTLE, HAS_ACCELERATOR, HAS_BRAKE, HAS_STEERING),
  Joystick_(JOY1_ID, JOY_TYPE, NUM_BUTTONS, HAT_SWITCHES, HAS_X_AXIS, HAS_Y_AXIS, HAS_Z_AXIS, HAS_RX_AXIS, HAS_RY_AXIS, HAS_RZ_AXIS, HAS_RUDDER, HAS_THROTTLE, HAS_ACCELERATOR, HAS_BRAKE, HAS_STEERING),
  Joystick_(JOY2_ID, JOY_TYPE, NUM_BUTTONS, HAT_SWITCHES, HAS_X_AXIS, HAS_Y_AXIS, HAS_Z_AXIS, HAS_RX_AXIS, HAS_RY_AXIS, HAS_RZ_AXIS, HAS_RUDDER, HAS_THROTTLE, HAS_ACCELERATOR, HAS_BRAKE, HAS_STEERING),
  Joystick_(JOY3_ID, JOY_TYPE, NUM_BUTTONS, HAT_SWITCHES, HAS_X_AXIS, HAS_Y_AXIS, HAS_Z_AXIS, HAS_RX_AXIS, HAS_RY_AXIS, HAS_RZ_AXIS, HAS_RUDDER, HAS_THROTTLE, HAS_ACCELERATOR, HAS_BRAKE, HAS_STEERING),
  };

//Button pins in order
int buttonPins[] = {BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y, BUTTON_RB, BUTTON_LB, BUTTON_START, BUTTON_SELECT, BUTTON_JOY};

void setPinModes()
{
  for(int i = 0; i < NUM_BUTTONS; i++)
  {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(DEBUG_LED, OUTPUT);
  return;
}

//1 indicates Not Pressed, 0 indicates Pressed
uint16_t readButtons()
{
  uint16_t states;
  for(int i = 0; i < NUM_BUTTONS; i++)
  {
    states |= (digitalRead(buttonPins[i]) << i);
  }
  return states;
}

void writeButtons(uint8_t controller, uint16_t states)
{
  for(int i = 0; i < NUM_BUTTONS; i++)
  {
    if((states & (1<<i)) == 0)
    {
      Joystick[controller].pressButton(i+1);
    }
    else
    {
      Joystick[controller].releaseButton(i+1);
    }
  }
  return states;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
