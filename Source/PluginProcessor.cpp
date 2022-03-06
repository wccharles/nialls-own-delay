/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ModDelayAudioProcessor::ModDelayAudioProcessor() :
    AudioProcessor(BusesProperties()
                       .withInput("Input", AudioChannelSet::stereo(), true)
                       .withOutput("Output", AudioChannelSet::stereo(), true)),
    m_params(*this),
    m_delay(m_params)
{
}

ModDelayAudioProcessor::~ModDelayAudioProcessor()
{
}

//==============================================================================

void ModDelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = getTotalNumOutputChannels();

    m_delay.prepare(spec);
    m_delay.updateParams();
}

void ModDelayAudioProcessor::releaseResources()
{
}

void ModDelayAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto              totalNumInputChannels = getTotalNumInputChannels();
    auto              totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> block(buffer);

    int subBlockPosition = 0;

    while (subBlockPosition < buffer.getNumSamples())
    {
        const auto                   subBlockSize = buffer.getNumSamples() - subBlockPosition;
        juce::dsp::AudioBlock<float> subBlock = block.getSubBlock((size_t) subBlockPosition, static_cast<size_t>(subBlockSize));

        juce::dsp::ProcessContextReplacing<float> context(subBlock);

        m_delay.process(context);
        m_delay.updateParams();

        subBlockPosition += subBlockSize;
    }

    ignoreUnused(midiMessages);
}

//==============================================================================

AudioProcessorEditor* ModDelayAudioProcessor::createEditor()
{
    return new ModDelayAudioProcessorEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ModDelayAudioProcessor();
}
