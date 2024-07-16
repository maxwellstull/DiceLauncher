/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <printf.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>


const byte address[6] = "00001";

Servo servo;
RF24 radio(7, 8); // CE, CSN

struct Data
{
  bool start = false;
  bool launch = false;
  int force = 0;
};
Data data;

void setup() {
  Serial.begin(9600);
  if (!radio.begin()) 
  { 
    Serial.println(F("radio hardware not responding!"));
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  servo.attach(9);
}

void loop() {

  if (radio.available()) {
    char text[32] = "";
    radio.read(&data, sizeof(data));
    Serial.println(data.force);
    Serial.println(data.launch);
    Serial.println(data.start);

    float force = data.force;
    float result = (180.0*force)/1024.0;
    Serial.println(result);

    servo.write(result);
    
    Serial.println();
  }


  delay(15);
}