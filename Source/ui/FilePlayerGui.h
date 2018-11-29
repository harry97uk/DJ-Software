/*
  ==============================================================================

    FilePlayerGui.h
    Created: 22 Jan 2013 2:49:07pm
    Author:  tj3-mitchell

  ==============================================================================
*/

#ifndef H_FILEPLAYERGUI
#define H_FILEPLAYERGUI

#include "../JuceLibraryCode/JuceHeader.h"
#include "../audio/FilePlayer.h"
#include "../audio/EQ.hpp"
#include "../audio/Audio.h"
#include "BeatGrid.cpp"
#include <thread>


/**
 Gui for the FilePlayer class
 */
class FilePlayerGui :   public Component,
                        public Button::Listener,
                        public Slider::Listener,
                        public Timer,
                        public ChangeListener
{
public:
    /**
     constructor - receives a reference to a FilePlayer object to control
     */
    FilePlayerGui(FilePlayer& filePlayer_, EQ& eq_);
    
    /**
     Destructor 
     */
    ~FilePlayerGui();
    
    /** Used to arrange objects in the FilePlayerGui */
    void resized() override;
    
    /**Button Listener - called when a button is clicked*/
     void buttonClicked(Button* button) override;
    
    /** Slider Listener - called when a slider is moved*/
    void sliderValueChanged(Slider* slider) override;
   
    /** Used to update playback slider position*/
    void timerCallback() override;
    
    /** FileBrowserListener */
    void loadFile(File audioFile);
    
    void paint (Graphics& g) override;
    
    void paintIfNoFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds);
    
    void paintIfFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds);
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void transportSourceChanged();
    
    void thumbnailChanged();
    
    void audioAnalysis(float bpm, String key, float BGS);
    
    
    
    //void mouseUp(const juce::MouseEvent &event) override;
    
    
    /** @return the value of the gain slider for each file player for use with the overall crossfade slider*/
    float crossfadeFileGainValue();
private:
    FilePlayer& filePlayer;
    EQ& eq;
    AudioThumbnailCache audioCache;
    AudioThumbnail audioWaveform;
    BeatGrid beatGrid;
    Atomic<bool> loop;
    
    TextButton playButton;
    TextButton startLoopButton, endLoopButton;
    //MouseInputSource click;
    //MouseEvent playbackControl;
    Slider playbackPosition;
    Slider fileGain;
    Slider LFreq, MFreq, HFreq;
    Label gain, bass, mid, high, playback;
    Label BPM, Key;
    Label time, totalTime;
    float timeInSeconds, totalTimeInSeconds, pValue;
    float secondsPerBeat, bpm1;
    float beatGridStart;
    
};


#endif  // H_FILEPLAYERGUI
