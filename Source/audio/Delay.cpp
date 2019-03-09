//
//  Delay.cpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 03/03/2019.
//

#include "Delay.hpp"


DelaySignal::DelaySignal()
{
    
}

DelaySignal::~DelaySignal()
{
    
}

void DelaySignal::initialize (float sampleRate)
{
    fSR = sampleRate;
    iBufferSize = (SInt32)(2.0 * fSR);
    pfCircularBuffer = new Float32[iBufferSize];
    
    
    for (int iPos = 0; iPos < iBufferSize; iPos++)
    {
        pfCircularBuffer[iPos] = 0;
        
    }
    
    iBufferWritePos = 0;
    
}

Float32 DelaySignal::read (float delayTime)
{
    SInt32 channelReadPos;
    Float32 value;
    
    channelReadPos = iBufferWritePos - (delayTime * fSR);
    
    
    if (channelReadPos < 0)
    {
        channelReadPos = channelReadPos + iBufferSize;
    }
    
    value = pfCircularBuffer[channelReadPos];
    
    return value;
}

void DelaySignal::write (float channel)
{
    iBufferWritePos++;
    
    if (iBufferWritePos > (iBufferSize - 1))
    {
        iBufferWritePos = 0;
    }
    
    pfCircularBuffer[iBufferWritePos] = channel;
    
}
