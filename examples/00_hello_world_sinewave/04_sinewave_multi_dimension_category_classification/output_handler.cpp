/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "Arduino.h"
#include "output_handler.h"


// The pin of the Arduino's built-in LED
int led = LED_BUILTIN;   //定義輸出腳位


void HandleOutput(float x_value_1,float x_value_2,int y_value) {

  switch(y_value)
  {
  case 0:
    digitalWrite (led, LOW);
    break;
  case 1:
    digitalWrite (led, HIGH);
    delay(1000);
    digitalWrite (led, LOW);
    break;
  case 2:
    for(int i = 0 ; i < 2 ; i++)
    {
      digitalWrite (led, HIGH);
      delay(1000);
      digitalWrite (led, LOW);
      delay(1000);
    }
    break;
  case 3:
    for(int i = 0 ; i < 3 ; i++)
    {
      digitalWrite (led, HIGH);
      delay(1000);
      digitalWrite (led, LOW);
      delay(1000);
    }
    break;
  default:
    break;
  }
  
  //print x and y
  Serial.printf("k_x_1 = %f,  k_x_2 = %f,  y = %d \n\n",x_value_1,x_value_2,y_value);
  Serial.flush();
  Serial.printf("\n");
  Serial.flush();

  delay(1000);
}






