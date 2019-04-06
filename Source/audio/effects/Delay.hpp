//
//  Delay.hpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 03/03/2019.
//

#ifndef Delay_hpp
#define Delay_hpp

#include <stdio.h>
#include "../../../JuceLibraryCode/JuceHeader.h"

class DelaySignal
{
public:
    
    DelaySignal();
    
    ~DelaySignal();
    
    void initialize (float sampleRate);
    
    Float32 read (float delayTime);
    
    void write (float channel);

    
private:
    Float32 *pfCircularBuffer;
    Float32 fSR;
    SInt32 iBufferSize, iBufferWritePos;
    float delayTime = 0;
};

#endif /* Delay_hpp */
