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
#include "../audio/effects/EQ.hpp"
#include "MIDImapper.hpp"
#include "AudioState.hpp"

enum {playButton = 0, syncButton, inLoopButton, outLoopButton, eightBarButton, halfLoopButton};


class Audio :   public MidiInputCallback,
                public AudioIODeviceCallback,
                public ValueTree::Listener

{
public:
    /** Constructor */
    Audio();
    
    /** Destructor */
    ~Audio();
    
    /** Returns the audio device manager, don't keep a copy of it! */
    AudioDeviceManager& getAudioDeviceManager() { return audioDeviceManager;}
    
    FilePlayer& getFilePlayer(int fileNum);
    
    ValueTree& getState() {return midiMapper.getState();}
    
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
    
    void setChannelGain(float sliderValue, int chanNum);
    
    void setSave(bool save);
    
    void saveAudio();
    
    
    //ValueTreeListener
    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override {};
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {};
    void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {};
    void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) override {};
    
private:
    AudioDeviceManager audioDeviceManager;
    AudioSourcePlayer audioSourcePlayer;
    std::unique_ptr<FilePlayer> filePlayerChannel0;
    std::unique_ptr<FilePlayer> filePlayerChannel1;
    MixerAudioSource mixerAudioSource;
    std::unique_ptr<AudioState> state;
    MidiMapper midiMapper;
    AudioSampleBuffer audioSampleBuffer;
    float gainValChan0 = 1, gainValChan1 = 1;
    
    bool Sync = false;
    bool record = false;
    
};

#endif  // AUDIO_H_INCLUDED
