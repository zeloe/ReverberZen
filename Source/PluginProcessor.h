/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ReverberZenAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ReverberZenAudioProcessor();
    ~ReverberZenAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout()};
private:
    juce::LinearSmoothedValue<float> eRSmooth {0.0f};
    juce::LinearSmoothedValue<float> reverbSmooth {0.0f};
    juce::LinearSmoothedValue<float> wetSmooth {0.0f};
    juce::LinearSmoothedValue<float> decaySmooth {0.0f};
    juce::LinearSmoothedValue<float> preDelaySmooth {0.0f};
    
    juce::dsp::DelayLine<float> preDelayL {19200} ;
    juce::dsp::DelayLine<float> firstFIRL {14400} ;
    juce::dsp::DelayLine<float> firstIIRL {14400} ;
    juce::dsp::DelayLine<float> secondFIRL {14400} ;
    juce::dsp::DelayLine<float> secondIIRL {14400} ;
    juce::dsp::DelayLine<float> thirdFIRL {14400} ;
    juce::dsp::DelayLine<float> thirdIIRL {14400} ;
    juce::dsp::DelayLine<float> fourthFIRL {14400} ;
    juce::dsp::DelayLine<float> fourthIIRL {14400} ;
    
    juce::dsp::DelayLine<float> fifthIIRL {14400} ;
    juce::dsp::DelayLine<float> sixthIIRL {14400} ;
    juce::dsp::DelayLine<float> seventhIIRL {14400} ;
    juce::dsp::DelayLine<float> eightIIRL {14400} ;
    
    juce::dsp::DelayLine<float> firstalignL {14400} ;
    juce::dsp::DelayLine<float> secondalignL {14400} ;
    juce::dsp::DelayLine<float> thirdalignL {14400} ;
    juce::dsp::DelayLine<float> fourthalignL {14400} ;
    
    juce::dsp::DelayLine<float> feedbackL {19200} ;
    juce::dsp::DelayLine<float> feedbacktwoL {19200} ;
    
    juce::dsp::DelayLine<float> preDelayR {19200} ;
    juce::dsp::DelayLine<float> firstFIRR {14400} ;
    juce::dsp::DelayLine<float> firstIIRR {14400} ;
    juce::dsp::DelayLine<float> secondFIRR {14400} ;
    juce::dsp::DelayLine<float> secondIIRR {14400};
    juce::dsp::DelayLine<float> thirdFIRR {14400} ;;
    juce::dsp::DelayLine<float> thirdIIRR {14400} ;
    juce::dsp::DelayLine<float> fourthFIRR {14400} ;
    juce::dsp::DelayLine<float> fourthIIRR {14400} ;
    
    juce::dsp::DelayLine<float> fifthIIRR {14400} ;
    juce::dsp::DelayLine<float> sixthIIRR {14400} ;
    juce::dsp::DelayLine<float> seventhIIRR {14400} ;
    juce::dsp::DelayLine<float> eightIIRR {14400} ;
    
    juce::dsp::DelayLine<float> firstalignR {14400} ;
    juce::dsp::DelayLine<float> secondalignR {14400} ;
    juce::dsp::DelayLine<float> thirdalignR {14400} ;
    juce::dsp::DelayLine<float> fourthalignR {14400} ;
    
    juce::dsp::DelayLine<float> feedbackR {19200} ;
    juce::dsp::DelayLine<float> feedbacktwoR {19200} ;
    
   
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverberZenAudioProcessor)
};
