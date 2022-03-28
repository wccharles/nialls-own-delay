#include "FeedbackDelayNetwork.h"
#include "FDNConstants.h"

namespace Constants
{
    constexpr auto smoothedValueRamp = 0.05f;
    constexpr auto delayTimeSmoothedValueRamp = 0.25f;
}

using namespace ModDelay::FDNMatrixData;
FeedbackDelayNetwork::FeedbackDelayNetwork(ParamsData& params) :
    m_modulationDepth(0.8f),
    m_decay(0.0f),
    m_dampingAmount(0.0f),
    m_dampingLowCutoff(0.0f),
    m_dampingHighCutoff(0.0f),
    m_dampingGlobalLowGain(0.0f),
    m_dampingGlobalHighGain(0.0f),
    m_dryWet(0.0f),
    m_preDelayTime(0.0f),
    m_feedbackNetwork(fdnSize, 1),
    m_correctionFilterLow(new IIRFilter()),
    m_correctionFilterHigh(new IIRFilter()),
    m_params(params)
{
    for (int i = 0; i < fdnSize - 1; i++)
    {
        m_lowShelf.add(new IIRFilter());
        m_highShelf.add(new IIRFilter());
    }
}

void FeedbackDelayNetwork::prepare(const dsp::ProcessSpec& spec)
{
    m_samplerate = spec.sampleRate;
    m_modulationDepth.reset(m_samplerate, Constants::smoothedValueRamp);
    m_decay.reset(m_samplerate, Constants::smoothedValueRamp);
    m_dryWet.reset(m_samplerate, Constants::smoothedValueRamp);
    m_preDelayTime.reset(m_samplerate, Constants::delayTimeSmoothedValueRamp);

    const auto iirCoefficients = dsp::IIR::Coefficients<float>::makeLowPass(m_samplerate, 8000);
    m_lowpass.reset(new dsp::IIR::Filter<float>(iirCoefficients));
    m_lowpass->prepare(spec);

    m_feedbackGains.clear();
    m_lfoOscillators.resize(fdnSize);
    m_delayLines.resize(fdnSize);
    m_feedbackGains.resize(fdnSize);
    for (size_t i = 0; i < fdnSize - 1; i++)
    {
        m_delayLines[i].reset();
        m_delayLines[i].prepare(spec);
        m_delayLines[i].setMaximumDelayInSamples(static_cast<int>(m_samplerate));
        m_delayLines[i].setDelay(exponentialDelayTimes[i] * static_cast<float>(m_samplerate));
        //        m_lfoOscillators[i].initialise([this](float x)
        //                                       { return m_modulationDepth.getNextValue() * std::sin(x); });
        //        m_lfoOscillators[i].prepare(spec);
        //        m_lfoOscillators[i].setFrequency(lfoRates[i]);
        m_lowShelf[static_cast<int>(i)]->reset();
        m_highShelf[static_cast<int>(i)]->reset();
    }

    m_correctionFilterLow->reset();
    m_correctionFilterHigh->reset();

    m_preDelay.reset();
    m_preDelay.prepare(spec);
    m_preDelay.setMaximumDelayInSamples(static_cast<int>(0.25f * m_samplerate));
    m_preDelay.setDelay(m_preDelayTime.getNextValue() * static_cast<float>(m_samplerate));

    m_feedbackNetwork.clear();
}

void FeedbackDelayNetwork::process(const dsp::ProcessContextReplacing<float>& context)
{
    const auto& inputBlock = context.getInputBlock();
    auto&       outputBlock = context.getOutputBlock();
    const auto  numChannels = outputBlock.getNumChannels();
    const auto  numSamples = outputBlock.getNumSamples();

    dsp::Matrix<float> inputMatrix(fdnSize, 1);
    dsp::Matrix<float> outputMatrix(fdnSize, 1);

    if (!context.isBypassed)
    {
        for (int sampleIndex = 0; sampleIndex < static_cast<int>(numSamples); ++sampleIndex)
        {
            const auto dryWet = m_dryWet.getNextValue();
            for (int channelIndex = 0; channelIndex < static_cast<int>(numChannels); ++channelIndex)
            {
                float      outputSample = 0.0f;
                const auto drySample = inputBlock.getSample(channelIndex, sampleIndex);

                m_preDelay.pushSample(channelIndex, drySample);
                const auto preDelayedSample = m_preDelay.popSample(channelIndex, m_preDelayTime.getNextValue() * static_cast<float>(m_samplerate));

                FloatVectorOperations::add(inputMatrix.getRawDataPointer(), m_feedbackNetwork.getRawDataPointer(), preDelayedSample, fdnSize);

                for (size_t i = 0; i < fdnSize - 1; i++)
                {
                    //                    lfoUpdate(i);
                    m_delayLines[i].pushSample(channelIndex, inputMatrix(i, 0));
                    const auto delayedSample = m_delayLines[i].popSample(channelIndex) * m_feedbackGains[i];
                    const auto lowShelfSample = m_lowShelf[static_cast<int>(i)]->processSingleSampleRaw(delayedSample);
                    const auto highShelfSample = m_highShelf[static_cast<int>(i)]->processSingleSampleRaw(lowShelfSample);

                    const auto dryDamp = delayedSample * (1.0f - m_dampingAmount.getNextValue());
                    const auto wetDamp = highShelfSample * m_dampingAmount.getNextValue();

                    outputMatrix(i, 0) = dryDamp + wetDamp;
                }

                FloatVectorOperations::copy(m_feedbackNetwork.getRawDataPointer(), outputMatrix.getRawDataPointer(), fdnSize);

                jassert(std::none_of(m_feedbackNetwork.begin(), m_feedbackNetwork.end(), [](float i)
                                     { return (i >= 1.0f && i <= 0.0f); }));

                outputSample = std::accumulate(m_feedbackNetwork.begin(), m_feedbackNetwork.end(), 0.0f);

                //                const auto lowpassed = m_lowpass->processSample(outputSample);
                const auto correctionLow = m_correctionFilterLow->processSingleSampleRaw(outputSample);
                const auto correctionHigh = m_correctionFilterHigh->processSingleSampleRaw(correctionLow);

                outputBlock.addSample(channelIndex, sampleIndex, (correctionHigh / fdnSize) * dryWet);
            }
        }
    }
    else
    {
        outputBlock.copyFrom(inputBlock);
    }
}

void FeedbackDelayNetwork::updateParams()
{
    const auto currentDryWet = m_params.getValue(ModDelay::ParamID::ReverbDryWet);
    m_dryWet.setTargetValue(currentDryWet);

    const auto currentDecay = m_params.getValue(ModDelay::ParamID::ReverbDecay);
    m_decay.setTargetValue(currentDecay);

    const auto currentFreeze = m_params.getBoolValue(ModDelay::ParamID::ReverbFreeze);
    m_freeze.set(currentFreeze);

    const auto currentDampnessAmount = m_params.getValue(ModDelay::ParamID::ReverbDampnessAmount);
    m_dampingAmount.setTargetValue(m_freeze.get() ? 0.0f : currentDampnessAmount);

    const auto currentLowShelfCutoff = m_params.getValue(ModDelay::ParamID::ReverbDampLowFrequency);
    m_dampingLowCutoff.set(currentLowShelfCutoff);

    const auto currentHighShelfCutoff = m_params.getValue(ModDelay::ParamID::ReverbDampHighFrequency);
    m_dampingHighCutoff.set(currentHighShelfCutoff);

    const auto currentGlobalLowGain = m_params.getValue(ModDelay::ParamID::ReverbDampLowFactor);
    m_dampingGlobalLowGain.set(currentGlobalLowGain);

    const auto currentGlobalHighGain = m_params.getValue(ModDelay::ParamID::ReverbDampHighFactor);
    m_dampingGlobalHighGain.set(currentGlobalHighGain);

    const auto low = 2.0f * currentDecay;
    const auto high = 0.25f * currentDecay;

    auto reverbTime = [this](float decayTime, int index)
    { return powf(10, (-3 * (exponentialDelayTimes[index] * static_cast<float>(m_samplerate))) / (decayTime * static_cast<float>(m_samplerate))); };

    for (int i = 0; i < fdnSize - 1; i++)
    {
        const auto feedbackDecay = reverbTime(m_decay.getNextValue(), i);
        m_feedbackGains[static_cast<size_t>(i)] = m_freeze.get() ? 1.0f : feedbackDecay;

        const auto lowGain = reverbTime(low, i);
        const auto highGain = reverbTime(high, i);

        const Atomic<float> lowShelfGainFactor((lowGain / feedbackDecay) * m_dampingGlobalLowGain.get());
        const Atomic<float> highShelfGainFactor((highGain / feedbackDecay) * m_dampingGlobalHighGain.get());

        m_lowShelf[i]->setCoefficients(IIRCoefficients::makeLowShelf(m_samplerate, m_dampingLowCutoff.get(), 0.7f, lowShelfGainFactor.get()));
        m_highShelf[i]->setCoefficients(IIRCoefficients::makeHighShelf(m_samplerate, m_dampingHighCutoff.get(), 0.7f, highShelfGainFactor.get()));
    }

    const Atomic<float> correctionLowShelfGainFactor(1.0f / sqrt(low / currentDecay));
    const Atomic<float> correctionHighShelfGainFactor(1.0f / sqrt(high / currentDecay));

    m_correctionFilterLow->setCoefficients(IIRCoefficients::makeLowShelf(m_samplerate, m_dampingLowCutoff.get(), 0.7f, correctionLowShelfGainFactor.get()));
    m_correctionFilterHigh->setCoefficients(IIRCoefficients::makeHighShelf(m_samplerate, m_dampingHighCutoff.get(), 0.7f, correctionHighShelfGainFactor.get()));

    jassert(std::none_of(m_feedbackGains.begin(), m_feedbackGains.end(), [](float i)
                         { return (i >= 1.0f && i <= 0.0f); }));

    const auto currentPreDelayTime = m_params.getValue(ModDelay::ParamID::ReverbPreDelayTime);
    m_preDelayTime.setTargetValue(currentPreDelayTime);
}

//void FeedbackDelayNetwork::lfoUpdate(int index)
//{
//    const auto lfo = m_lfoOscillators[index].processSample(0.0f);
//    const auto modDelay = jmap(lfo, -1.0f, 1.0f, primeDelayTimes[index] * static_cast<float>(m_samplerate) - 1.0f, primeDelayTimes[index] * static_cast<float>(m_samplerate) + 1.0f);
//    const auto modDelayLimit = jlimit(0.0f, static_cast<float>(m_delayLines[index].getMaximumDelayInSamples()), modDelay);
//
//    m_delayLines[index].setDelay(modDelayLimit);
//}
