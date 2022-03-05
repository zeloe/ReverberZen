/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Zen_Knob.h"
//==============================================================================
/**
*/
class ReverberZenAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ReverberZenAudioProcessorEditor (ReverberZenAudioProcessor&);
    ~ReverberZenAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ReverberZenAudioProcessor& audioProcessor;
    juce::Zen_Knob earlyReflectionsKnob;
    juce::Zen_Knob reverbAmountKnob;
    juce::Zen_Knob wetKnob;
    juce::Zen_Knob decayKnob;
    juce::Zen_Knob preDelayKnob;
    using APVTS = juce::AudioProcessorValueTreeState;
            using Attachment = APVTS::SliderAttachment;
        Attachment eRAttachment, reverbAttachment, wetAttachment, decayAttachment,preDelayAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverberZenAudioProcessorEditor)
};
