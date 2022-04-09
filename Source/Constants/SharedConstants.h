#pragma once

#include <juce_core/juce_core.h>

namespace ModDelay
{
    namespace SharedConstants
    {
        // constexpr int maxNumChannels = 2;
        // constexpr int maxNumParameters = 12;
        // constexpr int maxNumParameterGroups = 1;
        // constexpr int maxNumParameterIDs = maxNumParameters * maxNumParameterGroups;
        // constexpr int maxNumParameterTuples = maxNumParameterIDs * maxNumParameterGroups;

        static constexpr auto pi = juce::MathConstants<float>::pi;
        static constexpr auto twoPi = juce::MathConstants<float>::twoPi;
        static constexpr auto halfPi = juce::MathConstants<float>::halfPi;
        static constexpr auto euler = juce::MathConstants<float>::euler;
        static constexpr auto sqrt2 = juce::MathConstants<float>::sqrt2;
        static constexpr auto halfSqrt2 = 1.0f / sqrt2;

        static constexpr auto smoothedValueRamp = 0.05f;
        static constexpr auto delayTimeSmoothedValueRamp = 0.25f;

        juce::ignoreUnused(pi, twoPi, halfPi, euler, sqrt2, halfSqrt2);
    } // namespace SharedConstants
}
