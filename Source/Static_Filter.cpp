/*
  ==============================================================================

    Static_Filter.cpp
    Created: 5 May 2022 12:36:14pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#include "Static_Filter.h"
Static_Filter::Static_Filter()
{
    
}
Static_Filter::~Static_Filter()
{
    
}

float Static_Filter::process(float inputAudio, float b0, float b1, float b2,  float a2, float a3)
{
   
    b[0] = inputAudio;
    a[0] = b[0] * b0+ b[1] * b1 + b[2] * b2 + a[1] * a2 + a[2] * a3;
    
    
    b[2] = b[1];
    b[1] = b[0];
    a[3] = a[2];
    a[2] = a[1];
    a[1] = a[0];
    return a[0];
}
