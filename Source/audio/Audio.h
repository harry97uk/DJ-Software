/*
  ==============================================================================

    Audio.h
    Created: 13 Nov 2014 8:14:40am
    Author:  Tom Mitchell

  ==============================================================================
*/

#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

/**
 Class containing all audio processes
 */

#include "../../JuceLibraryCode/JuceHeader.h"
#include "FilePlayer.h"
#include "EQ.hpp"


class Audio :   public MidiInputCallback,
                public AudioIODeviceCallback

{
public:
    /** Constructor */
    Audio();
    
    /** Destructor */
    ~Audio();
    
    /** Returns the audio device manager, don't keep a copy of it! */
    AudioDeviceManager& getAudioDeviceManager() { return audioDeviceManager;}
    
    FilePlayer& getFilePlayer(int fileNum) { return filePlayer[fileNum]; }
    
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override;
    
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples) override;
    void audioDeviceAboutToStart (AudioIODevice* device) override;
    void audioDeviceStopped() override;
    
    /** Controls the gain for both file players*/
    void masterGain(float sliderValue);
    
    /** Controls how the crossfase works
     @param sliderValue Decides the level of the crossfade from a slider with range -1 to 1
     @param FileGain is the value of the file gain as constant*/
    void crossfadeGain(float sliderValue, float FileGain, float FileGain1);
    
private:
    AudioDeviceManager audioDeviceManager;
    AudioSourcePlayer audioSourcePlayer;
    FilePlayer filePlayer[2];
    MixerAudioSource mixerAudioSource;
    
    
};

#endif  // AUDIO_H_INCLUDED
