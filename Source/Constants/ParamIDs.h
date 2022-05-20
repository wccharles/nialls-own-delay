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

        const String ReverbDryWet = "ReverbDryWet";
        const String ReverbDecay = "ReverbDecay";
        const String ReverbDampnessAmount = "ReverbDampnessAmount";
        const String ReverbDampLowFrequency = "ReverbDampLowFreq";
        const String ReverbDampHighFrequency = "ReverbDampHighFreq";
        const String ReverbDampLowFactor = "ReverbDampLowFactor";
        const String ReverbDampHighFactor = "ReverbDampHighFactor";
        const String ReverbPreDelayTime = "ReverbPreDelayTime";
        const String ReverbFreeze = "ReverbFreeze";

    } // namespace ParamID

    const StringArray prePostDelayFilter = { "Pre", "Post" };
    const StringArray filterType = { "LPF", "BPF", "HPF" };

    namespace ParamTuples
    {
        // Tuple of parameter data goes: ID, Range, Default Value.
        const std::vector<std::tuple<String, String, NormalisableRange<float>, float, std::function<String(float, int)>>> floatParameterData {

            { ParamID::Gain, "Master Gain", { 0.0f, 1.0f }, 0.75f, [](float value, int /* maxLength */)
              { return String(Decibels::gainToDecibels(value), 1) + "dB"; } },

            { ParamID::StereoWidth, "Stereo Width", { Decibels::gainToDecibels(0.5f), Decibels::gainToDecibels(2.0f) }, 0.0f, [](float value, int /* maxLength */)
              {
                  return String(Decibels::decibelsToGain(value) * 100.f, 1) + "%";
              } },

            { ParamID::DelayDryWet, "Delay Dry/Wet", { 0.0f, 1.0f }, 0.5f, [](float value, int /* maxLength */)
              { return String(value * 100.f, 1) + "%"; } },
            { ParamID::DelayTime, "Delay Time", { 0.01f, 1.0f, 0.00001f }, 0.25f, [](float value, int /* maxLength */)
              { return String(value * 1000.0f, 2) + "ms"; } },
            { ParamID::DelayFeedback, "Delay Feedback", { 0.0f, 1.0f }, 0.5f, [](float value, int /* maxLength */)
              { return String(value * 100.f, 1) + "%"; } },

            { ParamID::FilterDrive, "Filter Drive", { 0.5f, 2.0f }, 1.0f, [](float value, int /* maxLength */)
              { return String(Decibels::gainToDecibels(value), 1) + "dB"; } },
            { ParamID::FrequencyCutoff, "Filter Cutoff", { 20.0f, 20000.0f, 0.0f, 0.3f }, 20000.0f, [](float value, int /* maxLength */)
              { if (value >= 1000.0f) return String(value / 1000.0f, 1) + "kHz"; else return String(value) + "Hz"; } },

            { ParamID::FrequencyResonance, "Filter Resonance", { 0.01f, 5.0f, 0.01f }, 0.7f, [](float value, int /* maxLength */)
              {
                  return String(value, 2);
              } },

            { ParamID::FreqShift, "Frequency Shift", { -12.0f, 12.0f, 2.0f }, 0.0f, [](float value, int /**/)
              { return String(value); } },

            { ParamID::WowFactor, "Modulation Rate", { 0.0f, 5.0f, 0.01f }, 0.5f, [](float value, int /**/)
              { return String(value) + "Hz"; } },
            { ParamID::WowDepth, "Modulation Depth", { 0.0f, 1.0f }, 0.0f, [](float value, int /**/)
              { return String(value, 2); } },

            { ParamID::ReverbDryWet, "Reverb Dry/Wet", { 0.0f, 1.0f }, 0.5f, [](float value, int /* maxLength */)
              { return String(value * 100.f, 1) + "%"; } },
            { ParamID::ReverbDecay, "Reverb Decay", { 0.25f, 10.0f, 0.01f }, 0.25f, [](float value, int /* maxLength */)
              { return String(value, 2) + "s"; } },
            { ParamID::ReverbDampnessAmount, "Reverb Dampness", { 0.0f, 1.0f }, 0.5f, [](float value, int /* maxLength */)
              { return String(value * 100.0f, 1) + "%"; } },
            { ParamID::ReverbDampLowFrequency, "Low Shelf Damping Frequency", { 20.0f, 20000.0f, 0.0f, 0.3f }, 250.0f, [](float value, int /* maxLength */)
              { if (value >= 1000.0f) return String(value / 1000.0f, 1) + "kHz"; else return String(value) + "Hz"; } },
            { ParamID::ReverbDampHighFrequency, "High Shelf Damping Frequency", { 20.0f, 20000.0f, 0.0f, 0.3f }, 4000.0f, [](float value, int /* maxLength */)
              { if (value >= 1000.0f) return String(value / 1000.0f, 1) + "kHz"; else return String(value) + "Hz"; } },
            { ParamID::ReverbDampLowFactor, "Low Shelf Damping Amplitude", { 0.01f, 1.0f }, 1.0f, [](float value, int /**/)
              { return String(value, 2); } },
            { ParamID::ReverbDampHighFactor, "High Shelf Damping Amplitude", { 0.01f, 1.0f }, 1.0f, [](float value, int /**/)
              { return String(value, 2); } },
            { ParamID::ReverbPreDelayTime, "FDN Pre Delay", { 0.01f, 0.25f, 0.00001f }, 0.05f, [](float value, int /* maxLength */)
              { return String(value * 1000.0f, 2) + "ms"; } },
        };

        const std::vector<std::tuple<String, String, StringArray, int>> choiceParameterData {
            { ParamID::PrePostFilter, "Pre/Post Delay Filter", prePostDelayFilter, 0 },
            { ParamID::FilterType, "Filter Type", filterType, 0 }
        };

        const std::vector<std::tuple<String, String, bool>> boolParameterData {
            { ParamID::ReverbFreeze, "Reverb Freeze", false }
        };
    }
}
