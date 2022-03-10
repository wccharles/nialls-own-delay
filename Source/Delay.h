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
    SmoothedValue<float> m_dryWet;
    SmoothedValue<float> m_time;
    SmoothedValue<float> m_feedback;

    double m_sampleRate;

    Modulation            m_modulation;
    dsp::DelayLine<float> m_delayLine;
    STFT                  stft;

    ParamsData& m_params;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Delay)
};
