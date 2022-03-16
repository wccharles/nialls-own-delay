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
    m_filterType(0),
    m_prePostFilterChoice(0),
    m_sampleRate(0.0),
    m_delayLine(Constants::maximumDelaySamples),
    bypassProcess([](const float input, const int channel)
                  {
    ignoreUnused(channel);
    return input; }),
    filterProcess([this](const float input, const int channel)
                  { return m_filterDrive.processSample(m_filter.processSample(channel, input)); }),
    stftProcess(std::bind(&STFT::processSample, &this->stft, _1, _2)),
    modulationProcess(std::bind(&Modulation::processSample, &this->m_modulation, _1, _2)),
    m_params(params)
{
}

void Delay::prepare(const dsp::ProcessSpec& spec)
{
    m_sampleRate = spec.sampleRate;
    jassert(m_sampleRate <= Constants::maximumDelaySampleRate);

    m_dryWet.reset(m_sampleRate, Constants::smoothedValueRamp);
    m_time.reset(m_sampleRate, Constants::delayTimeSmoothedValueRamp);
    m_feedback.reset(m_sampleRate, Constants::smoothedValueRamp);

    m_filterDrive.reset();
    m_filterDrive.prepare(spec);
    m_filterDrive.setRampDurationSeconds(Constants::smoothedValueRamp);

    updateParams();

    m_delayLine.reset();
    m_delayLine.prepare(spec);
    m_delayLine.setDelay(m_time.getCurrentValue() * static_cast<float>(m_sampleRate));

    m_filter.reset();
    m_filter.prepare(spec);
    m_filter.setType(dsp::StateVariableTPTFilterType(m_filterType));
    m_filter.setCutoffFrequency(m_cutoffFrequency.getNextValue());
    m_filter.setResonance(m_resonance.getCurrentValue());

    stft.setup(m_sampleRate, spec.numChannels);
    m_modulation.prepare(spec);
}

void Delay::process(const dsp::ProcessContextReplacing<float>& context)
{
    const auto& inputBlock = context.getInputBlock();
    auto&       outputBlock = context.getOutputBlock();
    const auto  numChannels = outputBlock.getNumChannels();
    const auto  numSamples = outputBlock.getNumSamples();

    if (!context.isBypassed)
    {
        for (int sampleIndex = 0; sampleIndex < static_cast<int>(numSamples); ++sampleIndex)
        {
            m_filter.setCutoffFrequency(m_cutoffFrequency.getNextValue());
            m_filter.setResonance(m_resonance.getNextValue());
            m_filter.setType(dsp::StateVariableTPTFilterType(m_filterType));
            m_delayLine.setDelay(m_time.getNextValue() * static_cast<int>(m_sampleRate));

            const auto dryWet = m_dryWet.getNextValue();
            auto       feedback = m_feedback.getNextValue();

            for (int channelIndex = 0; channelIndex < static_cast<int>(numChannels); ++channelIndex)
            {
                const auto drySample = inputBlock.getSample(channelIndex, sampleIndex);

                const auto preProcess = m_prePostFilterChoice.get() ? bypassProcess : filterProcess;

                const auto preDelayFilterSample = connectEffects(drySample, channelIndex, preProcess);

                outputBlock.setSample(channelIndex, sampleIndex, preDelayFilterSample);

                const auto delayedSample = m_delayLine.popSample(channelIndex);

                const auto postProcess = m_prePostFilterChoice.get() ? filterProcess : bypassProcess;

                const auto postDelayFilterSample = connectEffects(delayedSample, channelIndex, postProcess);

                // Add the delayed sample to the output block
                outputBlock.addSample(channelIndex, sampleIndex, postDelayFilterSample * dryWet * feedback);

                // Get the next sample
                const auto nextSample = outputBlock.getSample(channelIndex, sampleIndex);

                const auto modulatedSample = m_modulation.processSample(nextSample, channelIndex);

                const auto freqShiftedSample = stft.processSample(modulatedSample, channelIndex);

                // Push the next sample to the delay line
                m_delayLine.pushSample(channelIndex, freqShiftedSample);
            }
            stft.updatePointers();
        }
        m_filter.snapToZero();
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

    const auto currentFilterDrive = m_params.getValue(ModDelay::ParamID::FilterDrive);
    m_filterDrive.setGainLinear(currentFilterDrive);

    const auto currentFreqCutoff = m_params.getValue(ModDelay::ParamID::FrequencyCutoff);
    m_cutoffFrequency.setTargetValue(currentFreqCutoff);

    const auto currentFreqResonance = m_params.getValue(ModDelay::ParamID::FrequencyResonance);
    m_resonance.setTargetValue(currentFreqResonance);

    const auto freqShift = m_params.getValue(ModDelay::ParamID::FreqShift);
    stft.updateParameters(freqShift);

    const auto wowFactor = m_params.getValue(ModDelay::ParamID::WowFactor);
    const auto wowDepth = m_params.getValue(ModDelay::ParamID::WowDepth);

    m_modulation.youveGotTheWowFactor(wowFactor, wowDepth);

    const auto prePostFilterChoice = m_params.getChoiceValue(ModDelay::ParamID::PrePostFilter);
    m_prePostFilterChoice.set(prePostFilterChoice);

    const auto filterType = m_params.getChoiceValue(ModDelay::ParamID::FilterType);
    m_filterType = filterType;
}
