#pragma once

#include <juce_core/juce_core.h>

namespace ModDelay
{
    namespace SharedConstants
    {
        constexpr int maxNumChannels = 2;
        constexpr int maxNumParameters = 12;
        constexpr int maxNumParameterGroups = 1;
        constexpr int maxNumParameterIDs = maxNumParameters * maxNumParameterGroups;
        constexpr int maxNumParameterTuples = maxNumParameterIDs * maxNumParameterGroups;

        static constexpr auto pi = juce::MathConstants<float>::pi;
        static constexpr auto twoPi = juce::MathConstants<float>::twoPi;
        static constexpr auto halfPi = juce::MathConstants<float>::halfPi;
        static constexpr auto euler = juce::MathConstants<float>::euler;
        static constexpr auto sqrt2 = juce::MathConstants<float>::sqrt2;
        static constexpr auto halfSqrt2 = 1.0f / sqrt2;

        static constexpr auto smoothedValueRamp = 0.05f;
        static constexpr auto delayTimeSmoothedValueRamp = 0.25f;
    } // namespace SharedConstants

    namespace DelayConstants
    {
        static constexpr auto maximumDelayInSeconds = 1.0f;
        static constexpr auto maximumDelaySampleRate = 192000.0f;
    } // namespace DelayConstants

    namespace FilterConstants
    {
        static constexpr auto minimumCutoffFrequency = 20.0f;
        static constexpr auto maximumCutoffFrequency = 20000.0f;
        static constexpr auto minimumResonance = 0.01f;
        static constexpr auto maximumResonance = 5.0f;
    } // namespace FilterConstants

    namespace STFTConstants
    {
        static constexpr auto fftOrder = 11;
        static constexpr auto fftSize = 1 << fftOrder;
        static constexpr auto overlap = 2;
        static constexpr auto hopSize = fftSize / overlap;
        static constexpr auto minimumFrequencyShift = -12.0f;
        static constexpr auto maximumFrequencyShift = 12.0f;
    } // namespace STFTConstants

    namespace ModulationConstants
    {
        static constexpr auto maximumModulationDepth = 1.0f;
        static constexpr auto maximumModulationRate = 5.0f;
        static constexpr auto maximumModDelayInSeconds = 0.25f;
    } // namespace ModulationConstants

    namespace FDNConstants
    {
        static constexpr auto fdnSize = 16;
        static constexpr auto maximumPreDelayInSeconds = 0.25f;
        static constexpr auto minimumReverbDecay = 0.25f;
        static float          exponentialDelayTimes[] = { 0.01f, 0.0128083f, 0.0164052f, 0.0210122f, 0.026913f, 0.034471f, 0.0441513f, 0.0565503f, 0.0724311f, 0.0927718f, 0.118825f, 0.152194f, 0.194934f, 0.249677f, 0.319793f, 0.4096f };
    } // namespace FDNConstants
}
