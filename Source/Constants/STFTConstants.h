#pragma once

#include <juce_core/juce_core.h>

namespace ModDelay
{
    namespace STFTConstants
    {
        static constexpr auto pi = juce::MathConstants<float>::pi;
        static constexpr auto fftOrder = 11;
        static constexpr auto fftSize = 1 << fftOrder;
        static constexpr auto overlap = 2;
        static constexpr auto hopSize = fftSize / overlap;
        static constexpr auto minimumFrequencyShift = -12.0f;
        static constexpr auto maximumFrequencyShift = 12.0f;
    } // namespace STFTConstants
} // namespace ModDelay
