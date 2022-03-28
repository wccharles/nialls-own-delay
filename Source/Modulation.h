#pragma once

#include "Constants.h"
#include <JuceHeader.h>

using namespace ModDelay::ModulationConstants;

static constexpr auto maximumModDelayInSamples = [](double sampleRate)
{ return static_cast<int>(maximumModDelayInSeconds * sampleRate); };

class Modulation
{
public:
    Modulation()
    {
    }
    ~Modulation() {}

    void prepare(const dsp::ProcessSpec& spec)
    {
        m_samplerate = spec.sampleRate;

        m_wowFactor.reset(m_samplerate, ModDelay::SharedConstants::smoothedValueRamp);
        m_wowDepth.reset(m_samplerate, ModDelay::SharedConstants::smoothedValueRamp);

        mod_osc.prepare(spec);
        mod_osc.setFrequency(m_wowFactor.getNextValue());

        m_wowDelay.reset();
        m_wowDelay.prepare(spec);
        m_wowDelay.setMaximumDelayInSamples(maximumModDelayInSamples(m_samplerate));
        m_wowDelay.setDelay(0.0f);
    }

    void youveGotTheWowFactor(const float wowFactor, const float wowDepth)
    {
        jassert(wowFactor >= 0.0f && wowFactor <= maximumModulationRate);
        m_wowFactor.setTargetValue(wowFactor);
        jassert(wowDepth >= 0.0f && wowDepth <= maximumModulationDepth);
        m_wowDepth.setTargetValue(wowDepth);
    }

    const float processSample(const float input, const int channel)
    {
        float output = 0.0f;
        mod_osc.setFrequency(m_wowFactor.getNextValue());
        const auto wowLFOOut = mod_osc.processSample(0.0f) * m_wowDepth.getNextValue();
        auto       modulatedDelay = jlimit(0.0f, static_cast<float>(m_wowDelay.getMaximumDelayInSamples()), jmap(wowLFOOut, -1.0f, 1.0f, 0.0f, static_cast<float>(m_wowDelay.getMaximumDelayInSamples())));

        m_wowDelay.pushSample(channel, input);
        output = m_wowDelay.popSample(channel, modulatedDelay);

        return output;
    }

private:
    dsp::Oscillator<float> mod_osc {
        [](float x)
        { return std::sin(x); }
    };

    dsp::DelayLine<float> m_wowDelay;
    dsp::DelayLine<float> m_flutterDelay;

    SmoothedValue<float> m_wowFactor;
    SmoothedValue<float> m_wowDepth;

    double m_samplerate;

    Random random;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Modulation)
};
