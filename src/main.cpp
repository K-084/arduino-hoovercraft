/************************************************************************************************************************************************************************                                              
 * - Author               : Shafiqur Khan                                                                              
 * - Copyright (c) holder : All rights reserved
 * - License              : GNU GPLv3
 * - Date                 : 13/06/2019
 * ***********************************************************************************************************************************************************************/
 
 /*********************************** NOTE **************************************/
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED

                    /* Custom Shield Configuration */
#define CUSTOM_SETTINGS
#define INCLUDE_SLIDER_SHIELD
#define INCLUDE_GAMEPAD_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_BUZZER_SHIELD
#include <OneSheeld.h> 
#include <ESC.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

                      /* Set the limitation Speed in microseconds */          
#define SPEED_MIN (1000)     
#define SPEED_MAX (1900)

                       /* Motor configuration */
                       /* ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)*/
ESC m1 (3, SPEED_MIN, SPEED_MAX, 500);      
ESC m2 (5, SPEED_MIN, SPEED_MAX, 500);
ESC m3 (6, SPEED_MIN, SPEED_MAX, 500);
ESC m4 (9, SPEED_MIN, SPEED_MAX, 500);     

          /* Int Variables */
int value;             //For Slider
int LED_PIN = 13;
int m1_val = 0;
int m4_val = 0;
int m1_Pot = A1;
int m4_Pot = A0;
                                                   /* Tempreture */
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
float temperature;

            /* Send the Arm value so the ESC will be ready to take commands */
void setup() { 
  OneSheeld.begin();
  m1.arm();       
  m2.arm();
  m3.arm();
  m4.arm();
  pinMode(LED_PIN, OUTPUT);             // LED Visual Output
  digitalWrite(LED_PIN, HIGH);          // LED High Once Armed

  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Terminal.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}

void loop(void)
{
                               /*Slider Begin & start thrusters*/
  value = Slider.getValue();
  value = map(value,0,255,1000,1800);
  m2.speed(value);
  m3.speed(value);
  

  m1_val = analogRead(m1_Pot);                    //Read frome the left potentionemter
  m1_val = map(m1_val, 0, 1023, 1000, 1800);

  
  m4_val = analogRead(m4_Pot);                  //Read from the right poetnttiomer
  m4_val = map(m4_val, 0, 1023, 1000, 1800);

  
                              /*Gamepad Begin & Navigation*/
  if(GamePad.isUpPressed()) {         //Go Forward 
    m1.speed(m1_val);
    m4.speed(m4_val);
    
  } else if (GamePad.isBluePressed()) {    //Go Left
    m1.speed(1000);
    m4.speed(1500); 
    
  } else if (GamePad.isLeftPressed()) {    //Go Left at Full Speed
    m1.speed(1000);
    m4.speed(1800);

  } else if (GamePad.isRedPressed()) {    //Go Right
   m1.speed(1500);
   m4.speed(1000);   
  
  } else if (GamePad.isRightPressed()) {   //Go Right Full Speed
    m1.speed(1800);
    m4.speed(1000);

  } else if (GamePad.isDownPressed()) {    //Go Backward
   m1.speed(-1500);
   m4.speed(-1500);

  } else if (GamePad.isGreenPressed())  { /*Turn off all the motors */
   m1.speed(1000);
   m2.speed(1000);
   m3.speed(1000);
   m4.speed(1000);

  } else {          /* Do Nothing if nothing is pressed */
   m1.speed(1000);
   m4.speed(1000);

  }

  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);

  if(event.temperature){
  bmp.getTemperature(&temperature);
 
  Terminal.print("Temperature: ");
  Terminal.print(temperature);
  Terminal.println(" C");
  delay(30);

  } else {
    Terminal.println("Sensor error");
    delay(50);
  }

  if(temperature > 75.00) {
    Buzzer.buzzOn();
    delay(50);
    Buzzer.buzzOn();
    delay(50);
  } else {
    Buzzer.buzzOff();
  }
}
