#include "ParamsData.h"

namespace Constants
{
    constexpr auto valueTreeType = "PARAMETERS";
}

ParamsData::ParamsData(juce::AudioProcessor& processor) :
    m_valueTreeState(processor, nullptr, Constants::valueTreeType, createAllParameters())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout ParamsData::createAllParameters() const
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    for (auto [id, range, defaultValue] : ModDelay::ParamTuples::floatParameterData)
    {
        auto param = std::make_unique<juce::AudioParameterFloat>(id, id, range, defaultValue);
        params.push_back(std::move(param));
    };

    return { params.begin(), params.end() };
}

juce::AudioProcessorValueTreeState& ParamsData::getAPVTS()
{
    return m_valueTreeState;
}

float ParamsData::getValue(juce::String parameterID) const
{
    const auto rawParamValue = m_valueTreeState.getRawParameterValue(parameterID);
    jassert(rawParamValue);

    const float paramValue = rawParamValue ? rawParamValue->load() : 0.0f;
    return paramValue;
}
