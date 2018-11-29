//
//  BeatGrid.cpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 24/11/2018.
//

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

class BeatGrid
{
public:
    
    void beatGridStartProcess(float time, float BGS, int width, float secondsPerBeat, float totalTime, float bpm)
    {
        float bps = bpm/60;
        
        int totalBeats = totalTime * bps;
        startBeats = totalBeats/4;
        betweenBeats = totalBeats - startBeats;
        beatNo = totalBeats;
        
        float tempBgsValue[startBeats];
        
        for (int counter = 0; counter < startBeats; counter++)
        {
            
            tempBgsValue[counter] = ((BGS/4000) * width) + (((((4 * (counter)) * secondsPerBeat) * 1000)/4000) * width);
            float tempTimeValue = ((time * 1000)/4000) * width;
            
            BGSValue[counter] = tempBgsValue[counter] - tempTimeValue;
        }

    }
    
    void beatGridOtherProcess(float time, float BGS, float secondsPerBeat, int width)
    {
        float tempBgValue[betweenBeats];
        float tempTimeValue = ((time * 1000)/4000) * width;
        
        for (int counter = 0; counter < betweenBeats; counter++)
        {
            tempBgValue[counter] = ((BGS/4000) * width) + ((((secondsPerBeat * (counter + 1)) * 1000)/4000) * width);
            otherBGSValue[counter] = tempBgValue[counter] - tempTimeValue;
            
            if (otherBGSValue[counter] == BGSValue[(counter * 3) + (counter - 1)])
            {
                otherBGSValue[counter] = -1;
            }
        }

    }
    
    int getBeatNo(int beatType)
    {
        if (beatType == 0)
        {
            return startBeats;
        }
        else if (beatType == 1)
        {
            return betweenBeats;
        }
        else
            return 0;
        
        
    }
    
    float getBgsPosition(int beatNo)
    {
        return BGSValue[beatNo];
    }
    
    float getOtherBgPosition(int beatNo)
    {
        return otherBGSValue[beatNo];
    }
    
private:
    
    float BGSValue[2048];
    float otherBGSValue[2048];
    int startBeats, betweenBeats;
    int beatNo;
    
};
