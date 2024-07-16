/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "FSM.h"

/// Loop frequency
int DELAY_MS = 50;

/// Potentiometer
int POT_PIN = A0;
int POT_VALUE = 0;
/// Start button
int START_BUTTON = 2;
FSM start_fsm;

/// Launch button
int LAUNCH_BUTTON = 3;
FSM launch_fsm;

/// Message sending
const byte address[6] = "00001";

RF24 radio(7, 8);

struct Data
{
  bool start = false;
  bool launch = false;
  int force = 0;
};
Data data;

void setup() {
  Serial.begin(9600);
  pinMode(START_BUTTON, INPUT_PULLUP);
  start_fsm.init();
  start_fsm.set_recharge(300);
  
  pinMode(LAUNCH_BUTTON, INPUT_PULLUP);
  launch_fsm.init();
  launch_fsm.set_recharge(300);

  DELAY_MS = 50;
  
  if(!radio.begin()) 
  { 
    Serial.println("Radio hardware not responding!");
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {

  POT_VALUE = analogRead(POT_PIN);
  ///Serial.println(POT_VALUE);
  data.force = POT_VALUE;
  
  if(digitalRead(START_BUTTON) == 0)
  {
    if (start_fsm.press() == true)
    {
      data.start = true;
      radio.write(&data, sizeof(data));
      data.start = false;
    //start button pressed - send message
//    const char text[] = "Hello World";
//    radio.write(&text, sizeof(text));
    }
  }
  if(digitalRead(LAUNCH_BUTTON) == 0)
  {
    if(launch_fsm.press() == true)
    {    
      data.launch = true;
      radio.write(&data, sizeof(data));
      data.launch = false;
//    const char text[] = "Hello World";
//    radio.write(&text, sizeof(text));
    }
  }


  delay(DELAY_MS);
  start_fsm.update(DELAY_MS);
  launch_fsm.update(DELAY_MS);
}

