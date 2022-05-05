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
    setSize (600, 600);
    setResizable(true, true);
    getConstrainer()->setFixedAspectRatio(1.0);
    setResizeLimits(600, 600, 900, 900);
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
    preDelayKnob.setRange(1.f,25.f,0.01f);
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
    auto bounds = getLocalBounds();
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();
    
    auto offset_x =  width / 10 - 10;
    auto offset_y = height / 10 ;
    
    
    earlyReflectionsKnob.setBounds(offset_x, offset_y * 2, offset_x * 2, offset_x * 2);
    reverbAmountKnob.setBounds(offset_x * 7, offset_y * 2, offset_x * 2, offset_x * 2);
    wetKnob.setBounds(offset_x, offset_y * 6, offset_x * 2, offset_x * 2);
    decayKnob.setBounds(offset_x * 7,offset_y * 6,offset_x * 2,offset_x * 2);
    preDelayKnob.setBounds(offset_x * 4,offset_y * 2,offset_x * 2,offset_x * 2);
}
