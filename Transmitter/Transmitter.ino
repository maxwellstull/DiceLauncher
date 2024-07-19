#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "FSM.h"

/// Loop frequency
int loop_delay = 50;

/// Potentiometer
int POT_PIN = A0;
int pot_value = 0;
int last_pot_value = 0;

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
//  Serial.begin(9600);
  // Initialize start button FSM
  pinMode(START_BUTTON, INPUT_PULLUP);
  start_fsm.init();
  start_fsm.set_recharge(300);
  
  // Initialize launch button FSM
  pinMode(LAUNCH_BUTTON, INPUT_PULLUP);
  launch_fsm.init();
  launch_fsm.set_recharge(100);
  
  // Make radio start
  if(!radio.begin()) 
  { 
    Serial.println("Radio hardware not responding!");
  }
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  // Store last pot value and get pot value - only send if pot value changed
  last_pot_value = pot_value;
  pot_value = analogRead(POT_PIN);
  if(last_pot_value != pot_value)
  {  
    data.force = pot_value;
  }  
  // Check if start button pressed
  if(digitalRead(START_BUTTON) == 0)
  {
    if (start_fsm.press() == true)
    {
      data.start = true;
      radio.write(&data, sizeof(data));
      data.start = false;
    }
  }
  // Check if launch button pressed
  if(digitalRead(LAUNCH_BUTTON) == 0)
  {
    if(launch_fsm.press() == true)
    {    
      data.launch = true;
      radio.write(&data, sizeof(data));
      data.launch = false;
    }
  }
  //Update FSM timers and then delay
  start_fsm.update(loop_delay);
  launch_fsm.update(loop_delay);
  delay(loop_delay);

}

