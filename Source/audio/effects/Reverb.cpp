//
//  Reverb.cpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 27/03/2019.
//

#include "Reverb.hpp"


myReverb::myReverb() : filter(44100)
{
    filter.FilterConfig(kBPF, 1500, 1500);
}

myReverb::~myReverb()
{
    
}

void myReverb::initialize (float sampleRate)
{
    fSR = sampleRate;
    iBufferSize = (SInt32)(2.0 * fSR);
    pfCircularBuffer = new Float32[iBufferSize];
    filter.FilterConfig(kLPF, fSR/2, 0);
    
    
    for (int iPos = 0; iPos < iBufferSize; iPos++)
    {
        pfCircularBuffer[iPos] = 0;
        
    }
    
    iBufferWritePos = 0;
    
}

Float32 myReverb::read(float delayTime)
{
    SInt32 channelReadPos;
    Float32 value;

    
    channelReadPos = iBufferWritePos - ((delayTime * 5) * fSR);
        
        
        if (channelReadPos < 0)
        {
            channelReadPos = channelReadPos + iBufferSize;
        }
        
        value = pfCircularBuffer[channelReadPos];
    
    value = filter.Filter(value);

    
    return value;
}

void myReverb::write (float channel)
{
    iBufferWritePos++;
    
    if (iBufferWritePos > (iBufferSize - 1))
    {
        iBufferWritePos = 0;
    }
    
    pfCircularBuffer[iBufferWritePos] = channel;
    
}

void myReverb::originalSample(float sample, float gain)
{
    oSample = sample;
    oGain = gain;
}


