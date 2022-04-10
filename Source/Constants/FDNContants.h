#pragma once

#include <juce_core/juce_core.h>

namespace ModDelay
{
    namespace FDNConstants
    {
        static constexpr auto fdnSize = 16;
        static constexpr auto maximumPreDelayInSeconds = 0.25f;
        static constexpr auto minimumReverbDecay = 0.25f;
        static float          exponentialDelayTimes[] = { 0.01f, 0.0128083f, 0.0164052f, 0.0210122f, 0.026913f, 0.034471f, 0.0441513f, 0.0565503f, 0.0724311f, 0.0927718f, 0.118825f, 0.152194f, 0.194934f, 0.249677f, 0.319793f, 0.4096f };
    } // namespace FDNConstants
} // namespace ModDelay
