#include "Delay.h"

namespace Constants
{
    constexpr auto smoothedValueRamp = 0.05f;
    constexpr auto delayTimeSmoothedValueRamp = 0.25f;
    constexpr auto maximumDelaySeconds = 4.0f;
    constexpr auto maximumDelaySampleRate = 192000.0f;
    constexpr auto maximumDelaySamples = static_cast<int>(maximumDelaySeconds * maximumDelaySampleRate);
}

Delay::Delay(ParamsData& params) :
    m_params(params),
    m_sampleRate(0.0),
    m_delayLine(Constants::maximumDelaySamples)
{
}

void Delay::prepare(const juce::dsp::ProcessSpec& spec)
{
    m_sampleRate = spec.sampleRate;
    jassert(m_sampleRate <= Constants::maximumDelaySampleRate);

    m_dryWet.reset(m_sampleRate, Constants::smoothedValueRamp);
    m_time.reset(m_sampleRate, Constants::delayTimeSmoothedValueRamp);
    m_feedback.reset(m_sampleRate, Constants::smoothedValueRamp);

    updateParams();

    m_delayLine.reset();
    m_delayLine.prepare(spec);
    m_delayLine.setDelay(m_time.getCurrentValue() * static_cast<float>(m_sampleRate));

    stft.setup(m_sampleRate, spec.numChannels);
}

void Delay::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
    const auto& inputBlock = context.getInputBlock();
    auto&       outputBlock = context.getOutputBlock();
    const auto  numChannels = outputBlock.getNumChannels();
    const auto  numSamples = outputBlock.getNumSamples();

    if (!context.isBypassed)
    {
        for (int sampleIndex = 0; sampleIndex < static_cast<int>(numSamples); ++sampleIndex)
        {
            m_delayLine.setDelay(m_time.getNextValue() * static_cast<int>(m_sampleRate));

            const auto dryWet = m_dryWet.getNextValue();
            auto       feedback = m_feedback.getNextValue();

            for (int channelIndex = 0; channelIndex < static_cast<int>(numChannels); ++channelIndex)
            {
                const auto delayedSample = m_delayLine.popSample(channelIndex);

                // Add the delayed sample to the output block
                outputBlock.addSample(channelIndex, sampleIndex, delayedSample * dryWet * feedback);

                // Get the next sample
                const auto nextSample = outputBlock.getSample(channelIndex, sampleIndex);

                const auto freqShiftedSample = stft.processSample(nextSample, channelIndex);

                // Push the next sample to the delay line
                m_delayLine.pushSample(channelIndex, freqShiftedSample);
            }
            stft.updatePointers();
        }
    }
    else
    {
        outputBlock.copyFrom(inputBlock);
    }
}

void Delay::updateParams()
{
    const auto currentDelayTime = m_params.getValue(ModDelay::ParamID::DelayTime);
    assert(currentDelayTime <= Constants::maximumDelaySeconds);
    m_time.setTargetValue(currentDelayTime);

    const auto currentFeedback = m_params.getValue(ModDelay::ParamID::DelayFeedback);
    m_feedback.setTargetValue(currentFeedback);

    const auto currentDryWet = m_params.getValue(ModDelay::ParamID::DelayDryWet);
    m_dryWet.setTargetValue(currentDryWet);

    const auto freqShift = m_params.getValue(ModDelay::ParamID::FreqShift);
    stft.updateParameters(freqShift);
}
