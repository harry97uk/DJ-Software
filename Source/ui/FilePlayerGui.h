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
                        public ChangeListener
{
public:
    /**
     constructor - receives a reference to a FilePlayer object to control
     */
    FilePlayerGui (FilePlayer& filePlayer_, bool right);
    
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
    
    void setBPM (double bpm);
    
    void setOriginalBpm (double originalBpm);
    
    void setKey (juce::String key);
    
    void setBGS (float BGS);
    
    void setBpmRatio (double bpmRatio);
    
    void setSyncBpm (double syncBpm);
    
    float getBpm() {return Bpm;}
    float getOBpm() {return OBpm;}
    float getBGS() {return beatGridStart;}
    
    void setMaster (bool isMaster);
    
    bool getMaster();
    
    void audioAnalysis();
    
    void setSectionPaint(Graphics& g, const Rectangle<int>& sectionBounds);
    
    //void mouseUp(const juce::MouseEvent &event) override;
    
    
    /** @return the value of the gain slider for each file player for use with the overall crossfade slider*/
    float crossfadeFileGainValue();
private:
    FilePlayer& filePlayer;
    //EQ& eq;
    AudioThumbnailCache audioCache;
    ColouredAudioThumbnail audioWaveform;
    std::unique_ptr<BeatGrid> beatgrid;
    std::vector<BeatMarkerData> markers;
    Atomic<bool> loop;
    
    TextButton playButton;
    TextButton startLoopButton, endLoopButton;
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
    float timeInSeconds, totalTimeInSeconds, pValue;
    float zoomNo = 8.0;
    float nextDownBeatTime;
    double secondsPerBeat, Bpm, BpmRatio, OBpm;
    float totalMeasures, waveformRatio = 1.0;
    float beatGridStart;
    double SyncBpm;
    int syncCounter;
    bool Master, Right, newMaster;
    
};


#endif  // H_FILEPLAYERGUI
