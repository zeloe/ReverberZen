/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Delays.h"
#include "GainStage.h"
#include "Static_Filter.h"
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
    void initDSP() {
        for (int i = 0; i < 2; i++){
                
                    predelay[i] = std::make_unique<Delays>();
            
                    delayOne[i] = std::make_unique<Delays>();
                    
                    delayTwo[i] = std::make_unique<Delays>();
                    
                    delayThree[i] = std::make_unique<Delays>();
                    
                    delayFour[i] = std::make_unique<Delays>();
                    
                    delayFive[i] = std::make_unique<Delays>();
                    
                    delaySix[i] = std::make_unique<Delays>();
                    
                    delaySeven[i] = std::make_unique<Delays>();
                    
                    delayEight[i] = std::make_unique<Delays>();
            
                    lopFilterOne[i] = std::make_unique<Static_Filter>();
            
                    earlyGain[i] = std::make_unique<Gain>();
            
                    feedbackGain[i] = std::make_unique<Gain>();
            
                    wetGain[i] = std::make_unique<Gain>();
            
                    lopFilterOne[i] = std::make_unique<Static_Filter>();
            
                    lopFilterTwo[i] = std::make_unique<Static_Filter>();
            
                    lopFilterThree[i] = std::make_unique<Static_Filter>();
            
                    lopFilterFour[i] = std::make_unique<Static_Filter>();
            
                    lopFilterFive[i] = std::make_unique<Static_Filter>();
                    
                    lopFilterSix[i] = std::make_unique<Static_Filter>();
                    
                    lopFilterSeven[i] = std::make_unique<Static_Filter>();
            
                    lopFilterEight[i] = std::make_unique<Static_Filter>();
                }
    }
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout()};
private:

    juce::LinearSmoothedValue<float> reverbSmooth {0.0f};
    juce::LinearSmoothedValue<float> preDelaySmooth {0.0f};
    
    
    std::unique_ptr<Delays> predelay[2];
    std::unique_ptr<Delays> delayOne[2];
    std::unique_ptr<Delays> delayTwo[2];
    std::unique_ptr<Delays> delayThree[2];
    std::unique_ptr<Delays> delayFour[2];
    std::unique_ptr<Delays> delayFive[2];
    std::unique_ptr<Delays> delaySix[2];
    std::unique_ptr<Delays> delaySeven[2];
    std::unique_ptr<Delays> delayEight[2];
    
    std::unique_ptr<Gain> earlyGain[2];
    std::unique_ptr<Gain> feedbackGain[2];
    std::unique_ptr<Gain> wetGain[2];
    
    std::unique_ptr<Static_Filter> lopFilterOne[2];
    std::unique_ptr<Static_Filter> lopFilterTwo[2];
    std::unique_ptr<Static_Filter> lopFilterThree[2];
    std::unique_ptr<Static_Filter> lopFilterFour[2];
    std::unique_ptr<Static_Filter> lopFilterFive[2];
    std::unique_ptr<Static_Filter> lopFilterSix[2];
    std::unique_ptr<Static_Filter> lopFilterSeven[2];
    std::unique_ptr<Static_Filter> lopFilterEight[2];
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverberZenAudioProcessor)
};
