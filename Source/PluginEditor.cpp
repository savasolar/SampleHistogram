/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SampleHistogramAudioProcessorEditor::SampleHistogramAudioProcessorEditor (SampleHistogramAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    Timer::startTimerHz(120);
    setSize (500, 300);
    setResizable(false, false);
}

SampleHistogramAudioProcessorEditor::~SampleHistogramAudioProcessorEditor()
{
    Timer::stopTimer();
}

//==============================================================================
void SampleHistogramAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    paintHistogram(g);
}

void SampleHistogramAudioProcessorEditor::resized()
{

}

void SampleHistogramAudioProcessorEditor::timerCallback()
{
    repaint();
}

void SampleHistogramAudioProcessorEditor::paintHistogram(juce::Graphics& g)
{
    //painting the histogram
    int ampHeight = static_cast<int>(audioProcessor.mAmplitude * 300);

    for (int i = 0; i < 499; i++)
    {
        rectArray[i] = rectArray[i + 1];
    }
    rectArray[499] = ampHeight;

    for (int i = 0; i < 500; i++)
    {
        g.setColour(juce::Colours::lightblue);
        g.fillRect(i, 300 - rectArray[i], 1, rectArray[i]);
    }
}