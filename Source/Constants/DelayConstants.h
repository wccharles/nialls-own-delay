#pragma once

#include <juce_core/juce_core.h>

namespace ModDelay
{
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
} // namespace ModDelay
