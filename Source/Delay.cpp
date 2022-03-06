#include "Delay.h"

void Delay::prepareToPlay (int readPointer)
{
    delayBuffer.clear();

    writeHeadPos = 0;

    writeHead = std::make_unique<LagrangeInterpolator>();
    readHead  = std::make_unique<LagrangeInterpolator>();

    readHeadPos = (writeHeadPos - readPointer + delayBufferSize) % delayBufferSize;

    inputBuffer.setSize (1, 100);
    outputBuffer.setSize (1, 100);

    isActive = true;
}

void Delay::delayParams (bool isActive, float delayTime)
{
    this->isActive  = isActive;
    this->delayTime = delayTime;
}

void Delay::reset()
{
    delayBuffer.clear();
    writeHead->reset();
    readHead.reset();
}

void Delay::writeSample (float sample)
{
    // auto inData = buffer.getArrayOfReadPointers();
    // auto outData = buffer.getArrayOfWritePointers();

    // auto delayRData = delayBuffer.getArrayOfReadPointers();
    // auto delayWData = delayBuffer.getArrayOfWritePointers();

    if (isActive)
    {
        int numInputSamplesNeeded = ceil (currentSpeed);

        if (currentSpeed < 1)
            numInputSamplesNeeded++;

        //     if ()

        for (int i = 0; i < ceil (1 / currentSpeed); ++i)
        {
            delayBuffer.setSample (0, writeHeadPos, sample);
            writeHeadPos++;

            if (writeHeadPos >= delayBufferSize)
                writeHeadPos -= delayBufferSize;
        }

        currentSpeed = speed;
    }
}

float Delay::readSample()
{
}