#include <SPI.h>
#include <Wire.h>
#include "RF24.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*DEFINE SETUPS*/
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

/*PIN SETUPS*/
int ButtonA = 8;
int ButtonB = 9;
int ButtonX = 5;
int ButtonY = 10;
int ButtonStart = 6;//gotta define these after final implementation
int ButtonSelect = 12;
int ButtonRbump = A2;
int ButtonLbump = A3;
int Xaxis = A1;
int Yaxis = A0;
int count = 0;
bool cntrl_assign = false;
RF24 radio(7,11);
byte addresses[][6] = {"1Node","2Node"}; //first is check pipe second is feedback
long pipeassign;
long getassign;
/*setup======================================================*/
void setup() {
//start lcd
Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(4000);
  display.clearDisplay();
//start radio
  radio.begin();//start it
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);//open check pipe
  radio.startListening();//start listening on the pipe
//button setup
  pinMode(ButtonA,INPUT);
  pinMode(ButtonB,INPUT);
  pinMode(ButtonY,INPUT);
  pinMode(ButtonX,INPUT);
}
/*loop=======================================================*/
long selector = 1;
void loop() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
  while(!cntrl_assign){//while not assigned, check for assignment value
      delay(100);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,16);
      display.println("start to change #,");
      display.setCursor(0,26);
      display.println("select to confirm :");
      
        //cntrl_assign = true;
      
      if(digitalRead(ButtonStart) == LOW){
        selector++;
        if (selector == 5){selector = 1;}
      }
      if (selector == 1){
        display.setCursor(0,0);
        display.println("[Controller 1]");
      }
      else if (selector == 2){
        display.setCursor(0,0);
        display.println("[Controller 2]");
      }
      else if (selector == 3){
        display.setCursor(0,0);
        display.println("[Controller 3]");
      }
      else if (selector == 4){
        display.setCursor(0,0);
        display.println("[Controller 4]");
      }
      display.display();
      if(digitalRead(ButtonSelect) == LOW){
         cntrl_assign = true;
      }
    }
  long dispnum = selector - 1;
  radio.stopListening();
  radio.openWritingPipe(addresses[1]);
  
  while(cntrl_assign){
    unsigned long sendmsg = statusreturn(dispnum);
    radio.write(&sendmsg,sizeof(unsigned long));
    delay(10);
    if(count>=20){
  //    update screen stuff
  //    check battery levels
    
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,30);
      display.println("ctrl #");
      display.setTextSize(2);
      display.setCursor(20,40);
      display.print(selector,DEC);
      display.display();
      count = 0;
    }
    count++;
  }
  
}
/*statusreturn===============================================*/
long statusreturn(long cntrlnum){ //status of all states
  long stat = 0;
  long a     = digitalRead(ButtonA);
  long b     = digitalRead(ButtonB);
  long x     = digitalRead(ButtonX);
  long y     = digitalRead(ButtonY);
  long sta   = digitalRead(ButtonStart);
  long sel   = digitalRead(ButtonSelect);
  long rbump = digitalRead(ButtonRbump);
  long lbump = digitalRead(ButtonLbump);
  long xax   = analogRead(Xaxis)/4;
  long yax   = analogRead(Yaxis)/4;

  //OR the bitshifted states to make 4 byte state
  //DIO 0-7B 
  //Analog 8-15B, 16-23B
  //controller number 24-31B
  stat |= (a); 
  stat |= (b << 1);
  stat |= (x << 2);
  stat |= (y << 3);
  stat |= (sta << 4); 
  stat |= (sel << 5);
  stat |= (rbump << 6);
  stat |= (lbump << 7);
  stat |= (xax << 8);
  stat |= (yax << 16);
  stat |= (cntrlnum << 24);
  Serial.println(stat,HEX);
  
  return stat;
}//end statusreturn
