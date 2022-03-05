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
    fifthFIRL.prepare(spec);
    fifthIIRL.prepare(spec);
    sixthFIRL.prepare(spec);
    sixthIIRL.prepare(spec);
    seventhFIRL.prepare(spec);
    seventhIIRL.prepare(spec);
    eightFIRL.prepare(spec);
    eightIIRL.prepare(spec);
    
    firstalignL.prepare(spec);
    secondalignL.prepare(spec);
    thirdalignL.prepare(spec);
    fourthalignL.prepare(spec);
    fifthalignL.prepare(spec);
    sixthalignL.prepare(spec);
    seventhalignL.prepare(spec);
    eightalignL.prepare(spec);
    
    
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
    
    fifthFIRR.prepare(spec);
    fifthIIRR.prepare(spec);
    sixthFIRR.prepare(spec);
    sixthIIRR.prepare(spec);
    seventhFIRR.prepare(spec);
    seventhIIRR.prepare(spec);
    eightFIRR.prepare(spec);
    eightIIRR.prepare(spec);
    
   
    firstalignR.prepare(spec);
    secondalignR.prepare(spec);
    thirdalignR.prepare(spec);
    fourthalignR.prepare(spec);
    fifthalignR.prepare(spec);
    sixthalignR.prepare(spec);
    seventhalignR.prepare(spec);
    eightalignR.prepare(spec);
    
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
        preDelayL.setDelay(getSampleRate()/1000.f *preDelaySmooth.getNextValue());
        
        
        
        //preDelay Right
        float inputR  = preDelayR.popSample(0);
        preDelayR.pushSample(0, rightChannel[i]);
        preDelayR.setDelay(getSampleRate()/1000.f *preDelaySmooth.getNextValue());
        
    
        //feedforward Left
        firstFIRL.pushSample(0, inputL);
        firstFIRL.setDelay(75.f * getSampleRate()/1000.f * reverbSmooth.getNextValue() +10.f);
        secondFIRL.pushSample(0, inputL);
        secondFIRL.setDelay(50.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() +10.f);
        thirdFIRL.pushSample(0, inputL);
        thirdFIRL.setDelay(90.f * getSampleRate()/1000.f* reverbSmooth.getNextValue()+ 10.f);
        fourthFIRL.pushSample(0, inputL);
        fourthFIRL.setDelay(50.f * getSampleRate()/1000.f* reverbSmooth.getNextValue() +10.f);
        fifthFIRL.pushSample(0, inputL);
        fifthFIRL.setDelay(40.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        sixthFIRL.pushSample(0, inputL);
        sixthFIRL.setDelay(100.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        seventhFIRL.pushSample(0, inputL);
        seventhFIRL.setDelay(40.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        eightFIRL.pushSample(0, inputL);
        eightFIRL.setDelay(110.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        
        
        
        //Left Delay Processing + feedbackcoefficients
        float firstallpassL = firstFIRL.popSample(0) * 0.5f + firstIIRL.popSample(0) * -0.5f;
        firstIIRL.pushSample(0, firstallpassL) ;
        firstIIRL.setDelay(75.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float secondallpassL = secondFIRL.popSample(0) * 0.25f + secondIIRL.popSample(0) * -0.75f;
        secondIIRL.pushSample(0,secondallpassL);
        secondIIRL.setDelay(50.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        float thirdallpassL = thirdFIRL.popSample(0) * 0.75f + thirdIIRL.popSample(0) * -0.25f;
        thirdIIRL.pushSample(0, thirdallpassL);
        thirdIIRL.setDelay(90.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        float fourthallpassL = fourthFIRL.popSample(0) * 0.15f + fourthIIRL.popSample(0) * -0.85f;
        fourthIIRL.pushSample(0, fourthallpassL);
        fourthIIRL.setDelay(50.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float fifthallpassL = fifthFIRL.popSample(0) * 0.2f + fifthIIRL.popSample(0)* -0.8f;
        fifthIIRL.pushSample(0, fifthallpassL);
        fifthIIRL.setDelay(40.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float sixthallpassL = sixthFIRL.popSample(0) * 0.15f + sixthIIRL.popSample(0)* -0.85f;
        sixthIIRL.pushSample(0, sixthallpassL);
        sixthIIRL.setDelay(100.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float seventhallpassL = seventhFIRL.popSample(0) * 0.5f + seventhIIRL.popSample(0)* -0.5f;
        seventhIIRL.pushSample(0, seventhallpassL);
        seventhIIRL.setDelay(40.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float eighthallpassL = eightFIRL.popSample(0) * 0.15f + eightIIRL.popSample(0)* -0.85f;
        eightIIRL.pushSample(0, eighthallpassL);
        eightIIRL.setDelay(110.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        //aligment Left
        
        float alignoneL = firstallpassL + firstalignL.popSample(0) * 0.85f * decaySmooth.getNextValue();
        float aligntwoL =  secondallpassL + secondalignL.popSample(0)  * 0.85f * decaySmooth.getNextValue();
        float alignthirdL = thirdallpassL + thirdalignL.popSample(0)  * 0.85f * decaySmooth.getNextValue();
        float alignfourthL = fourthallpassL + fourthalignL.popSample(0)  * 0.85f * decaySmooth.getNextValue();
        float alignfifthL = fifthallpassL + fifthalignL.popSample(0) * 0.85f * decaySmooth.getNextValue();
        float alignsixthL = sixthallpassL + sixthalignL.popSample(0)  * 0.85f * decaySmooth.getNextValue();
        float alignseventhL = seventhallpassL + seventhalignL.popSample(0) * 0.85f * decaySmooth.getNextValue();
        float aligneigthL = eighthallpassL + eightalignL.popSample(0)  * 0.85f * decaySmooth.getNextValue();
        firstalignL.pushSample(0, alignoneL);
        firstalignL.setDelay(40.f* reverbSmooth.getNextValue());
        secondalignL.pushSample(0, aligntwoL);
        secondalignL.setDelay(50.f* reverbSmooth.getNextValue());
        thirdalignL.pushSample(0, alignthirdL);
        thirdalignL.setDelay(60.f* reverbSmooth.getNextValue());
        fourthalignL.pushSample(0, alignfourthL);
        fourthalignL.setDelay(70.f* reverbSmooth.getNextValue());
        fifthalignL.pushSample(0, alignfifthL);
        fifthalignL.setDelay(80.f * reverbSmooth.getNextValue());
        sixthalignL.pushSample(0, alignsixthL);
        sixthalignL.setDelay(90.f* reverbSmooth.getNextValue());
        seventhalignL.pushSample(0, alignseventhL);
        seventhalignL.setDelay(70.f* reverbSmooth.getNextValue());
        eightalignL.pushSample(0, aligneigthL);
        eightalignL.setDelay(80.f * reverbSmooth.getNextValue());
        //feedforward Right
        firstFIRR.pushSample(0, inputR);
        firstFIRR.setDelay(75.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() +10.f);
        secondFIRR.pushSample(0, inputR);
        secondFIRR.setDelay(50.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() +10.f);
        thirdFIRR.pushSample(0, inputR);
        thirdFIRR.setDelay(90.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        fourthFIRR.pushSample(0, inputR);
        fourthFIRR.setDelay(50.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() +10.f);
        fifthFIRR.pushSample(0, inputR);
        fifthFIRR.setDelay(40.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        sixthFIRR.pushSample(0, inputR);
        sixthFIRR.setDelay(100.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        seventhFIRR.pushSample(0, inputR);
        seventhFIRR.setDelay(40.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        eightFIRR.pushSample(0, inputR);
        eightFIRR.setDelay(110.f* getSampleRate()/1000.f * reverbSmooth.getNextValue() + 10.f);
        //Right Delay Processing + feedbackcoefficients
        float firstallpassR = firstFIRR.popSample(0) * 0.5f + firstIIRR.popSample(0) * -0.5f;
        firstIIRR.pushSample(0, firstallpassR);
        firstIIRR.setDelay(75.f * getSampleRate()/1000.f* reverbSmooth.getNextValue()+ 10.f);
        float secondallpassR = secondFIRR.popSample(0) * 0.25f + secondIIRR.popSample(0) * -0.75f;
        secondIIRR.pushSample(0,secondallpassR);
        secondIIRR.setDelay(50.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float thirdallpassR = thirdFIRR.popSample(0) * 0.75f + thirdIIRR.popSample(0) * -0.25f;
        thirdIIRR.pushSample(0, thirdallpassR);
        thirdIIRR.setDelay(90.f * getSampleRate()/1000.f* reverbSmooth.getNextValue() +10.f);
        float fourthallpassR = fourthFIRR.popSample(0) * 0.15f + fourthIIRR.popSample(0) * -0.85f;
        fourthIIRR.pushSample(0, fourthallpassR);
        fourthIIRR.setDelay(50.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float fifthallpassR = fifthFIRR.popSample(0) * 0.2f + fifthIIRR.popSample(0)* -0.8f;
        fifthIIRR.pushSample(0, fifthallpassR);
        fifthIIRR.setDelay(40.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float sixthallpassR = sixthFIRR.popSample(0) * 0.15f + sixthIIRR.popSample(0)* -0.85f;
        sixthIIRR.pushSample(0, sixthallpassR);
        sixthIIRR.setDelay(100.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float seventhallpassR = seventhFIRR.popSample(0) * 0.5f + seventhIIRR.popSample(0)* -0.5f;
        seventhIIRR.pushSample(0, seventhallpassR);
        seventhIIRR.setDelay(40.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        float eighthallpassR = eightFIRR.popSample(0) * 0.15f + eightIIRR.popSample(0)* -0.85f;
        eightIIRR.pushSample(0, eighthallpassR);
        eightIIRR.setDelay(110.f* getSampleRate()/1000.f * reverbSmooth.getNextValue()+ 10.f);
        //aligment Right
        
        float alignoneR = firstallpassR + firstalignR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        float aligntwoR = secondallpassR + secondalignR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        float alignthirdR = thirdallpassR + thirdalignR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        float alignfourthR = fourthallpassR + fourthalignR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        float alignfifthR = fifthallpassR + fifthalignR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        float alignsixthR = sixthallpassR + sixthalignR.popSample(0)  * 0.85f * decaySmooth.getNextValue();
        float alignseventhR = seventhallpassR + seventhalignR.popSample(0) * 0.85f * decaySmooth.getNextValue();
        float aligneigthR = eighthallpassR + eightalignR.popSample(0)  * 0.85f * decaySmooth.getNextValue();
        firstalignR.pushSample(0, alignoneR);
        firstalignR.setDelay(70.f* getSampleRate()/1000.f * reverbSmooth.getNextValue());
        secondalignR.pushSample(0, aligntwoR);
        secondalignR.setDelay(50.f * getSampleRate()/1000.f* reverbSmooth.getNextValue());
        thirdalignR.pushSample(0, alignthirdR);
        thirdalignR.setDelay(90.f * getSampleRate()/1000.f* reverbSmooth.getNextValue());
        fourthalignR.pushSample(0, alignfourthR);
        fourthalignR.setDelay(50.f* getSampleRate()/1000.f * reverbSmooth.getNextValue());
        fifthalignR.pushSample(0, alignfifthR);
        fifthalignR.setDelay(80.f * reverbSmooth.getNextValue());
        sixthalignR.pushSample(0, alignsixthR);
        sixthalignR.setDelay(90.f* reverbSmooth.getNextValue());
        seventhalignR.pushSample(0, alignseventhR);
        seventhalignR.setDelay(70.f* reverbSmooth.getNextValue());
        eightalignR.pushSample(0, aligneigthR);
        eightalignR.setDelay(80.f * reverbSmooth.getNextValue());
        
        
        
        float earlyReflectionsL = (firstallpassL + secondallpassL + thirdallpassL + fourthallpassL + fifthallpassL + sixthallpassL + seventhallpassL + eighthallpassL) * eRSmooth.getNextValue();
        float earlyReflectionsR = (firstallpassR + secondallpassR + thirdallpassR + fourthallpassR+ fifthallpassR + sixthallpassR + seventhallpassR + eighthallpassR) * eRSmooth.getNextValue();

        
        float sumL = alignoneL + aligntwoL - alignthirdL - alignfourthL + alignfifthL + alignsixthL - alignseventhL - aligneigthL + feedbackL.popSample(0) * 0.85 * decaySmooth.getNextValue();
        feedbackL.pushSample(0, sumL);
        feedbackL.setDelay(300.f* getSampleRate()/1000.f*reverbSmooth.getNextValue() + 50.f);
        float sumLtwo = leftChannel[i] * offset + (sumL +earlyReflectionsL + feedbacktwoL.popSample(0) * 0.85f * decaySmooth.getNextValue()) * wetSmooth.getNextValue() ;
        feedbacktwoL.pushSample(0, sumLtwo);
        feedbacktwoL.setDelay(200.f* getSampleRate()/1000.f*reverbSmooth.getNextValue() + 50.f);
        outLeft[i] = sumLtwo;
      // outRight[i] = alignoneR + aligntwoR+ alignthirdR + alignfourthR;
       float sumR = alignoneR + aligntwoR - alignthirdR - alignfourthR + alignsixthR - alignseventhR - aligneigthR +  feedbackR.popSample(0) * 0.85 * decaySmooth.getNextValue();
       feedbackR.pushSample(0, sumR);
        feedbackR.setDelay(300.f* getSampleRate()/1000.f*reverbSmooth.getNextValue() + 50.f);
       float sumRtwo = rightChannel[i] * offset + (sumR + earlyReflectionsR + feedbacktwoR.popSample(0) * 0.85f * decaySmooth.getNextValue()) * wetSmooth.getNextValue();
        feedbacktwoR.pushSample(0, sumRtwo);
        feedbacktwoR.setDelay(200.f* getSampleRate()/1000.f*reverbSmooth.getNextValue() + 50.f);
        outRight[i] = sumRtwo;
        
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
                                                          "PreDelay",0.01f,25.f,0.3f));
    
    return layout;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverberZenAudioProcessor();
}
