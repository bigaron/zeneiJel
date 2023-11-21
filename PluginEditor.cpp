/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_11BandEqAudioProcessorEditor::_11BandEqAudioProcessorEditor(_11BandEqAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    sliders = std::vector<juce::Slider>(10);
    int i = 0;

    for (auto& slider : sliders) {
        slider.setSliderStyle(juce::Slider::LinearBarVertical);
        slider.setRange(-18.0f, 18.0f, 1.0f);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
        slider.setPopupDisplayEnabled(true, true, this);
        slider.setTextValueSuffix(" Gain");
        slider.setValue(audioProcessor.filters[i].getGain());
        ++i;
    }

    setSize(1280, 720);


    for (auto& slider : sliders) {
        slider.addListener(this);
        addAndMakeVisible(slider);
    }
}

void _11BandEqAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    int val = slider->getX();
    int offset = 180, endX = 980;
    if (val == offset)  audioProcessor.filters[0].setGain(slider->getValue());
    else if (val == offset + 1 * endX / sliders.size()) audioProcessor.filters[1].setGain(slider->getValue());
    else if (val == offset + 2 * endX / sliders.size()) audioProcessor.filters[2].setGain(slider->getValue());
    else if (val == offset + 3 * endX / sliders.size()) audioProcessor.filters[3].setGain(slider->getValue());
    else if (val == offset + 4 * endX / sliders.size()) 
        audioProcessor.filters[4].setGain(slider->getValue());
    else if (val == offset + 5 * endX / sliders.size()) audioProcessor.filters[5].setGain(slider->getValue());
    else if (val == offset + 6 * endX / sliders.size()) audioProcessor.filters[6].setGain(slider->getValue());
    else if (val == offset + 7 * endX / sliders.size()) audioProcessor.filters[7].setGain(slider->getValue());
    else if (val == offset + 8 * endX / sliders.size()) audioProcessor.filters[8].setGain(slider->getValue());
    else if (val == offset + 9 * endX / sliders.size()) audioProcessor.filters[9].setGain(slider->getValue());

}

_11BandEqAudioProcessorEditor::~_11BandEqAudioProcessorEditor()
{
}

//==============================================================================
void _11BandEqAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::white);

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Octave equalizer by bigaron", getLocalBounds(), juce::Justification::centredTop, 1);
}

void _11BandEqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    for (int i = 0; i < sliders.size(); ++i) sliders[i].setBounds(i * (980 / sliders.size()) + 180, 150, 20, 500);
    //sliders[0].setBounds(230, 30, 20, 500);
}
