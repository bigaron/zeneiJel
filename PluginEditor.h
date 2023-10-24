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
class _11BandEqAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _11BandEqAudioProcessorEditor (_11BandEqAudioProcessor&);
    ~_11BandEqAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _11BandEqAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_11BandEqAudioProcessorEditor)
};
