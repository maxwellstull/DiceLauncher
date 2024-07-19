#include <printf.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo servo;

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
  if (!radio.begin()) 
  { 
    Serial.println("radio hardware not responding!");
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  servo.attach(9);
}

void loop() {
  if (radio.available()) 
  {
    radio.read(&data, sizeof(data));

    float force = data.force;
    float result = (180.0*force)/1024.0;
    servo.write(result);
  }


  delay(15);
}