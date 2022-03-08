#pragma once

#include <JuceHeader.h>

namespace ModDelay
{
    namespace ParamID
    {
        const String Gain = "Gain";

        const String DelayDryWet = "DelayDryWet";
        const String DelayTime = "DelayTime";
        const String DelayFeedback = "DelayFeedback";

        const String Attack = "Attack";
        const String Decay = "Decay";
        const String Sustain = "Sustain";
        const String Release = "Release";

    } // namespace ParamID

    namespace ParamTuples
    {
        // Tuple of parameter data goes: ID, Range, Default Value.
        const std::vector<std::tuple<String, NormalisableRange<float>, float>> floatParameterData {
            { ParamID::Gain, { 0.0f, 1.0f }, 0.75f },

            { ParamID::DelayDryWet, { 0.0f, 2.0f, 1.0f }, 0.0f },
            { ParamID::DelayTime, { 0.1f, 2.0f, 4.0f }, 0.25f },
            { ParamID::DelayFeedback, { 0.0f, 2.0f, 1.0f }, 0.5f },

            { ParamID::Attack, { 0.0f, 2.0f, 0.01f }, 1.0f },
            { ParamID::Decay, { 0.0f, 2, 0f, 1.0f }, 1.0f },
            { ParamID::Sustain, { 0.0f, 2.0f, 1.0f }, 1.0f },
            { ParamID::Release, { 0.0f, 2.0f, 0.01f }, 1.0f }
        };
    }
}
