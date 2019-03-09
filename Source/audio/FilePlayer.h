/*
  ==============================================================================

    FilePlayer.h
    Created: 22 Jan 2013 2:49:14pm
    Author:  tj3-mitchell

  ==============================================================================
*/

#ifndef H_FILEPLAYER
#define H_FILEPLAYER

#include "../JuceLibraryCode/JuceHeader.h"
#include "EQ.hpp"
#include "Delay.hpp"
#include "DJAudioSource.hpp"

/**
 Simple FilePlayer class - streams audio from a file.
 */
class FilePlayer : public AudioSource
{
public:
    /**
     Constructor
     */
    FilePlayer();
    
    /**
     Destructor
     */
    ~FilePlayer();
    
    /**
     Starts or stops playback of the looper
     */
    void setPlaying (const bool newState);
    
    /**
     Gets the current playback state of the looper
     */
    bool isPlaying () const;
    
    /**
     Loads the specified file into the transport source
     @param newFile The file loaded in the FilePlayerGui FileBrowserComponent
     @see FilePlayerGui
     */
    void loadFile (const File& newFile);
    
    /** sets the position as a percentage of the total song in seconds
     @param newPosition a value of 0 to 1 that is available on a slider at the top*/
    void setPosition (float newPosition);
    
    /** @return the position of the song as a percentage of the total song in seconds */
    float getPosition();
    
    double getLengthInSeconds();
    
    /** sets the amplitude of the audio coming from the file player
     @param sliderValue the value of a slider with the range 0 to 1*/
    void setGain (float sliderValue);
    
    /** @return the value of the amplitude in a range from 0 o 1*/
    float getGain();
    
    void setEqFreqGain (float gain, int eqIndex);
    
    //void setLooping(bool newState, float startPos, float secondsPerBeat);
    
    float getNextSample();
    
    AudioFormatManager& getFormatManager() {return formatManager;}
    
    File& getFile() {return waveformFile;}
    
    DJAudioSource& getATS() {return audioSource;}
    
    float getSampleRate() {return sRate;}
    
    int64 getTotalSamples();
    
    void setBpmRatio (double bpmRatio);
    
    void setFilterValue (float filterVal);
    
    void setDelayValue (float delayVal);
    
    void setReverbValue (float reverbVal);
    
    //AudioSource
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
private:
    TimeSliceThread thread;//thread for the transport source
    AudioFormatReaderSource* currentAudioFileSource;    //reads audio from the file
    std::unique_ptr<AudioFormatReader> reader;
    DJAudioSource audioSource;
    
                                            // starting/stopping and sample-rate conversion
    
    EQ eq[2];
    DelaySignal delay[2];
    DelaySignal reverb[2][16];
    AudioFormatManager formatManager;
    CriticalSection loopLock;
    AudioSourceChannelInfo fullAudio;
    
    int64 totalSamples;
    int samplesPerFrame;
    double sRate;
    float filterValue = 0, delayValue = 0, reverbValue = 0;
    double BpmRatio;
    Random reverbDelay;
    File waveformFile;
    
    
    
    
};

#endif  // H_FILEPLAYER
