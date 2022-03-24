#pragma once

#include <JuceHeader.h>

namespace ModDelay
{
    namespace ParamID
    {
        const String Gain = "Gain";

        const String StereoWidth = "StereoWidth";

        const String DelayDryWet = "DelayDryWet";
        const String DelayTime = "DelayTime";
        const String DelayFeedback = "DelayFeedback";

        const String FilterDrive = "FilterDrive";
        const String FrequencyCutoff = "FrequencyCutoff";
        const String FrequencyResonance = "FrequencyResonance";

        const String FreqShift = "FreqShift";

        const String WowFactor = "WowFactor";
        const String WowDepth = "WowDepth";

        const String PrePostFilter = "PrePostFilter";
        const String FilterType = "FilterType";

    } // namespace ParamID

    const StringArray prePostDelayFilter = { "Pre", "Post" };
    const StringArray filterType = { "LPF", "BPF", "HPF" };

    namespace ParamTuples
    {
        // Tuple of parameter data goes: ID, Range, Default Value.
        const std::vector<std::tuple<String, NormalisableRange<float>, float, std::function<String(float, int)>>> floatParameterData {

            { ParamID::Gain, { 0.0f, 1.0f }, 0.75f, [](float value, int /* maxLength */)
              { return String(Decibels::gainToDecibels(value), 1) + "dB"; } },

            { ParamID::StereoWidth, { Decibels::gainToDecibels(0.5f), Decibels::gainToDecibels(2.0f) }, 0.0f, [](float value, int /* maxLength */)
              {
                  return String(Decibels::decibelsToGain(value) * 100.f, 1) + "%";
              } },

            { ParamID::DelayDryWet, { 0.0f, 1.0f }, 0.0f, [](float value, int /* maxLength */)
              { return String(value * 100.f, 1) + "%"; } },
            { ParamID::DelayTime, { 0.01f, 1.0f, 0.00001f }, 0.25f, [](float value, int /* maxLength */)
              { return String(value * 1000.0f, 2) + "ms"; } },
            { ParamID::DelayFeedback, { 0.0f, 1.0f }, 0.5f, [](float value, int /* maxLength */)
              { return String(value * 100.f, 1) + "%"; } },

            { ParamID::FilterDrive, { 0.5f, 2.0f }, 1.0f, [](float value, int /* maxLength */)
              { return String(Decibels::gainToDecibels(value), 1) + "dB"; } },
            { ParamID::FrequencyCutoff, { 20.0f, 20000.0f, 0.0f, 0.3f }, 20000.0f, [](float value, int /* maxLength */)
              { if (value >= 1000.0f) return String(value / 1000.0f, 1) + "kHz"; else return String(value) + "Hz"; } },

            { ParamID::FrequencyResonance, { 0.01f, 10.0f, 0.01f }, 0.7f, [](float value, int /* maxLength */)
              {
                  return String(value, 2);
              } },

            { ParamID::FreqShift, { -12.0f, 12.0f, 2.0f }, 0.0f, [](float value, int /**/)
              { return String(value); } },

            { ParamID::WowFactor, { 0.0f, 5.0f, 0.01f }, 0.5f, [](float value, int /**/)
              { return String(value) + "Hz"; } },
            { ParamID::WowDepth, { 0.0f, 1.0f }, 0.0f, [](float value, int /**/)
              { return String(value, 2); } }
        };

        const std::vector<std::tuple<juce::String, juce::StringArray, int>> choiceParameterData {
            { ParamID::PrePostFilter, prePostDelayFilter, 0 },
            { ParamID::FilterType, filterType, 0 }
        };
    }
}
