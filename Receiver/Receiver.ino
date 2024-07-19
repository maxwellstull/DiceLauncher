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
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  if (!radio.begin()) 
  { 
    Serial.println("radio hardware not responding!");
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  servo.attach(9);
  servo.write(10);
}

void start_motor(int power)
{
  for(int start = 255; start > power; start--)
  {
    analogWrite(5, start);
    delay(2);
  }
}


void loop() {

  if (radio.available()) 
  {
    radio.read(&data, sizeof(data));

    Serial.println("Data:");
    Serial.println(data.force);


//    float force = data.force;
//    float result = 180.0*(force)/1024.0;
//    servo.write(result);

    float force = data.force;
   // float result = 255.0*((force)/1024.0);
    float trim = 750.0;
    float result = 255.0*((trim+force)/(1024.0+trim));
    //analogWrite(5, result2);

    Serial.println(result);
    Serial.println();

    if(data.start == true)
    {
      start_motor(result);
    }
    if(data.launch == true)
    {
      servo.write(35);
      delay(300);
      analogWrite(5, 0);
      delay(1000);
      servo.write(10);
    }


  
  
  }
  delay(15);
}
