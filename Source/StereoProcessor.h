#pragma once

#include <JuceHeader.h>

class StereoProcessor
{
public:
    StereoProcessor()
    {
    }
    ~StereoProcessor() {}
    void prepare(const dsp::ProcessSpec& spec)
    {
        m_midGain.reset();
        m_midGain.prepare(spec);
        m_midGain.setRampDurationSeconds(ModDelay::SharedConstants::smoothedValueRamp);

        m_sideGain.reset();
        m_sideGain.prepare(spec);
        m_sideGain.setRampDurationSeconds(ModDelay::SharedConstants::smoothedValueRamp);
    }
    void process(const dsp::ProcessContextReplacing<float>& context);
    void updateParams(const float stereoWidth)
    {
        const float widthdB = stereoWidth;
        const float sidedB = widthdB / 2.0f;
        const float middB = -(sidedB);
        m_midGain.setGainDecibels(middB);
        m_sideGain.setGainDecibels(sidedB);
    }

private:
    dsp::Gain<float> m_midGain;
    dsp::Gain<float> m_sideGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoProcessor)
};

inline void StereoProcessor::process(const dsp::ProcessContextReplacing<float>& context)
{
    auto&      outputBlock = context.getOutputBlock();
    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    if (!context.isBypassed)
    {
        if (numChannels >= 2)
        {
            auto* l = outputBlock.getChannelPointer(0);
            auto* r = outputBlock.getChannelPointer(1);

            for (int sampleIndex = 0; sampleIndex < static_cast<int>(numSamples); ++sampleIndex)
            {
                const auto m = l[sampleIndex] + r[sampleIndex]; // encode mid-side
                const auto s = l[sampleIndex] - r[sampleIndex];

                const auto midGains = m_midGain.processSample(m); // perform gains for width
                const auto sideGains = m_sideGain.processSample(s);

                l[sampleIndex] = (midGains + sideGains) / 2.0f; // decode mid-side
                r[sampleIndex] = (midGains - sideGains) / 2.0f;
            }
        }
    }
}
