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
    m_delay(m_params)
{
}

ModDelayAudioProcessor::~ModDelayAudioProcessor()
{
    stopTimer();
}

//==============================================================================

void ModDelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<uint32>(samplesPerBlock);
    spec.numChannels = static_cast<uint32>(getTotalNumOutputChannels());

    m_delay.prepare(spec);
    m_delay.updateParams();

    m_gain.reset();
    m_gain.prepare(spec);
    m_gain.setRampDurationSeconds(0.005);
    const auto gainLinear = m_params.getValue(ModDelay::ParamID::Gain);
    m_gain.setGainLinear(gainLinear);
}

void ModDelayAudioProcessor::releaseResources()
{
}

void ModDelayAudioProcessor::timerCallback()
{
}

void ModDelayAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    dsp::AudioBlock<float> block(buffer);

    int subBlockPosition = 0;

    const auto gainLinear = m_params.getValue(ModDelay::ParamID::Gain);
    m_gain.setGainLinear(gainLinear);

    while (subBlockPosition < buffer.getNumSamples())
    {
        const auto                   subBlockSize = buffer.getNumSamples() - subBlockPosition;
        juce::dsp::AudioBlock<float> subBlock = block.getSubBlock((size_t) subBlockPosition, static_cast<size_t>(subBlockSize));

        juce::dsp::ProcessContextReplacing<float> context(subBlock);

        m_gain.process(context);
        m_delay.process(context);
        m_delay.updateParams();

        subBlockPosition += subBlockSize;
    }

    ignoreUnused(midiMessages);
}

//==============================================================================

AudioProcessorEditor* ModDelayAudioProcessor::createEditor()
{
    File sourceDir = File(PLUGIN_SOURCE_DIR);
    File bundle = sourceDir.getChildFile("jsui/build/js/main.js");

    auto* editor = new reactjuce::GenericEditor(*this, bundle);

    editor->setResizable(true, true);
    editor->setResizeLimits(400, 240, 400 * 2, 240 * 2);
    editor->getConstrainer()->setFixedAspectRatio(400.0 / 240.0);
    editor->setSize(400, 240);

    // Start timer to dispatch gainPeakValues event to update Meter values
    startTimer(100);

    return editor;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ModDelayAudioProcessor();
}
