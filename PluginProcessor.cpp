/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_11BandEqAudioProcessor::_11BandEqAudioProcessor()
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
    filters = std::vector<Filter>(10);
}


_11BandEqAudioProcessor::~_11BandEqAudioProcessor()
{
}

//==============================================================================
const juce::String _11BandEqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _11BandEqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _11BandEqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _11BandEqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _11BandEqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _11BandEqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _11BandEqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _11BandEqAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _11BandEqAudioProcessor::getProgramName (int index)
{
    return {};
}

void _11BandEqAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _11BandEqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    filters[0] = Filter(sampleRate, 30, 31.25, 0);
    filters[1] = Filter(sampleRate, 31.25, 62.5, 0);
    filters[2] = Filter(sampleRate, 50, 125, 0);
    filters[3] = Filter(sampleRate, 75, 250, 0);
    filters[4] = Filter(sampleRate, 100, 500, 0);
    filters[5] = Filter(sampleRate, 500, 1000, 0);
    filters[6] = Filter(sampleRate, 1000, 2000, 0);
    filters[7] = Filter(sampleRate, 2000, 4000, 0);
    filters[8] = Filter(sampleRate, 4000, 8000, 0);
    filters[9] = Filter(sampleRate, 8000, 16000, 0);
}

void _11BandEqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _11BandEqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void _11BandEqAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
        
    filters[0].lowPassBuffer(buffer);

    for (int i = 0; i < filters.size(); ++i) {
        filters[i].processBuffer(buffer);
    }

    //filters[4].processBuffer(buffer);
   //eq1.process(buffer);
  // eq2.process(buffer);
}

//==============================================================================
bool _11BandEqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _11BandEqAudioProcessor::createEditor()
{
    return new _11BandEqAudioProcessorEditor (*this);
}

//==============================================================================
void _11BandEqAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void _11BandEqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _11BandEqAudioProcessor();
}
