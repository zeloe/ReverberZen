/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverberZenAudioProcessor::ReverberZenAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

ReverberZenAudioProcessor::~ReverberZenAudioProcessor()
{
}

//==============================================================================
const juce::String ReverberZenAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverberZenAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverberZenAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverberZenAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverberZenAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverberZenAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverberZenAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverberZenAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverberZenAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverberZenAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverberZenAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    eRSmooth.reset(sampleRate, 0.01f);
    reverbSmooth.reset(sampleRate, 0.0001f);
    wetSmooth.reset(sampleRate, 0.001f);
    decaySmooth.reset(sampleRate, 0.001f);
    preDelaySmooth.reset(sampleRate, 0.001f);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = 1;
    preDelayL.prepare(spec);
    
    firstFIRL.prepare(spec);
    firstIIRL.prepare(spec);
    secondFIRL.prepare(spec);
    secondIIRL.prepare(spec);
    thirdFIRL.prepare(spec);
    thirdIIRL.prepare(spec);
    fourthFIRL.prepare(spec);
    fourthIIRL.prepare(spec);
    
    fifthIIRL.prepare(spec);
    sixthIIRL.prepare(spec);
    seventhIIRL.prepare(spec);
    eightIIRL.prepare(spec);
    
    firstalignL.prepare(spec);
    secondalignL.prepare(spec);
    thirdalignL.prepare(spec);
    fourthalignL.prepare(spec);
    
    
    feedbackL.prepare(spec);
    feedbacktwoL.prepare(spec);
    preDelayR.prepare(spec);
    firstFIRR.prepare(spec);
    firstIIRR.prepare(spec);
    secondFIRR.prepare(spec);
    secondIIRR.prepare(spec);
    thirdFIRR.prepare(spec);
    thirdIIRR.prepare(spec);
    fourthFIRR.prepare(spec);
    fourthIIRR.prepare(spec);
    
    
    fifthIIRR.prepare(spec);
    sixthIIRR.prepare(spec);
    seventhIIRR.prepare(spec);
    eightIIRR.prepare(spec);
    
   
    firstalignR.prepare(spec);
    secondalignR.prepare(spec);
    thirdalignR.prepare(spec);
    fourthalignR.prepare(spec);
    
    feedbackR.prepare(spec);
    feedbacktwoR.prepare(spec);
    
}

void ReverberZenAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverberZenAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ReverberZenAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);
    auto* outLeft = buffer.getWritePointer(0);
    auto* outRight = buffer.getWritePointer(1);
    reverbSmooth.setTargetValue(apvts.getRawParameterValue("Reverb")->load());
    eRSmooth.setTargetValue(apvts.getRawParameterValue("ER")->load());
    wetSmooth.setTargetValue(apvts.getRawParameterValue("Wet")->load());
    decaySmooth.setTargetValue(apvts.getRawParameterValue("Decay")->load());
    preDelaySmooth.setTargetValue(apvts.getRawParameterValue("PreDelay")->load());
    for(int i = 0; i < buffer.getNumSamples(); ++i)
    {
        // preDelay Left
        float offset = 1.0 - wetSmooth.getNextValue();
        float inputL = preDelayL.popSample(0);
        preDelayL.pushSample(0, leftChannel[i]);
        preDelayL.setDelay(getSampleRate() * 0.001f *preDelaySmooth.getNextValue());
        
        
        //preDelay Right
        float inputR  = preDelayR.popSample(0);
        preDelayR.pushSample(0, rightChannel[i]);
        preDelayR.setDelay(getSampleRate() * 0.001f *preDelaySmooth.getNextValue());
        
        float mid = (inputL + inputR) * 0.5f;
        float side = (inputL - inputR) * 0.5f;
        
        float alignoneL = mid + firstalignL.popSample(0);
        firstalignL.pushSample(0, side);
        firstalignL.setDelay(40.f * getSampleRate()*0.001f * reverbSmooth.getNextValue());
        float aligntwoL = mid + secondalignL.popSample(0);
        secondalignL.pushSample(0, side);
        secondalignL.setDelay(50.f * getSampleRate()*0.001f * reverbSmooth.getNextValue());
        float alignthreeL = mid + thirdalignL.popSample(0);
        thirdalignL.pushSample(0, side);
        thirdalignL.setDelay(60.f * getSampleRate()*0.001f * reverbSmooth.getNextValue());
        float alignfourL = mid + fourthalignL.popSample(0);
        fourthalignL.pushSample(0, side);
        fourthalignL.setDelay(70.f * getSampleRate()*0.001f * reverbSmooth.getNextValue());
        
        
        firstFIRL.pushSample(0, alignoneL);
        firstFIRL.setDelay(100.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        secondFIRL.pushSample(0, aligntwoL);
        secondFIRL.setDelay(80.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        thirdFIRL.pushSample(0, alignthreeL);
        thirdFIRL.setDelay(90.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        fourthFIRL.pushSample(0, alignfourL);
        fourthFIRL.setDelay(110.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float firstallpassL = firstFIRL.popSample(0) * 0.25f + firstIIRL.popSample(0) * - 0.75f;
        firstIIRL.pushSample(0, firstallpassL);
        firstIIRL.setDelay(100.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float secondallpassL = secondFIRL.popSample(0) * 0.35f + secondIIRL.popSample(0) * - 0.65f;
        secondIIRL.pushSample(0, secondallpassL);
        secondIIRL.setDelay(80.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float thirdallpassL = thirdFIRL.popSample(0) * 0.15f + thirdIIRL.popSample(0) * -0.85f;
        thirdIIRL.pushSample(0, thirdallpassL);
        thirdIIRL.setDelay(90.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float fourthallpassL = fourthFIRL.popSample(0) * 0.45f + fourthIIRL.popSample(0) * -0.55f;
        fourthIIRL.pushSample(0, fourthallpassL);
        fourthIIRL.setDelay(110.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        
        float feedbackoneL = firstallpassL + fifthIIRL.popSample(0) * 0.85f * decaySmooth.getNextValue();
        fifthIIRL.pushSample(0, feedbackoneL);
        fifthIIRL.setDelay(100.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float feedbacktwoL = secondallpassL + sixthIIRL.popSample(0) * 0.85f * decaySmooth.getNextValue();
        sixthIIRL.pushSample(0, feedbacktwoL);
        sixthIIRL.setDelay(90.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float feedbackthreeL = thirdallpassL + seventhIIRL.popSample(0) * 0.85f * decaySmooth.getNextValue();
        seventhIIRL.pushSample(0, feedbackthreeL);
        seventhIIRL.setDelay(80.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float feedbackfourL = fourthallpassL + eightIIRL.popSample(0) * 0.85f * decaySmooth.getNextValue();
        eightIIRL.pushSample(0, feedbackfourL);
        eightIIRL.setDelay(70.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        
        
        float alignoneR = mid - firstalignR.popSample(0);
        firstalignR.pushSample(0, side);
        firstalignR.setDelay(40.f * getSampleRate()*0.001f * reverbSmooth.getNextValue());
        float aligntwoR = mid - secondalignR.popSample(0);
        secondalignR.pushSample(0, side);
        secondalignR.setDelay(50.f * getSampleRate()*0.001f * reverbSmooth.getNextValue());
        float alignthreeR = mid - thirdalignR.popSample(0);
        thirdalignR.pushSample(0, side);
        thirdalignR.setDelay(60.f * getSampleRate()*0.001f * reverbSmooth.getNextValue());
        float alignfourR = mid - fourthalignR.popSample(0);
        fourthalignR.pushSample(0, side);
        fourthalignR.setDelay(70.f * getSampleRate()*0.001f * reverbSmooth.getNextValue());
        
        firstFIRR.pushSample(0, alignoneR);
        firstFIRR.setDelay(100.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        secondFIRR.pushSample(0, aligntwoR);
        secondFIRR.setDelay(80.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        thirdFIRR.pushSample(0, alignthreeR);
        thirdFIRR.setDelay(90.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        fourthFIRL.pushSample(0, alignfourR);
        fourthFIRL.setDelay(110.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float firstallpassR = firstFIRR.popSample(0) * 0.25f + firstIIRR.popSample(0) * - 0.75f;
        firstIIRR.pushSample(0, firstallpassR);
        firstIIRR.setDelay(100.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float secondallpassR = secondFIRR.popSample(0) * 0.35f + secondIIRR.popSample(0) * - 0.65f;
        secondIIRR.pushSample(0, secondallpassR);
        secondIIRR.setDelay(80.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float thirdallpassR = thirdFIRR.popSample(0) * 0.15f + thirdIIRR.popSample(0) * -0.85f;
        thirdIIRR.pushSample(0, thirdallpassR);
        thirdIIRR.setDelay(90.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float fourthallpassR = fourthFIRL.popSample(0) * 0.45f + fourthIIRR.popSample(0) * -0.55f;
        fourthIIRR.pushSample(0, fourthallpassR);
        fourthIIRR.setDelay(110.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float feedbackoneR = firstallpassR + fifthIIRR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        fifthIIRR.pushSample(0, feedbackoneR);
        fifthIIRR.setDelay(100.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float feedbacktwoR = secondallpassR + sixthIIRR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        sixthIIRR.pushSample(0, feedbacktwoR);
        sixthIIRR.setDelay(90.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float feedbackthreeR = thirdallpassR + seventhIIRR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        seventhIIRR.pushSample(0, feedbackthreeR);
        seventhIIRR.setDelay(80.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        float feedbackfourR = fourthallpassR + eightIIRR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        eightIIRR.pushSample(0, feedbackfourR);
        eightIIRR.setDelay(70.f * getSampleRate() * 0.001f * reverbSmooth.getNextValue());
        
        
        float earlyReflectionsL = firstallpassL + secondallpassL + thirdallpassL + fourthallpassL;
        float earlyReflectionsR = firstallpassR + secondallpassR + thirdallpassR + fourthallpassR;
        
        
        
        outLeft[i] =leftChannel[i] * offset + (earlyReflectionsL * eRSmooth.getNextValue() + feedbackoneL + feedbacktwoL + feedbackthreeL + feedbackfourL) * wetSmooth.getNextValue();
        outRight[i] =rightChannel[i] * offset + (earlyReflectionsR * eRSmooth.getNextValue() + feedbackoneR + feedbacktwoR + feedbackthreeR + feedbackfourR) * wetSmooth.getNextValue();
        
    }
}

//==============================================================================
bool ReverberZenAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverberZenAudioProcessor::createEditor()
{
    return new ReverberZenAudioProcessorEditor (*this);
}

//==============================================================================
void ReverberZenAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
              apvts.state.writeToStream(mos);
}

void ReverberZenAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
                  if(tree.isValid() )
                  {
                      apvts.replaceState(tree);
                  }
}

juce::AudioProcessorValueTreeState::ParameterLayout
ReverberZenAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("ER",
                                                          "ER",0.00f,1.f,0.25f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Reverb",
                                                          "Reverb",0.01f,1.f,0.65f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Wet",
                                                          "Wet",0.f,1.f,0.6f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Decay",
                                                          "Decay",0.0f,1.f,0.3f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PreDelay",
                                                          "PreDelay",1.f,25.f,3.f));
    
    return layout;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverberZenAudioProcessor();
}
