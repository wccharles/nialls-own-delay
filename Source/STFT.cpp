#include "STFT.h"

STFT::STFT() :
    m_fft(log2(fftSize)),
    m_freqShift(0.0)
{
}

void STFT::setup(double sampleRate, const int numChannels)
{
    const double smoothTime = 1e-3;
    m_freqShift.reset(sampleRate, smoothTime);
    setFftSize(numChannels);
    outputBufferWritePosition = hopSize % outputBufferLength;
    updateWindow();
}

void STFT::updateParameters(const float freqShift)
{
    m_freqShift.setTargetValue(freqShift);
}

const float STFT::processSample(const float input, const int channel)
{
    float output;

    currentInputBufferWritePosition = inputBufferWritePosition;
    currentOutputBufferWritePosition = outputBufferWritePosition;
    currentOutputBufferReadPosition = outputBufferReadPosition;
    currentSamplesSinceLastFFT = samplesSinceLastFFT;

    inputBuffer.setSample(channel, currentInputBufferWritePosition, input);
    if (++currentInputBufferWritePosition >= inputBufferLength)
        currentInputBufferWritePosition = 0;

    output = outputBuffer.getSample(channel, currentOutputBufferReadPosition);
    outputBuffer.setSample(channel, currentOutputBufferReadPosition, 0.0f);
    if (++currentOutputBufferReadPosition >= outputBufferLength)
        currentOutputBufferReadPosition = 0;

    if (++currentSamplesSinceLastFFT >= hopSize)
    {
        currentSamplesSinceLastFFT = 0;

        analysis(channel);
        modification();
        synthesis(channel);
    }

    return output;
}

void STFT::updatePointers()
{
    inputBufferWritePosition = currentInputBufferWritePosition;
    outputBufferWritePosition = currentOutputBufferWritePosition;
    outputBufferReadPosition = currentOutputBufferReadPosition;
    samplesSinceLastFFT = currentSamplesSinceLastFFT;
}

void STFT::setFftSize(const int numChannels)
{
    inputBufferLength = fftSize;
    inputBuffer.clear();
    inputBuffer.setSize(numChannels, inputBufferLength);

    outputBufferLength = fftSize;
    outputBuffer.clear();
    outputBuffer.setSize(numChannels, outputBufferLength);

    fftWindow.realloc(fftSize);
    fftWindow.clear(fftSize);

    timeDomainBuffer.realloc(fftSize);
    timeDomainBuffer.clear(fftSize);

    frequencyDomainBuffer.realloc(fftSize);
    frequencyDomainBuffer.clear(fftSize);

    prevFrequencyDomainSamples.realloc(fftSize);
    prevFrequencyDomainSamples.clear(fftSize);

    inputBufferWritePosition = 0;
    outputBufferWritePosition = 0;
    outputBufferReadPosition = 0;
    samplesSinceLastFFT = 0;
}

void STFT::updateWindow()
{
    for (int sample = 0; sample < fftSize; ++sample)
    {
        fftWindow[sample] = 0.5f - 0.5f * cosf(2.0f * M_PI * (float) sample / (float) (fftSize - 1));
    }

    float windowSum = 0.0f;
    for (int sample = 0; sample < fftSize; ++sample)
    {
        windowSum += fftWindow[sample];
    }

    windowScaleFactor = 0.0f;
    if (windowSum != 0.0f)
    {
        windowScaleFactor = 1.0f / (float) overlap / windowSum * (float) fftSize;
    }
}

//======================================

void STFT::analysis(const int channel)
{
    int inputBufferIndex = currentInputBufferWritePosition;
    for (int index = 0; index < fftSize; ++index)
    {
        timeDomainBuffer[index].real(fftWindow[index] * inputBuffer.getSample(channel, inputBufferIndex));
        timeDomainBuffer[index].imag(0.0f);

        if (++inputBufferIndex >= inputBufferLength)
            inputBufferIndex = 0;
    }
}

void STFT::modification()
{
    m_fft.perform(timeDomainBuffer, frequencyDomainBuffer, false);

    for (int index = 0; index < fftSize / 2 + 1; ++index)
    {
        prevFrequencyDomainSamples[index].real(frequencyDomainBuffer[index].real());
        prevFrequencyDomainSamples[index].imag(frequencyDomainBuffer[index].imag());
        dsp::Complex<float> tempFreqBinValue;

        if (index > 0 && index > static_cast<int>(m_freqShift.getNextValue()) && index - static_cast<int>(m_freqShift.getNextValue()) < fftSize / 2)
        {
            tempFreqBinValue.real(prevFrequencyDomainSamples[index - (int) m_freqShift.getNextValue()].real());
            tempFreqBinValue.imag(prevFrequencyDomainSamples[index - (int) m_freqShift.getNextValue()].imag());
        }

        frequencyDomainBuffer[index].real(tempFreqBinValue.real());
        frequencyDomainBuffer[index].imag(tempFreqBinValue.imag());

        if (index != fftSize / 2)
        {
            frequencyDomainBuffer[fftSize - index].real(tempFreqBinValue.real());
            frequencyDomainBuffer[fftSize - index].imag(-tempFreqBinValue.imag());
        }
    }

    m_fft.perform(frequencyDomainBuffer, timeDomainBuffer, true);
}

void STFT::synthesis(const int channel)
{
    int outputBufferIndex = currentOutputBufferWritePosition;
    for (int index = 0; index < fftSize; ++index)
    {
        float outputSample = outputBuffer.getSample(channel, outputBufferIndex);
        outputSample += timeDomainBuffer[index].real() * windowScaleFactor;
        outputBuffer.setSample(channel, outputBufferIndex, outputSample);

        if (++outputBufferIndex >= outputBufferLength)
            outputBufferIndex = 0;
    }

    currentOutputBufferWritePosition += hopSize;
    if (currentOutputBufferWritePosition >= outputBufferLength)
        currentOutputBufferWritePosition = 0;
}
