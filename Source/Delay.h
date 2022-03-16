#pragma once

#include "Modulation.h"
#include "ParamsData.h"
#include "STFT.h"
#include <JuceHeader.h>

class Delay
{
public:
    Delay(ParamsData& params);

    void prepare(const dsp::ProcessSpec& spec);
    void process(const dsp::ProcessContextReplacing<float>& context);

    void updateParams();

private:
    const float connectEffects(const float input, const int channel, const std::function<const float(const float, const int)> processSample);

private:
    SmoothedValue<float> m_dryWet;
    SmoothedValue<float> m_time;
    SmoothedValue<float> m_feedback;

    dsp::Gain<float>     m_filterDrive;
    SmoothedValue<float> m_cutoffFrequency;
    SmoothedValue<float> m_resonance;
    int                  m_filterType;
    Atomic<int>          m_prePostFilterChoice;

    double m_sampleRate;

    dsp::DelayLine<float>              m_delayLine;
    dsp::StateVariableTPTFilter<float> m_filter;
    Modulation                         m_modulation;
    STFT                               m_stft;

    std::function<const float(const float, const int)> bypassProcess;
    std::function<const float(const float, const int)> filterProcess;
    std::function<const float(const float, const int)> modulationProcess;
    std::function<const float(const float, const int)> stftProcess;

    ParamsData& m_params;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Delay)
};
