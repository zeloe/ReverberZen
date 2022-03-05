/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverberZenAudioProcessorEditor::ReverberZenAudioProcessorEditor (ReverberZenAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
earlyReflectionsKnob("ER", 0, 1, 0.1, 0),
reverbAmountKnob("Reverb",0,1,0.1,0),
wetKnob("Wet", 0,1,0.1,0),
decayKnob("Decay", 0, 1, 0.1, 0),
preDelayKnob("preDelay", 0, 1, 0.1, 0),
eRAttachment(audioProcessor.apvts, "ER", earlyReflectionsKnob),
reverbAttachment(audioProcessor.apvts, "Reverb", reverbAmountKnob),
wetAttachment(audioProcessor.apvts, "Wet", wetKnob),
decayAttachment(audioProcessor.apvts, "Decay", decayKnob),
preDelayAttachment(audioProcessor.apvts, "PreDelay", preDelayKnob)

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (450, 450);
    setResizeLimits(450, 450, 600, 600);
    earlyReflectionsKnob.setStyles(juce::Zen_Knob::ZenStyles::blueknob);
    earlyReflectionsKnob.setTextValueSuffix(" ER");
    earlyReflectionsKnob.setRange(0.00f, 1.f,0.01f);
    addAndMakeVisible(&earlyReflectionsKnob);
        
    reverbAmountKnob.setStyles(juce::Zen_Knob::ZenStyles::redknob);
    reverbAmountKnob.setTextValueSuffix(" Reverb");
    reverbAmountKnob.setRange(0.01f,1.f,0.01f);
    addAndMakeVisible(&reverbAmountKnob);
    
    wetKnob.setStyles(juce::Zen_Knob::ZenStyles::whiteknob);
    wetKnob.setTextValueSuffix(" Wet");
    wetKnob.setRange(0.00f,1.f,0.01f);
    addAndMakeVisible(&wetKnob);
    
    decayKnob.setStyles(juce::Zen_Knob::ZenStyles::greenknob);
    decayKnob.setTextValueSuffix(" Decay");
    decayKnob.setRange(0.00f,1.f,0.01f);
    addAndMakeVisible(&decayKnob);
    
    preDelayKnob.setStyles(juce::Zen_Knob::ZenStyles::lightblueknob);
    preDelayKnob.setTextValueSuffix(" PreDelay");
    preDelayKnob.setRange(0.01f,25.f,0.01f);
    addAndMakeVisible(&preDelayKnob);
    
}

ReverberZenAudioProcessorEditor::~ReverberZenAudioProcessorEditor()
{
}

//==============================================================================
void ReverberZenAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);
}

void ReverberZenAudioProcessorEditor::resized()
{
    earlyReflectionsKnob.setBounds(50, 100, 150, 150);
    reverbAmountKnob.setBounds(300, 100, 150, 150);
    wetKnob.setBounds(50, 300, 150, 150);
    decayKnob.setBounds(300,300,150,150);
    preDelayKnob.setBounds(175,100,150,150);
}
