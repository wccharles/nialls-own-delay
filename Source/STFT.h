#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================

using namespace juce;

class STFT
{
public:
    STFT();
    ~STFT() {}

    void        setup(const double sampleRate, const int numChannels);
    void        updateParameters(const float freqShift);
    const float processSample(const float input, const int channel);
    void        updatePointers();

private:
    void setFftSize(const int numChannels);
    void updateWindow();
    //======================================
    void analysis(const int channel);
    void modification();
    void synthesis(const int channel);

private:
    int               inputBufferLength;
    AudioSampleBuffer inputBuffer;

    int               outputBufferLength;
    AudioSampleBuffer outputBuffer;

    dsp::FFT                       m_fft;
    HeapBlock<float>               fftWindow;
    HeapBlock<dsp::Complex<float>> timeDomainBuffer;
    HeapBlock<dsp::Complex<float>> frequencyDomainBuffer;
    HeapBlock<dsp::Complex<float>> prevFrequencyDomainSamples;

    SmoothedValue<float> m_freqShift;

    float windowScaleFactor;

    int inputBufferWritePosition;
    int outputBufferWritePosition;
    int outputBufferReadPosition;
    int samplesSinceLastFFT;

    int currentInputBufferWritePosition;
    int currentOutputBufferWritePosition;
    int currentOutputBufferReadPosition;
    int currentSamplesSinceLastFFT;
};

//==============================================================================
