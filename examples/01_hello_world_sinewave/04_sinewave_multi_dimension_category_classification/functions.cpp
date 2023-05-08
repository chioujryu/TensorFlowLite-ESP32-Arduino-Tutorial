#include "functions.h"
#include "Arduino.h"

// Find the biggest number index, then return the index of biggest number
int FindBiggestNumberIndex(int (&arr)[4])
{
  int index = 0;
  int buffer = 0;
  for (int i = 0 ; i < sizeof(arr)/ sizeof(arr[0]) ; i++)
  {
    if (buffer < arr[i])
    {
      buffer = arr[i];
      index = i;
    }
  }
  return index;
}

// Find the biggest number index, then return the index of biggest number
int FindBiggestNumber(int (&arr)[4])
{
  int buffer = 0;
  for (int i = 0 ; i < sizeof(arr)/ sizeof(arr[0]) ; i++)
  {
    if (buffer < arr[i])
    {
      buffer = arr[i];
    }
  }
  return buffer;
}

