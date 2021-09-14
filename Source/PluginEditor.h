/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SampleHistogramAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                             public juce::Timer
{
public:
    SampleHistogramAudioProcessorEditor (SampleHistogramAudioProcessor&);
    ~SampleHistogramAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

    void paintHistogram(juce::Graphics&);

private:
    int rectArray[500] = { 0 };

    SampleHistogramAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleHistogramAudioProcessorEditor)
};
