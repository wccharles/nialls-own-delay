/*
 ==============================================================================

 Code by Juan Gil <https://juangil.com/>.
 Copyright (C) 2017-2020 Juan Gil.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

 ==============================================================================
 */

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================

using namespace juce;

class STFT
{
public:
    static constexpr auto fftOrder = 11;
    static constexpr auto fftSize = 1 << fftOrder;
    static constexpr auto overlap = 2;
    static constexpr auto hopSize = fftSize / overlap;

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
