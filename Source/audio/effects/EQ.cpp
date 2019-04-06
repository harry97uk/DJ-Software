//
//  EQ.cpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 09/01/2018.
//

#include "EQ.hpp"


EQ::EQ() : filter{44100, 44100, 44100, 44100, 44100}

{
    ScopedLock sl(eqLock);
    filter[0].FilterConfig(kLPF, 400, 0);
    filter[1].FilterConfig(kBPF, 1100, 1800);
    filter[2].FilterConfig(kHPF, 2000, 0);
    filter[4].FilterConfig(kBPF, 2000, 3800);

    
    for(int counter = 0; counter < 3; counter++)
    {
        freqGain[counter] = 0;
    }
    
    
}

EQ::~EQ()
{
    
}

void EQ::setFreqGain(float sliderValue, int freqGainNum)
{
    ScopedLock sl(eqLock);
    freqGain[freqGainNum] = sliderValue;
}

float EQ::getFreqGain(int freqGainNum)
{
    ScopedLock sl(eqLock);
    float gain = freqGain[freqGainNum];
    return gain;
}


float EQ::filterSamples(float sample, UInt16 type)
{
    ScopedLock sl(eqLock);
    
    
    iType = type;
    float returnSample = sample;
    
    if (iType == kBass)
    {
        returnSample = filter[0].Filter(sample);
    }
    else if (iType == kMid)
    {
        returnSample = filter[1].Filter(sample);
    }
    else if (iType == kHigh)
    {
        returnSample = filter[2].Filter(sample);
    }
    else if (iType == kReverbFilter)
    {
        returnSample = filter[4].Filter(sample);
    }
    
    return returnSample;
}



