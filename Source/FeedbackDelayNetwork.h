#pragma once

#include "ParamsData.h"
#include <JuceHeader.h>

class FeedbackDelayNetwork
{
public:
    FeedbackDelayNetwork(ParamsData& params);
    ~FeedbackDelayNetwork() {}

    void prepare(const dsp::ProcessSpec& spec);
    void process(const dsp::ProcessContextReplacing<float>& context);

    void updateParams();

private:
    void lfoUpdate(int index);

private:
    std::vector<float>   m_feedbackGains;
    SmoothedValue<float> m_modulationDepth;
    SmoothedValue<float> m_decay;
    Atomic<bool>         m_freeze;

    SmoothedValue<float> m_dampingAmount;
    Atomic<float>        m_dampingLowCutoff;
    Atomic<float>        m_dampingHighCutoff;
    Atomic<float>        m_dampingGlobalLowGain;
    Atomic<float>        m_dampingGlobalHighGain;

    SmoothedValue<float> m_dryWet;

    SmoothedValue<float> m_preDelayTime;

    dsp::Matrix<float> m_feedbackNetwork;

    std::vector<dsp::DelayLine<float>> m_delayLines;

    OwnedArray<IIRFilter> m_lowShelf;
    OwnedArray<IIRFilter> m_highShelf;

    std::unique_ptr<IIRFilter> m_correctionFilterLow;
    std::unique_ptr<IIRFilter> m_correctionFilterHigh;

    dsp::DelayLine<float> m_preDelay;

    std::unique_ptr<dsp::IIR::Filter<float>> m_lowpass;

    double m_samplerate;

    ParamsData& m_params;
};
