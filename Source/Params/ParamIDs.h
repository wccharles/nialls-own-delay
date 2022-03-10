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

        const String FreqShift = "FreqShift";

        const String WowFactor = "WowFactor";
        const String WowDepth = "WowDepth";

    } // namespace ParamID

    namespace ParamTuples
    {
        // Tuple of parameter data goes: ID, Range, Default Value.
        const std::vector<std::tuple<String, NormalisableRange<float>, float, std::function<String(float, int)>>> floatParameterData {

            { ParamID::Gain, { 0.0f, 1.0f }, 0.75f, [](float value, int /* maxLength */)
              { return String(Decibels::gainToDecibels(value), 1) + "dB"; } },

            { ParamID::DelayDryWet, { 0.0f, 1.0f }, 0.0f, [](float value, int /* maxLength */)
              { return String(value * 100.f, 1) + "%"; } },
            { ParamID::DelayTime, { 0.01f, 1.0f, 0.00001f }, 0.25f, [](float value, int /* maxLength */)
              { return String(value * 1000.0f, 2) + "ms"; } },
            { ParamID::DelayFeedback, { 0.0f, 1.0f }, 0.5f, [](float value, int /* maxLength */)
              { return String(value * 100.f, 1) + "%"; } },

            { ParamID::FreqShift, { -12.0f, 12.0f, 2.0f }, 0.0f, [](float value, int /**/)
              { return String(value); } },

            { ParamID::WowFactor, { 0.0f, 5.0f, 0.01f }, 0.5f, [](float value, int /**/)
              { return String(value) + "Hz"; } },
            { ParamID::WowDepth, { 0.0f, 1.0f }, 0.0f, [](float value, int /**/)
              { return String(value, 2); } }
        };
    }
}
