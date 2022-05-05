/*
  ==============================================================================

    Delays.cpp
    Created: 18 Apr 2022 7:04:17pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#include "Delays.h"
Delays::Delays():
output(0),
feedbackoutput(0)
{
    for (int i = 0; i < 2; i++){
        Ffdelay[i] = std::make_unique<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>> (14400);
        Fbdelay[i] = std::make_unique<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>> (14400);
        delay[i] = std::make_unique<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>> (14400);
        feedback[i] = std::make_unique<juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear>> (14400);
        }
}
Delays::~Delays()
{
    
}
float Delays::calc(float offset)
{
    return 1.f - offset;
}


float Delays::allpass(float inputAudio, float delayTime, float offset, int chans)
{
    Ffdelay[chans]->pushSample(chans, inputAudio);
    Ffdelay[chans]->setDelay(delayTime);
    output = Ffdelay[chans]->popSample(chans) * this->calc(offset) -offset * Fbdelay[chans]->popSample(chans);
    Fbdelay[chans]->pushSample(chans,output);
    Fbdelay[chans]->setDelay(delayTime);
    return output;
}

float Delays::simpledelay(float inputAudio, float delayTime, int chans)
{
    delay[chans]->pushSample(chans, inputAudio);
    delay[chans]->setDelay(delayTime);
    
    
    return delay[chans]->popSample(chans);
}

float Delays::feedbackDelay(float inputAudio, float delayTime, float scale, int chans)
{
    feedbackoutput = inputAudio + feedback[chans]->popSample(chans) * scale;
    feedback[chans]->pushSample(chans, feedbackoutput);
    feedback[chans]->setDelay(delayTime);
    return feedbackoutput;
}

void Delays::saveStage1(float inputAudio, int numChans)
{
    stage1[numChans] = inputAudio;
}

float Delays::returnStage1(int numChans)
{
    return stage1[numChans];
}

float Delays::saveStage2(float inputAudio, int numChans)
{
    stage2[numChans] = inputAudio;
    return stage2[numChans];
}

float Delays::returnStage2(int numChans)
{
    return stage2[numChans];
}
