#include "ParamsData.h"

namespace Constants
{
    constexpr auto valueTreeType = "PARAMETERS";
}

ParamsData::ParamsData(AudioProcessor& processor) :
    m_valueTreeState(processor, nullptr, Constants::valueTreeType, createAllParameters())
{
}

AudioProcessorValueTreeState::ParameterLayout ParamsData::createAllParameters() const
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    for (auto [id, range, defaultValue, stringFromValue] : ModDelay::ParamTuples::floatParameterData)
    {
        auto param = std::make_unique<AudioParameterFloat>(id, id, range, defaultValue, String(), AudioProcessorParameter::genericParameter, stringFromValue);
        params.push_back(std::move(param));
    }

    for (auto [id, options, defaultIndex] : ModDelay::ParamTuples::choiceParameterData)
    {
        auto param = std::make_unique<AudioParameterChoice>(id, id, options, defaultIndex);
        params.push_back(std::move(param));
    }

    for (auto [id, defaultValue] : ModDelay::ParamTuples::boolParameterData)
    {
        auto param = std::make_unique<AudioParameterBool>(id, id, defaultValue);
        params.push_back(std::move(param));
    }

    return { params.begin(), params.end() };
}

AudioProcessorValueTreeState& ParamsData::getAPVTS()
{
    return m_valueTreeState;
}

float ParamsData::getValue(String parameterID) const
{
    const auto rawParamValue = m_valueTreeState.getRawParameterValue(parameterID);
    jassert(rawParamValue);

    const float paramValue = rawParamValue ? rawParamValue->load() : 0.0f;
    return paramValue;
}

int ParamsData::getChoiceValue(String parameterID) const
{
    const auto choice = dynamic_cast<AudioParameterChoice*>(m_valueTreeState.getParameter(parameterID));
    jassert(choice);

    const int choiceValue = choice ? choice->getIndex() : 0;
    return choiceValue;
}

bool ParamsData::getBoolValue(String ParameterID) const
{
    const auto boolParam = dynamic_cast<AudioParameterBool*>(m_valueTreeState.getParameter(ParameterID));
    jassert(boolParam);

    const bool boolValue = boolParam ? boolParam->get() : false;
    return boolValue;
}
