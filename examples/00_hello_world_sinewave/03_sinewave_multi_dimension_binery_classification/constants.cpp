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

#include "constants.h"

// This is a small number so that it's easy to read the logs
const int kInferencesPerCycle = 200;

// Create input values
const float k_x_1[8] = {4.3f,3.9f,0.7f,3.7f,3.4f,2.6f,2.f,4.2f};
const float k_x_2[8] = {11.8f,2.7f,11.9f,4.0f,3.3f,6.1f,3.f,10.1f};
