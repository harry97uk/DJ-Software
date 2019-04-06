//
//  Filter.hpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 26/03/2019.
//

#ifndef Filter_hpp
#define Filter_hpp

#include <stdio.h>
#include "../SimpleFilter.hpp"
#include "../../../JuceLibraryCode/JuceHeader.h"


class Filter
{
public:
    
    Filter();
    
    ~Filter();
    
    float filterSamples(float sample, float startFilterEffectSliderValue, float sampleRate);
    
private:
    CriticalSection filterLock;
    SimpleFilter filter[2];
    float returnSample = 0, overallReturnSample = 0;
};
#endif /* Filter_hpp */
