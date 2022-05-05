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
    initDSP();
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
    
    reverbSmooth.reset(sampleRate, 0.001f);
    preDelaySmooth.reset(sampleRate, 0.001f);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    for (int i = 0; i < 2; i++) {
        predelay[i]->delay[i]->prepare(spec);
        
        
        
        delayOne[i]->Fbdelay[i]->prepare(spec);
        delayOne[i]->Fbdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayOne[i]->Ffdelay[i]->prepare(spec);
        delayOne[i]->Ffdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayOne[i]->delay[i]->prepare(spec);
        delayOne[i]->feedback[i]->prepare(spec);
        
        delayTwo[i]->Fbdelay[i]->prepare(spec);
        delayTwo[i]->Fbdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayTwo[i]->Ffdelay[i]->prepare(spec);
        delayTwo[i]->Ffdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayTwo[i]->delay[i]->prepare(spec);
        delayTwo[i]->feedback[i]->prepare(spec);
        
        delayThree[i]->Fbdelay[i]->prepare(spec);
        delayThree[i]->Fbdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayThree[i]->Ffdelay[i]->prepare(spec);
        delayThree[i]->Ffdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayThree[i]->delay[i]->prepare(spec);
        delayThree[i]->feedback[i]->prepare(spec);
        
        delayFour[i]->Fbdelay[i]->prepare(spec);
        delayFour[i]->Fbdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayFour[i]->Ffdelay[i]->prepare(spec);
        delayFour[i]->Ffdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayFour[i]->delay[i]->prepare(spec);
        delayFour[i]->feedback[i]->prepare(spec);
        
        delayFive[i]->Fbdelay[i]->prepare(spec);
        delayFive[i]->Fbdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayFive[i]->Ffdelay[i]->prepare(spec);
        delayFive[i]->Ffdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayFive[i]->delay[i]->prepare(spec);
        delayFive[i]->feedback[i]->prepare(spec);
        
        delaySix[i]->Fbdelay[i]->prepare(spec);
        delaySix[i]->Fbdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delaySix[i]->Ffdelay[i]->prepare(spec);
        delaySix[i]->Ffdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delaySix[i]->delay[i]->prepare(spec);
        delaySix[i]->feedback[i]->prepare(spec);
        
        delaySeven[i]->Fbdelay[i]->prepare(spec);
        delaySeven[i]->Fbdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delaySeven[i]->Ffdelay[i]->prepare(spec);
        delaySeven[i]->Ffdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delaySeven[i]->delay[i]->prepare(spec);
        delaySeven[i]->feedback[i]->prepare(spec);
        
        delayEight[i]->Fbdelay[i]->prepare(spec);
        delayEight[i]->Fbdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayEight[i]->Ffdelay[i]->prepare(spec);
        delayEight[i]->Ffdelay[i]->setMaximumDelayInSamples(sampleRate * 0.001f * 250);
        delayEight[i]->delay[i]->prepare(spec);
        delayEight[i]->feedback[i]->prepare(spec);
        
        earlyGain[i]->gainParameter[i]->reset(sampleRate, 0.001f);
        feedbackGain[i]->gainParameter[i]->reset(sampleRate, 0.001f);
        wetGain[i]->gainParameter[i]->reset(sampleRate, 0.001f);
    }
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
    
    reverbSmooth.setTargetValue(apvts.getRawParameterValue("Reverb")->load());
    preDelaySmooth.setTargetValue(apvts.getRawParameterValue("PreDelay")->load());
    
    float millis = getSampleRate() * 0.001f;
    for(int chans = 0; chans < 2; chans++){
        auto* inputs = buffer.getWritePointer(chans);
        auto* outputs = buffer.getWritePointer(chans);
        feedbackGain[chans]->gainParameter[chans]->setTargetValue(apvts.getRawParameterValue("Decay")->load());
        earlyGain[chans]->gainParameter[chans]->setTargetValue(apvts.getRawParameterValue("ER")->load());
        wetGain[chans]->gainParameter[chans]->setTargetValue(apvts.getRawParameterValue("Wet")->load());
        for(int i = 0; i < buffer.getNumSamples(); ++i)
        {
            
        // preDelay + filters
            
            float predelayed = predelay[chans]->simpledelay(inputs[i], preDelaySmooth.getNextValue(), chans);
            // bessel 2nd order critical freq 10000 hz
            delayOne[chans]->saveStage1(lopFilterOne[chans]->process(delayOne[chans]->simpledelay(predelayed, reverbSmooth.getNextValue() * millis * 100 + 10,chans),
                                                                     0.2300941390717937, 0.4601882781435874,0.2300941390717937, -0.15674135277785778, 0.07711790906503295),chans);
            // bessel 2nd order critical freq 9000 hz
            delayTwo[chans]->saveStage1(lopFilterTwo[chans]->process(delayTwo[chans]->simpledelay(predelayed, reverbSmooth.getNextValue() * millis * 120+ 20,chans),
                                                                     0.19545962366720693, 0.39091924733441386, 0.19545962366720693, -0.3109199123078439, 0.09275840697667159),chans);
            // bessel 2nd order critical freq 8500 hz
            delayThree[chans]->saveStage1(lopFilterThree[chans]->process(delayThree[chans]->simpledelay(predelayed, reverbSmooth.getNextValue() * millis * 110 + 30,chans),
                                                                         0.2125061648995768, 0.4250123297991536, 0.2125061648995768, -0.23359635829124112, 0.08362101788954816
                                                                         ),chans);
            // bessel 2nd order critical freq 8900 hz
            delayFour[chans]->saveStage1(lopFilterFour[chans]->process(delayFour[chans]->simpledelay(predelayed, reverbSmooth.getNextValue() * millis * 100 + 10,chans),
                                                                       0.1921135654731113, 0.3842271309462226, 0.1921135654731113,-0.3264547217702863, 0.09490898366273133),chans);
            // chebyshev 2nd order critical freq 10000 Hz Abp 3 db
            delayFive[chans]->saveStage1(lopFilterFive[chans]->process(delayFive[chans]->simpledelay(predelayed, reverbSmooth.getNextValue() * millis * 120+ 20,chans),
                                                                       0.17930052450956305, 0.3586010490191261, 0.17930052450956305, -0.45266529092814395, 0.46574018148938473),chans);
            //chebishev 2nd order critical freq 8666 Hz
            delaySix[chans]->saveStage1(lopFilterSix[chans]->process(delaySix[chans]->simpledelay(predelayed, reverbSmooth.getNextValue() * millis * 90+ 30,chans),
                                                                     0.21216149712876453,0.42432299425752906,0.21216149712876453, -0.2533139194858135, 0.45205824035682385  ),chans);
            //chebishev 2nd order critical freq 9500 Hz
           delaySeven[chans]->saveStage1(lopFilterSeven[chans]->process(delaySeven[chans]->simpledelay(predelayed, reverbSmooth.getNextValue() * millis * 110+ 10,chans),
                                                                         0.22948890460240615, 0.4589778092048123, 0.22948890460240615,-0.15144251975956102, 0.44808929506055417),chans);
            //chebishev 2nd order critical freq 9999 Hz 3 db
          delayEight[chans]->saveStage1(lopFilterEight[chans]->process(delayEight[chans]->simpledelay(predelayed, reverbSmooth.getNextValue() * millis * 120+ 20,chans),
                                                                       0.17926886075782145, 0.3585377215156429, 0.17926886075782145, -0.4528616131590244, 0.46575759876771083
                                                                    ),chans);
        
        //earlyreflections
        earlyGain[chans]->gainprocess(
            delayOne[chans]->saveStage2(delayOne[chans]->allpass(delayOne[chans]->returnStage1(chans), reverbSmooth.getNextValue() * millis * 50 + 10, .85, chans),chans)+
            delayTwo[chans]->saveStage2(delayTwo[chans]->allpass(delayTwo[chans]->returnStage1(chans), reverbSmooth.getNextValue() * millis * 30 + 5, .85, chans),chans)+
            delayThree[chans]->saveStage2(delayThree[chans]->allpass(delayThree[chans]->returnStage1(chans), reverbSmooth.getNextValue() * millis * 40 + 8, 0.55, chans),chans)+
            delayFour[chans]->saveStage2(delayFour[chans]->allpass(delayFour[chans]->returnStage1(chans), reverbSmooth.getNextValue() * millis * 50 + 16, 0.84, chans),chans)+
            delayFive[chans]->saveStage2(delayFive[chans]->allpass(delayFive[chans]->returnStage1(chans), reverbSmooth.getNextValue() * millis * 30 + 20,0.89,chans),chans)+
            delaySix[chans]->saveStage2(delaySix[chans]->allpass(delaySix[chans]->returnStage1(chans), reverbSmooth.getNextValue() * millis * 60 +10,0.7565,chans),chans)+
            delaySeven[chans]->saveStage2(delaySeven[chans]->allpass(delaySeven[chans]->returnStage1(chans), reverbSmooth.getNextValue() * millis * 40 + 8,0.89,chans),chans)+
            delayEight[chans]->saveStage2(delayEight[chans]->allpass(delayEight[chans]->returnStage1(chans), reverbSmooth.getNextValue() * millis * 30 + 5,0.932,chans),chans),chans);
        
        //feedback
            feedbackGain[chans]->gainprocess(
            delayOne[chans]->feedbackDelay(delayOne[chans]->returnStage2(chans), reverbSmooth.getNextValue() * millis * 122, 0.85,chans) +
            delayTwo[chans]->feedbackDelay( delayTwo[chans]->returnStage2(chans), reverbSmooth.getNextValue() * millis * 120, 0.75, chans)  +
            delayThree[chans]->feedbackDelay(delayThree[chans]->returnStage2(chans), reverbSmooth.getNextValue() * millis * 120, 0.85, chans) +
            delayFour[chans]->feedbackDelay(delayFour[chans]->returnStage2(chans), reverbSmooth.getNextValue() * millis * 130, 0.84, chans)  +
            delayFive[chans]->feedbackDelay(delayFive[chans]->returnStage2(chans), reverbSmooth.getNextValue() * millis * 140,0.75,chans)  +
            delaySix[chans]->feedbackDelay(delaySix[chans]->returnStage2(chans), reverbSmooth.getNextValue() * millis * 130,0.85,chans) +
            delaySeven[chans]->feedbackDelay(delaySeven[chans]->returnStage2(chans), reverbSmooth.getNextValue() * millis * 100,0.85,chans)  +
            delayEight[chans]->feedbackDelay(delayEight[chans]->returnStage2(chans), reverbSmooth.getNextValue() * millis * 110,0.75,chans),chans);
            
            outputs[i] = wetGain[chans]->offsetgianprocess(inputs[i], chans) + wetGain[chans]->gainprocess(earlyGain[chans]->returnstage(chans) + feedbackGain[chans]->returnstage(chans),chans);
            
        
        }
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
