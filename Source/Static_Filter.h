/*
  ==============================================================================

    Static_Filter.h
    Created: 5 May 2022 12:36:14pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
class Static_Filter
{
public:
    Static_Filter();
    ~Static_Filter();
    
    float process(float inputAudio,float b0, float b1, float b2, float a2, float a3);
    
    
    
private:
    float a[4] {0.0};
    float b[3] {0.0};
};
