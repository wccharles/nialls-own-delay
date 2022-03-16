#pragma once

#include "ParamIDs.h"
#include <JuceHeader.h>

class ParamsData
{
public:
    ParamsData(juce::AudioProcessor& processor);

    juce::AudioProcessorValueTreeState& getAPVTS();

    float getValue(juce::String parameterID) const;
    int   getChoiceValue(String parameterID) const;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createAllParameters() const;

private:
    juce::AudioProcessorValueTreeState m_valueTreeState;
};
