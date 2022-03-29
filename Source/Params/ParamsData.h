#pragma once

#include "ParamIDs.h"
#include <JuceHeader.h>

class ParamsData
{
public:
    ParamsData(AudioProcessor& processor);

    AudioProcessorValueTreeState& getAPVTS();

    float getValue(String parameterID) const;
    int   getChoiceValue(String parameterID) const;
    bool  getBoolValue(String parameterID) const;

private:
    AudioProcessorValueTreeState::ParameterLayout createAllParameters() const;

private:
    AudioProcessorValueTreeState m_valueTreeState;
};
