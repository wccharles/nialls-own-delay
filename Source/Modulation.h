#pragma once

#include <JuceHeader.h>

class Modulation
{
public:
    Modulation()
    {
        // auto& random = Random::getSystemRandom();
    }
    ~Modulation() {}

    void modParams(float frequency, float sampleRate)
    {
        increment = 2 * M_PI * frequency / sampleRate;
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock, int channels)
    {
        dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = channels;

        mod_osc.prepare(spec);
        mod_osc.setFrequency(10);
    }

    float processSample(float input)
    {
        // float output = sinf (phase);

        // phase += increment;

        // if (phase > 2 * M_PI)
        //     phase -= 2 * M_PI;

        // float upper = output + 0.5 <= 1 ? output + 0.5 : 1;
        // float lower = output - 0.5 >= 0 ? output - 0.5 : 0;

        auto noise = random.nextFloat();

        auto output = mod_osc.processSample(input);

        return output /* * noise */;
    }

private:
    float phase;
    float increment;

    dsp::Oscillator<float> mod_osc {
        [](float x)
        { return std::sin(x); }
    };

    Random random;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Modulation)
};
