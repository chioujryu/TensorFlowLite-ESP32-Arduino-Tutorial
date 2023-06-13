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

// Provides an interface to take an action based on the output from the person
// detection model.

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_DETECTION_RESPONDER_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_DETECTION_RESPONDER_H_

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"

// Called every time the results of a person detection run are available. The
// `person_score` has the numerical confidence that the captured image contains
// a person, and `no_person_score` has the numerical confidence that the image
// does not contain a person. Typically if person_score > no person score, the
// image is considered to contain a person.  This threshold may be adjusted for
// particular applications.
// 每次人的检测结果出来后，都会调用。person_score "是捕获的图像包含一个人的数字信心，
// "no_person_score "是图像不包含一个人的数字信心。通常情况下，如果person_score > no person score，
// 图像就被认为包含一个人。 这个阈值可以为特定的应用进行调整。
void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        float person_score, float no_person_score);

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_DETECTION_RESPONDER_H_
