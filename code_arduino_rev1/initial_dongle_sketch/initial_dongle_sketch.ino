#include <SPI.h>
#include <Wire.h>
#include "RF24.h"
#include "Joystick.h"

RF24 radio(7,11);
byte addresses[][6] = {"1Node","2Node"}; //first is check pipe second is feedback
byte pipeassign = 0;

#define NUM_BUTTONS   8
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for(int index=0;index<NUM_JOYSTICKS;index++){
    Joystick[index].begin(true);
    Joystick[index].setXAxisRange(255, 0);
    Joystick[index].setYAxisRange(255, 0);
  }
  radio.begin();//start it
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);//open check pipe
  radio.startListening();//start listening on the pipe
}
unsigned long gotmsg = 0x0;

void loop() {
  
  if(radio.available()){
    while(radio.available()){
    radio.read(&gotmsg, sizeof(unsigned long));
    uint8_t controller_num = (gotmsg >> 24) & 0xFF;
    uint8_t button_states = (gotmsg) & 0xFF;
    uint8_t xax = 0xFF&(gotmsg>>8);
    uint8_t yax = 0xFF&(gotmsg>>16);
    Serial.println(gotmsg,HEX);
    Serial.print("controller number:  ");
    Serial.println(controller_num,HEX);
    Serial.print("button states:      ");
    Serial.println(button_states,HEX);
    Serial.print("x axis value:       ");
    Serial.println(xax,HEX);
    Serial.print("y axis value:       ");
    Serial.println(yax,HEX);
    Joystick[controller_num].setXAxis(xax);
    Joystick[controller_num].setYAxis(yax);
    writeDButtons(controller_num, button_states);
    }
  }
}

//Button pins in order
void writeDButtons(uint8_t controller, uint16_t states)
{
  for(int i = 0; i < NUM_BUTTONS; i++)
  {
    if(((states>>i) & 1) == 0)
    {
      Joystick[controller].pressButton(i);
    }
    else if(((states>>i) & 1) == 1)
    {
      Joystick[controller].releaseButton(i);
    }
  }
  return states;
}

