//
//  EQ.hpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 09/01/2018.
//

#ifndef EQ_hpp
#define EQ_hpp

#include <stdio.h>
#include "SimpleFilter.hpp"
#include "../../JuceLibraryCode/JuceHeader.h"

/** An enumeration to select the frequency range of the audio*/
enum { kBass = 0, kMid, kHigh};

/** A class that allows control over the bass, mid, high frequencies of the audio being played*/
class EQ
{
public:
    /** Constructor */
    EQ();
    
    /** Destructor */
    ~EQ();
    
    /** Reads the slider value and applies the value to the corresponding frequency range
     @param sliderValue The slider value that represents the gain of which ever frequency range
     @param freqGainNum Allows for the specific frequency range to be selected*/
    void setFreqGain(float sliderValue, int freqGainNum);
    
    /**Reads the samples and filters them based on the frequency range picked
     @param sample The incoming sample
     @param type The frequency range to filter, this is what the enumeration above is in refernce to*/
    float filterSamples(float sample, UInt16 type);
    
    
    /** @param freqGainNum Selects the specific frequency range
     @return Returns the gain value of the selected frequency range*/
    float getFreqGain(int freqGainNum);
    

    
    
private:
    CriticalSection eqLock;
    SimpleFilter filter[3];
    float freqGain[3];
    float bassGain, midGain, highGain;
    UInt16 iType;
    
};

#endif /* EQ_hpp */
