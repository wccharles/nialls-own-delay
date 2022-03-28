/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
//#include "PluginEditor.h"

//==============================================================================
ModDelayAudioProcessor::ModDelayAudioProcessor() :
    AudioProcessor(BusesProperties()
                       .withInput("Input", AudioChannelSet::stereo(), true)
                       .withOutput("Output", AudioChannelSet::stereo(), true)),
    m_params(*this),
    m_delay(m_params),
    m_fdn(m_params)
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
    spec.maximumBlockSize = static_cast<uint32>(samplesPerBlock);
    spec.numChannels = static_cast<uint32>(getTotalNumOutputChannels());

    const auto stereoWidth = m_params.getValue(ModDelay::ParamID::StereoWidth);
    m_stereoProcessor.prepare(spec);
    m_stereoProcessor.updateParams(stereoWidth);

    m_delay.prepare(spec);
    m_delay.updateParams();

    m_fdn.prepare(spec);
    m_fdn.updateParams();

    m_gain.reset();
    m_gain.prepare(spec);
    m_gain.setRampDurationSeconds(ModDelay::SharedConstants::smoothedValueRamp);
    const auto gainLinear = m_params.getValue(ModDelay::ParamID::Gain);
    m_gain.setGainLinear(gainLinear);
}

void ModDelayAudioProcessor::releaseResources()
{
}

void ModDelayAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    dsp::AudioBlock<float> block(buffer);

    int subBlockPosition = 0;

    const auto stereoWidth = m_params.getValue(ModDelay::ParamID::StereoWidth);

    const auto gainLinear = m_params.getValue(ModDelay::ParamID::Gain);
    m_gain.setGainLinear(gainLinear);

    while (subBlockPosition < buffer.getNumSamples())
    {
        const auto                   subBlockSize = buffer.getNumSamples() - subBlockPosition;
        juce::dsp::AudioBlock<float> subBlock = block.getSubBlock((size_t) subBlockPosition, static_cast<size_t>(subBlockSize));

        juce::dsp::ProcessContextReplacing<float> context(subBlock);

        m_gain.process(context);
        m_fdn.process(context);
        m_fdn.updateParams();
        m_stereoProcessor.process(context);
        m_stereoProcessor.updateParams(stereoWidth);
        m_delay.process(context);
        m_delay.updateParams();

        subBlockPosition += subBlockSize;
    }

    ignoreUnused(midiMessages);
}

//==============================================================================

AudioProcessorEditor* ModDelayAudioProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ModDelayAudioProcessor();
}
