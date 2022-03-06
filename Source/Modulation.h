#pragma once

#include <JuceHeader.h>

class Modulation {
public:
    Modulation(){
        // auto& random = Random::getSystemRandom();
    }
    ~Modulation(){}

    void modParams (float frequency, float sampleRate)
    {
        increment = 2 * M_PI * frequency / sampleRate;
    }

    float processSample ()
    {
        float output = sinf(phase);

        phase += increment;

        if (phase > 2 * M_PI)
            phase -= 2 * M_PI;

        float upper = output + 0.5 <= 1 ? output + 0.5 : 1;
        float lower = output - 0.5 >= 0 ? output - 0.5 : 0;

        auto noise = random.nextFloat ();

        return output * noise;
    }

private:

    float phase;
    float increment;

    Random random;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Modulation)
};