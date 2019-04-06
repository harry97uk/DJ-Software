//
//  Reverb.hpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 27/03/2019.
//

#ifndef Reverb_hpp
#define Reverb_hpp

#include <stdio.h>
#include <random>
#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../SimpleFilter.hpp"


class myReverb
{
public:
    
    myReverb();
    
    ~myReverb();
    
    void initialize (float sampleRate);
    
    Float32 read (float delayTime);
    
    void write (float channel);
    
    void originalSample(float sample, float gain);
    
    Float32 reverbRamp(float delayStartValue, float delayEndValue, int numSamples);
    
    
private:
    Float32 *pfCircularBuffer;
    Float32 fSR;
    SInt32 iBufferSize, iBufferWritePos;
    float reverbTime = 0;
    float oSample = 0, oGain = 0;
    SimpleFilter filter;
    
    
};

#endif /* Reverb_hpp */
