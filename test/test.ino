#include "HX711.h"
#include<SPI.h>
#include<RF24.h>
HX711 scale(5, 6);
RF24 radio(9, 10);
//Ultra sonic sensor references
#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED
int buzzer=4;//Smoke Sensor
int maximumRange = 100; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
float calibration_factor = 2230; // this calibration factor is adjusted according to my load cell
float units;
float ounces;
void setup() 
{
Serial.begin(9600);
 //smoke sensor  
pinMode(buzzer, OUTPUT);
//Load cell setup code
   Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  scale.set_scale();
  scale.tare();  //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
//ultrasonic setup
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)

}

void loop() 
{
Smoke();
load();
Ultra();
}

void Smoke()
{
float sensorVoltage; 
  float sensorValue;
   sensorValue = analogRead(A0);
  sensorVoltage = sensorValue/1024*5.0;
   Serial.print("sensor voltage = ");
  Serial.print(sensorVoltage);
  if(sensorVoltage > 1.0)
  {
          digitalWrite(buzzer, HIGH);
  }
  else
  {
              digitalWrite(buzzer, LOW);
  }
  Serial.println(" V");
  delay(1000);
 
}

void load()
{
 scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading: ");
  units = scale.get_units(), 10;
  if (units < 0)
  {
    units = 0.00;
  }
  ounces = units * 0.035274;
  Serial.print(units);
  Serial.print(" grams"); 
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
delay(1000);
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }
}


void Ultra()
{
digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
  //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
  if (distance >= maximumRange || distance <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
  Serial.println("Garbage Collector full");
 digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.print("Garbage Label");
 Serial.println(distance);
 digitalWrite(LEDPin, LOW); 
 }
  //Delay 50ms before next reading.
 delay(50);
 
}
