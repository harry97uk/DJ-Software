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
#include "../audio/effects/EQ.hpp"
#include "../audio/Audio.h"
#include "BeatGrid.hpp"
#include "../DRowAudio/ColouredAudioThumbnail.hpp"
#include <thread>


/**
 Gui for the FilePlayer class
 */
class FilePlayerGui :   public Component,
                        public Button::Listener,
                        public Slider::Listener,
                        public Timer,
                        public ChangeListener,
                        public ValueTree::Listener
{
public:
    /**
     Constructor - receives a reference to a FilePlayer object to control
     */
    FilePlayerGui (Audio& audio, bool right);
    
    /**
     Destructor 
     */
    ~FilePlayerGui();
    
    /** Used to arrange objects in the FilePlayerGui */
    void resized() override;
    
    /**Button Listener - called when a button is clicked*/
    void buttonClicked(Button* button) override;
    
    /**Button Listener - called when a button state is changed*/
    void buttonStateChanged(Button* button) override;
    
    /** Slider Listener - called when a slider is moved*/
    void sliderValueChanged(Slider* slider) override;
   
    /** Used to update playback slider position*/
    void timerCallback() override;
    
    /** FileBrowserListener */
    void loadFile(File audioFile);
    
    void paint (Graphics& g) override;
    
    /** Used to draw graphics for when there is no relevant file loaded into the application */
    void paintIfNoFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds);
    
    /** Used to draw an audio waveform thumbnail */
    void paintIfFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds);
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void transportSourceChanged();
    
    /** Updates the thumbnail */
    void thumbnailChanged();
    
    /** Set the current BPM*/
    void setBPM (double bpm);
    
    /** Set the orginal BPM of the loaded audio file */
    void setOriginalBpm (double originalBpm);
    
    /** Set the musical key of the loaded track - visual feedback only */
    void setKey (juce::String key);
    
    /** Set the beat grid start time */
    void setBGS (float BGS);
    
    /** Set the BPM ratio of an audio file compared to its orginal BPM i.e. 1 = normal speed, 2 = double the speed, 0.5 = half the speed */
    void setBpmRatio (double bpmRatio);
    
    /** Set the BPM that the other channel should be synced to */
    void setSyncBpm (double syncBpm);
    
    float getBpm() {return Bpm;}
    float getOBpm() {return OBpm;}
    float getBGS() {return beatGridStart;}
    
    /** Set whether the channel should dictate the BPM or not for when matching BPM's */
    void setMaster (bool isMaster);
    
    bool getMaster();
    
    /** Represents the details of the analysis of an audio file, the key and the BPM */
    void audioAnalysis();
    
    void setSectionPaint(Graphics& g, const Rectangle<int>& sectionBounds);
    
    //void mouseUp(const juce::MouseEvent &event) override;
    
    
    /** @return the value of the gain slider for each file player for use with the overall crossfade slider*/
    float crossfadeFileGainValue();
    
    
    //ValueTreeListener
    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) override {};
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {};
    void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {};
    void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) override {};
private:
    FilePlayer& filePlayer;
    AudioThumbnailCache audioCache;
    ColouredAudioThumbnail audioWaveform;
    std::unique_ptr<BeatGrid> beatgrid;
    std::vector<BeatMarkerData> markers;
    Atomic<bool> loop = false;

    TextButton playButton;
    TextButton fourBarLoopButton, eightBarLoopButton;
    TextButton zoomPlusButton, zoomMinusButton;
    TextButton syncButton;
    //MouseInputSource click;
    //MouseEvent playbackControl;
    Slider playbackPosition;
    Slider fileGain;
    Slider LFreq, MFreq, HFreq;
    Slider bpmSlider;
    Slider filterSlider, delaySlider, reverbSlider;
    Label gain, bass, mid, high, playback;
    Label BPM, Key;
    Label time, totalTime;
    
    String key1;
    float timeInSeconds, totalTimeInSeconds = 0, originalTotalTime = 0;
    bool midiButtonClicked = false;
    float secondStart;
    float zoomNo = 8.0;
    float nextDownBeatTime;
    double secondsPerBeat, Bpm, BpmRatio = 1, OBpm;
    float totalMeasures, waveformRatio = 1.0;
    float beatGridStart;
    double SyncBpm;
    float loopStart;
    bool Master, Right, newMaster;
    
};


#endif  // H_FILEPLAYERGUI
