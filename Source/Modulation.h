#pragma once

#include <JuceHeader.h>

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

        m_wowFactor.reset(m_samplerate, 0.05f);
        m_wowDepth.reset(m_samplerate, 0.05f);

        mod_osc.prepare(spec);
        mod_osc.setFrequency(m_wowFactor.getNextValue());

        m_wowDelay.reset();
        m_wowDelay.prepare(spec);
        m_wowDelay.setMaximumDelayInSamples(0.25f * static_cast<float>(m_samplerate));
        m_wowDelay.setDelay(0.0f);
    }

    void youveGotTheWowFactor(const float wowFactor, const float wowDepth)
    {
        m_wowFactor.setTargetValue(wowFactor);
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
