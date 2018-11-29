//
//  TimeStretching.cpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 12/11/2018.
//

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "/Users/harrygardiner/Desktop/Dissertation Stuff/SuperpoweredSDK/Superpowered/SuperpoweredTimeStretching.h"
#include "/Users/harrygardiner/Desktop/Dissertation Stuff/SuperpoweredSDK/Superpowered/SuperpoweredAudioBuffers.h"



class TimeStretching
: public ThreadWithProgressWindow
{
public:
    TimeStretching (): ThreadWithProgressWindow("Analysing..."
                                                , false
                                                , false),
                       timeStretch(nullptr)
    
    {}
    
    void run() override
    
    {
        timeStretch = new SuperpoweredTimeStretching(sRate);
        outputBuffers = new SuperpoweredAudiopointerList(8, 16);
        
        SuperpoweredAudiobufferlistElement inputBuffer;
        
        timeStretch->setRateAndPitchShift(2.0, 0);
        
            inputBuffer.samplePosition = sSample;
            inputBuffer.startSample = 0;
            inputBuffer.samplesUsed = 0;
            inputBuffer.endSample = sPerFrame; // <-- Important!
            inputBuffer.buffers[0] = SuperpoweredAudiobufferPool::getBuffer(sPerFrame * 8 + 64);
            inputBuffer.buffers[1] = inputBuffer.buffers[2] = inputBuffer.buffers[3] = NULL;
            
            timeStretch->process(&inputBuffer, outputBuffers);
            
            if (outputBuffers->makeSlice(0, outputBuffers->sampleLength)) {
                
                while (true) { // Iterate on every output slice.
                    // Get pointer to the output samples.
                    int numSamples = 0;
                    float *timeStretchedAudio = (float *)outputBuffers->nextSliceItem(&numSamples);
                    if (!timeStretchedAudio) break;
                    
                };
                
                // Clear the output buffer list.
                outputBuffers->clear();
            };
            
        };
    
    void threadComplete (bool userPressedCancel) override
    {
        
    }
    
    void setBufferDetails(double sampleRate, int samplesPerFrame, int samplePosition, int numSamples)
    {
        sRate = sampleRate;
        sPerFrame = samplesPerFrame;
        sSample = samplePosition;
        nSamples = numSamples;
    }
    

    SuperpoweredTimeStretching *timeStretch;
    SuperpoweredAudiopointerList *outputBuffers;
    
    double sRate;
    int sPerFrame, sSample, nSamples;
};










