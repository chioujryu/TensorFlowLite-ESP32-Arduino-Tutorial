# **Sinewave Multi Dimension Category Classification**

This example is designed to demonstrate the absolute basics of using [TensorFlow
Lite for Microcontrollers](https://www.tensorflow.org/lite/microcontrollers).
It includes the full end-to-end workflow of training a model, converting it for
use with TensorFlow Lite for Microcontrollers for running inference on a
microcontroller.

Furthermore, this example takes two inputs and produces multiple outputs, which is typical for multi-class classification problems where the objective is to predict between multiple classes.

## **Training your Model**
To begin, you can access this [Colab](https://colab.research.google.com/drive/1mx-3SY7k4Omv2RV2wz2u_mNshW62V0nR?usp=sharing) notebook to train your model.


## **Download your Model**
On the left-hand side of `Colab`, click on the file and download `model.cc`
![123](../assets/2.png)

## **Deploy the model to Microcontroller**
First, change the file extension of `model.cc` to `.cpp`, and then replace the existing `model.cpp` file. After that, include `model.h` by adding `#include "model.h`" in the code. Finally, you can upload the program to the microcontroller.

## **AllOpsResolver and MicroMutableOpResolver**
Creating a deep neural network on the microcontroller requires knowledge of using `AllOpsResolver` and `MicroMutableOpResolver`. While using `MicroMutableOpResolver` necessitates manually creating a deep neural network on the microcontroller, the advantage is that it consumes less memory. It is highly recommended to build the neural network on the microcontroller by continuously comparing it to the neural network trained in Python. You can find the AllOpsResolver source code in [AllOpsResolver](../../../src/tensorflow/lite/micro/all_ops_resolver.cpp).
