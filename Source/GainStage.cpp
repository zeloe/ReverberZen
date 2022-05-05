/*
  ==============================================================================

    GainStage.cpp
    Created: 18 Apr 2022 8:29:31pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#include "GainStage.h"
Gain::Gain():
offset(0)
{
    for (int i = 0; i < 2; i++){
        gainParameter[i] = std::make_unique<juce::LinearSmoothedValue<float>> (0.0f) ;
    }
}
Gain::~Gain()
{
    
}

float Gain::gainprocess(float inputaudio, int chan)
{
    audiotogiveback[chan] = inputaudio * gainParameter[chan]->getNextValue();
    return inputaudio * gainParameter[chan]->getNextValue();
}

float Gain::offsetgianprocess(float inputaudio, int chan)
{
    offset = 1.f - gainParameter[chan]->getNextValue();
    return inputaudio * offset;
}

void Gain::savestage(float inputaudio, int numChans)
{
    audiotogiveback[numChans] = inputaudio;
}

float Gain::returnstage(int numChans)
{
    return audiotogiveback[numChans];
}
