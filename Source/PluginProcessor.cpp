/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SampleHistogramAudioProcessor::SampleHistogramAudioProcessor()
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

SampleHistogramAudioProcessor::~SampleHistogramAudioProcessor()
{
}

//==============================================================================
const juce::String SampleHistogramAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SampleHistogramAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SampleHistogramAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SampleHistogramAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SampleHistogramAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SampleHistogramAudioProcessor::getNumPrograms()
{
    return 1;
}

int SampleHistogramAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SampleHistogramAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SampleHistogramAudioProcessor::getProgramName (int index)
{
    return {};
}

void SampleHistogramAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SampleHistogramAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void SampleHistogramAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SampleHistogramAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;


   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SampleHistogramAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            //building a little buffer of 256 samples, taking the highest peak of that buffer and assigning its value to mAmplitude
            float singleSample = std::abs(buffer.getSample(channel, sample));

            if (singleSample > mMaxPeak)
            {
                mMaxPeak = singleSample;
            }

            ++mSampleCounter;

            if (mSampleCounter == mDetectionLength)
            {
                mAmplitude = mMaxPeak;
                mMaxPeak = 0.0;
                mSampleCounter = 0;
            }
        }
    }
}

//==============================================================================
bool SampleHistogramAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SampleHistogramAudioProcessor::createEditor()
{
    return new SampleHistogramAudioProcessorEditor (*this);
}

//==============================================================================
void SampleHistogramAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void SampleHistogramAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

//==============================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SampleHistogramAudioProcessor();
}
