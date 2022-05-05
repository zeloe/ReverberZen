/*
  ==============================================================================

    Delays.h
    Created: 18 Apr 2022 7:04:17pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
class Delays
{
public:
    
    Delays();
    ~Delays();
    
    std::unique_ptr<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>> Ffdelay[2];
    
    std::unique_ptr<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>> Fbdelay[2];
    
    std::unique_ptr<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>> delay[2];
    
    std::unique_ptr<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>> feedback[2];
    
    float allpass (float inputAudio, float delayTime,float offset, int chans);
    float calc (float offset);
    float simpledelay(float inputAudio, float delayTime, int chans);
    float feedbackDelay(float inputAudio, float delayTime, float scale, int chans);
    void saveStage1(float inputAudio, int numChans);
    float returnStage1(int numChans);
    float saveStage2(float inputAudio, int numChans);
    float returnStage2(int numChans);

private:
    float output = 0;
    float feedbackoutput = 0;
    float stage1[2];
    float stage2[2];
    float stage3[2];
};





