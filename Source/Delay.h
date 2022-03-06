#pragma once

#define MAX_BUFFER_SIZE 2

#include <JuceHeader.h>

class Delay
{
public:
    Delay (int channels, float fs) : chans (channels),
                                     sampleRate (fs)
    {
        isActive        = false;
        currentSpeed    = 1;
        delayBufferSize = static_cast<int> (MAX_BUFFER_SIZE * sampleRate);
        DBG ("Delay Buffer Size: " << delayBufferSize);
        delayBuffer.setSize (chans, delayBufferSize);
    }

    ~Delay() {}

    void  prepareToPlay (int readPointer);
    void  delayParams (bool isActive, float delayTime);
    void  writeSample (float sample);
    float readSample();
    void  reset();

private:
    int   chans;
    float sampleRate;

    AudioSampleBuffer delayBuffer;
    AudioSampleBuffer inputBuffer;
    AudioSampleBuffer outputBuffer;

    int delayBufferSize;

    int writeHeadPos { 0 };
    int readHeadPos { 0 };

    float speed, currentSpeed, delayTime;

    std::unique_ptr<LagrangeInterpolator> writeHead { nullptr };
    std::unique_ptr<LagrangeInterpolator> readHead { nullptr };

    bool isActive;

    // ADSR adsr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Delay)
};
