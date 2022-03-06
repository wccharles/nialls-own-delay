/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ModDelayAudioProcessor::ModDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties().withInput  ("Input",  AudioChannelSet::stereo(), true)
                                        .withOutput ("Output", AudioChannelSet::stereo(), true))
#endif
{
}

ModDelayAudioProcessor::~ModDelayAudioProcessor()
{
}

//==============================================================================

void ModDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // _delay = std::make_unique<Delay>(getTotalNumOutputChannels(), sampleRate);
    // _delay->prepareToPlay(0);
    // adsr.setSampleRate(sampleRate);
    // adsr.setParameters({2.0f, 0.5f, 0.5, 0.1f});
}

void ModDelayAudioProcessor::releaseResources()
{
}

void ModDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // _delay->process(buffer);
    // adsr.setParameters({2.0f, 0.5f, 0.5, 0.5f});
    // adsr.applyEnvelopeToBuffer(buffer, 0, buffer.getNumSamples());
}

//==============================================================================

AudioProcessorEditor* ModDelayAudioProcessor::createEditor()
{
    return new ModDelayAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ModDelayAudioProcessor();
}
