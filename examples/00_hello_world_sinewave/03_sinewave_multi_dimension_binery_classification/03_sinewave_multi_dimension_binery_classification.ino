#include <TensorFlowLite_ESP32.h>
/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

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
#include "tensorflow/lite/micro/all_ops_resolver.h" //這個類別可讓解譯器仔入我們的模型使用的op(operation)操作
#include "tensorflow/lite/micro/micro_error_reporter.h" //這個類別可以log錯誤並將它輸，來協助除錯
#include "tensorflow/lite/micro/micro_interpreter.h"  //Tensorflow Lite Microcontroller解譯器，他將執行我們的模型
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/system_setup.h" 
#include "tensorflow/lite/schema/schema_generated.h"  //定義Tensorflow的

#include "model.h"  //我們之前訓練好並且用xxd來轉換的sine模型
#include "constants.h"
#include "output_handler.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 6000;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {

  Serial.begin(115200);

  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  // 包含全部的op方，這個方法比較占記憶體，如果不想占記憶，可以用下面的方法
  // static tflite::AllOpsResolver resolver;    
  //要這樣打也可，這樣比較不占記憶體
  static tflite::MicroMutableOpResolver<2> resolver;
  resolver.AddFullyConnected();  
  resolver.AddLogistic();  // Sigmoid


  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

}

// The name of this function is important for Arduino compatibility.
void loop() {

  for (int i = 0; i < sizeof(k_x_1) / sizeof(k_x_1[0]); i++)
  {

      int8_t k_x_1_quantized = k_x_1[i] / input->params.scale + input->params.zero_point;
      input->data.int8[0] = k_x_1_quantized;

      int8_t k_x_2_quantized = k_x_2[i] / input->params.scale + input->params.zero_point;
      input->data.int8[1] = k_x_2_quantized;


      // Run inference, and report any error
      TfLiteStatus invoke_status = interpreter->Invoke();
      if (invoke_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on %f, %f",
                            static_cast<double>(k_x_1[i]),static_cast<double>(k_x_2[i]) );
        return;
      }

      // Obtain the quantized output from model's output tensor
      int8_t y_quantized = output->data.int8[0];        
      // Dequantize the output from integer to floating-point
      float y = (y_quantized - output->params.zero_point) * output->params.scale;

      // Let the LED light on
      HandleOutput(k_x_1[i],k_x_2[i],y);
  }

  while(1);
  
}
