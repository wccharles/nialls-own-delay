#include "Delay.h"
#include "DelayConstants.h"
#include "SharedConstants.h"

using namespace ModDelay::DelayConstants;
using namespace ModDelay::FilterConstants;

static constexpr auto maximumDelayTimeInSamples = [](double sampleRate)
{ return static_cast<int>(maximumDelayInSeconds * sampleRate); };

using namespace std::placeholders;
Delay::Delay(ParamsData& params) :
    m_cutoffFrequency(1.0f),
    m_resonance(0.01f),
    m_filterType(0),
    m_prePostFilterChoice(0),
    m_sampleRate(0.0),
    bypassProcess([](const float input, const int channel)
                  {
    ignoreUnused(channel);
    return input; }),
    filterProcess([this](const float input, const int channel)
                  { return m_filterDrive.processSample(m_filter.processSample(channel, input)); }),
    modulationProcess(std::bind(&Modulation::processSample, &this->m_modulation, _1, _2)),
    stftProcess(std::bind(&STFT::processSample, &this->m_stft, _1, _2)),
    m_params(params)
{
}

void Delay::prepare(const dsp::ProcessSpec& spec)
{
    m_sampleRate = spec.sampleRate;
    jassert(m_sampleRate <= maximumDelaySampleRate);

    m_dryWet.reset(m_sampleRate, ModDelay::SharedConstants::smoothedValueRamp);
    m_time.reset(m_sampleRate, ModDelay::SharedConstants::delayTimeSmoothedValueRamp);
    m_feedback.reset(m_sampleRate, ModDelay::SharedConstants::smoothedValueRamp);

    m_filterDrive.reset();
    m_filterDrive.prepare(spec);
    m_filterDrive.setRampDurationSeconds(ModDelay::SharedConstants::smoothedValueRamp);

    m_delayLine.reset();
    m_delayLine.prepare(spec);
    m_delayLine.setMaximumDelayInSamples(maximumDelayTimeInSamples(m_sampleRate));
    m_delayLine.setDelay(m_time.getCurrentValue() * static_cast<float>(m_sampleRate));

    m_filter.reset();
    m_filter.prepare(spec);
    m_filter.setType(dsp::StateVariableTPTFilterType(m_filterType));
    m_filter.setCutoffFrequency(m_cutoffFrequency.getNextValue());
    m_filter.setResonance(m_resonance.getCurrentValue());

    m_stft.setup(m_sampleRate, static_cast<int>(spec.numChannels));
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

                const auto modulatedSample = connectEffects(nextSample, channelIndex, modulationProcess);

                const auto freqShiftedSample = connectEffects(modulatedSample, channelIndex, stftProcess);

                // Push the next sample to the delay line
                m_delayLine.pushSample(channelIndex, freqShiftedSample);
            }
            m_stft.updatePointers();
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
    jassert(currentDelayTime <= maximumDelayInSeconds);
    m_time.setTargetValue(currentDelayTime);

    const auto currentFeedback = m_params.getValue(ModDelay::ParamID::DelayFeedback);
    m_feedback.setTargetValue(currentFeedback);

    const auto currentDryWet = m_params.getValue(ModDelay::ParamID::DelayDryWet);
    m_dryWet.setTargetValue(currentDryWet);

    const auto currentFilterDrive = m_params.getValue(ModDelay::ParamID::FilterDrive);
    m_filterDrive.setGainLinear(currentFilterDrive);

    const auto currentFreqCutoff = m_params.getValue(ModDelay::ParamID::FrequencyCutoff);
    jassert(currentFreqCutoff >= minimumCutoffFrequency && currentFreqCutoff <= maximumCutoffFrequency);
    m_cutoffFrequency.setTargetValue(currentFreqCutoff);

    const auto currentFreqResonance = m_params.getValue(ModDelay::ParamID::FrequencyResonance);
    jassert(currentFreqResonance >= minimumResonance && currentFreqResonance <= maximumResonance);
    m_resonance.setTargetValue(currentFreqResonance);

    const auto currentFreqShift = m_params.getValue(ModDelay::ParamID::FreqShift);
    m_stft.updateParameters(currentFreqShift);

    const auto currentWowFactor = m_params.getValue(ModDelay::ParamID::WowFactor);
    const auto currentWowDepth = m_params.getValue(ModDelay::ParamID::WowDepth);

    m_modulation.youveGotTheWowFactor(currentWowFactor, currentWowDepth);

    const auto currentPrePostFilter = m_params.getChoiceValue(ModDelay::ParamID::PrePostFilter);
    m_prePostFilterChoice.set(currentPrePostFilter);

    const auto currentFilterType = m_params.getChoiceValue(ModDelay::ParamID::FilterType);
    m_filterType = currentFilterType;
}

const float Delay::connectEffects(const float input, const int channel, const std::function<const float(const float, const int)> processSample)
{
    return processSample(input, channel);
}
