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

// Track whether the function has run at least once
//追蹤函數是否已經運行過至少一次。
bool initialized = false;


void HandleOutput(float x_value_1,float x_value_2,float y_value) {

  // Do this only once
  if (!initialized){
    //set the LED pin to output
    pinMode(led, OUTPUT);
    initialized = true;
  }

  if (y_value > 0.5)
  {
    digitalWrite (led, HIGH);
  }
  else
  {
    digitalWrite (led, LOW);
  }
  
  //印出 x_value 值以及 y_value
  //print x and y
  Serial.printf("k_x_1 = %f,  k_x_2 = %f,  y = %f \n\n",x_value_1,x_value_2,y_value);
  Serial.flush();
  Serial.printf("\n");
  Serial.flush();

  delay(1000);
}






