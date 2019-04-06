//
//  Filter.cpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 26/03/2019.
//

#include "Filter.hpp"

Filter::Filter() : filter{44100, 44100}
{
    ScopedLock sl(filterLock);
    filter[0].FilterConfig(kLPF, 25000, 0);
    filter[1].FilterConfig(kHPF, 0, 0);
}

Filter::~Filter()
{
    
}

float Filter::filterSamples(float sample, float startFilterEffectSliderValue, float sampleRate)
{
    ScopedLock sl(filterLock);
    returnSample = sample;
    
  
    
    if (startFilterEffectSliderValue < 0)
    {
        float filterScale = (startFilterEffectSliderValue * (sampleRate/2)) + ((sampleRate/2) + 500);
        filter[0].FilterConfig(kLPF, filterScale, 0);
        return filter[0].Filter(sample);
    }
    else if (startFilterEffectSliderValue >= 0)
    {
        float filterScale = (startFilterEffectSliderValue * 2000);
        filter[1].FilterConfig(kHPF, filterScale, 0);
        returnSample = filter[1].Filter(sample);
    }
    
    
    
    return returnSample;
}
