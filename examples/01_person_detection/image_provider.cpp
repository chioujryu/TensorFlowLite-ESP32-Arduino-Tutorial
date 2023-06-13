
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
#include <cstdlib>  //提供一些函式與符號常，像是size_t, wchar_t, div_t, ldiv_t, lldiv_t
#include <cstring>
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "esp_timer.h"

#include "app_camera_esp.h"
#include "esp_camera.h"
#include "model_settings.h"
#include "image_provider.h"
#include "esp_main.h"

#include "Arduino.h"

// ============define function============
#define CLI_ONLY_INFERENCE true



static const char* TAG = "app_camera";

static uint16_t *display_buf; // buffer to hold data to be sent to display



// ============Get the camera module ready============
TfLiteStatus InitCamera(tflite::ErrorReporter* error_reporter) {
//檢查是否定義 `CLI_ONLY_INFERENCE`
//如果定義了該標誌，代表只進行命令列推論，而不需要使用相機模組
// cli的全名是 Command Line Interface
#if CLI_ONLY_INFERENCE
  ESP_LOGI(TAG, "CLI_ONLY_INFERENCE enabled, skipping camera init");
  return kTfLiteOk;
#endif
// if display support is present, initialise display buf
// 在這種情況下，程式會使用 heap_caps_malloc 函式分配一塊記憶體作為顯示緩衝區。
// 分配的大小是 96 * 2 * 96 * 2 * 2，根據這段程式碼來看，
// 顯示緩衝區可能是一個 uint16_t 型別的二維陣列
#if DISPLAY_SUPPORT
  if (display_buf == NULL) {
    display_buf = (uint16_t *) heap_caps_malloc(96 * 2 * 96 * 2 * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);  // 因為最後要用 uint16_t 存取，所以最後要乘以2，因為從8bit變成16bit
    ESP_LOGE(TAG, "Success allocate display buffer");
  }
  // 分配完記憶體之後如果 `display_buf` 還是 NULL，那就會返回 kTfLiteError
  if (display_buf == NULL) {
    ESP_LOGE(TAG, "Couldn't allocate display buffer");
    return kTfLiteError;
  }
#endif

  // 如果相機初始化成功，程式使用 TF_LITE_REPORT_ERROR 函式輸出一條消息表示相機初始化完成，並返回 kTfLiteOk。
  int ret = app_camera_init();
  if (ret != 0) {
    TF_LITE_REPORT_ERROR(error_reporter, "Camera init failed\n");
    return kTfLiteError;
  }
  TF_LITE_REPORT_ERROR(error_reporter, "Camera Initialized\n");
  return kTfLiteOk;
}

void *image_provider_get_display_buf()
{
  return (void *) display_buf;
}

// ============Get an image from the camera module============
TfLiteStatus GetImage(tflite::ErrorReporter* error_reporter, int image_width,
                      int image_height, int channels, int8_t* image_data) {
  camera_fb_t* fb = esp_camera_fb_get();  // fb = frame buffe，表示相机捕获的帧缓冲区
  if (!fb) {
    ESP_LOGE(TAG, "Camera capture failed");
    return kTfLiteError;
  }

#if DISPLAY_SUPPORT
  // In case if display support is enabled, we initialise camera in rgb mode
  // Hence, we need to convert this data to grayscale to send it to tf model
  // For display we extra-polate the data to 192X192
  // 如果要查看 kNumRows, kNumCol，可以看 model_settings.h 這個文件
  for (int i = 0; i < kNumRows; i++) {
    for (int j = 0; j < kNumCols; j++) {
      uint16_t pixel = ((uint16_t *) (fb->buf))[i * kNumCols + j];

      // for inference
      uint8_t hb = pixel & 0xFF;  // hb = high binary, 獲取 pixel 右邊八個 bit
      uint8_t lb = pixel >> 8;  // lb = low binary, 獲取 pixel 左邊八個 bit
      uint8_t r = (lb & 0x1F) << 3; // r 提取了红色通道的值，通过将低字节与掩码 0x1F 进行按位与运算，并将结果左移 3 位。
      uint8_t g = ((hb & 0x07) << 5) | ((lb & 0xE0) >> 3);  // g 提取了绿色通道的值，通过将高字节的低位与掩码 0x07 进行按位与运算，
                                                            // 并将结果左移 5 位。同时，将低字节的高位与掩码 0xE0 进行按位与运算，并将结果右移 3 位。
                                                            // 最后，将两个结果进行按位或运算得到完整的绿色通道值。
      uint8_t b = (hb & 0xF8);

      /**
       * Gamma corected rgb to greyscale formula: Y = 0.299R + 0.587G + 0.114B
       * for effiency we use some tricks on this + quantize to [-128, 127]
       * 伽玛核心的rgb到灰度的公式： Y = 0.299R + 0.587G + 0.114B 为了提高效率，
       * 我们使用了一些技巧+量化到[-128, 127]。
       */
      int8_t grey_pixel = ((305 * r + 600 * g + 119 * b) >> 10) - 128;

      image_data[i * kNumCols + j] = grey_pixel;

      // to display
      display_buf[2 * i * kNumCols * 2 + 2 * j] = pixel;
      display_buf[2 * i * kNumCols * 2 + 2 * j + 1] = pixel;
      display_buf[(2 * i + 1) * kNumCols * 2 + 2 * j] = pixel;
      display_buf[(2 * i + 1) * kNumCols * 2 + 2 * j + 1] = pixel;
    }
  }
#else
  TF_LITE_REPORT_ERROR(error_reporter, "Image Captured\n");
  // We have initialised camera to grayscale
  // Just quantize to int8_t
  for (int i = 0; i < image_width * image_height; i++) {
    image_data[i] = ((uint8_t *) fb->buf)[i] ^ 0x80;
  }
#endif

  esp_camera_fb_return(fb);
  /* here the esp camera can give you grayscale image directly */
  return kTfLiteOk;
}
