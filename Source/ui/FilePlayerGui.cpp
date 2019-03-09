/*
  ==============================================================================

    FilePlayerGui.cpp
    Created: 22 Jan 2013 2:49:07pm
    Author:  tj3-mitchell

  ==============================================================================
*/

#include "FilePlayerGui.h"

FilePlayerGui::FilePlayerGui (FilePlayer& filePlayer_, bool right)
 :  filePlayer (filePlayer_),
    audioCache(5),
    audioWaveform (512, filePlayer.getFormatManager(), audioCache)

{
    if (right == true)
    {
        Right = true;
    }
     if (right == false)
        Right = false;
    
    playButton.setButtonText ("PLAY");
    playButton.addListener(this);
    addAndMakeVisible(&playButton);
    
    zoomPlusButton.setButtonText("Zoom+");
    zoomPlusButton.addListener(this);
    addAndMakeVisible(&zoomPlusButton);
    
    zoomMinusButton.setButtonText("Zoom-");
    zoomMinusButton.addListener(this);
    addAndMakeVisible(&zoomMinusButton);
    
    BPM.setText("BPM = -", dontSendNotification);
    addAndMakeVisible(&BPM);
    
    Key.setText("Key = -", dontSendNotification);
    addAndMakeVisible(&Key);
    
    time.setText("00:00", dontSendNotification);
    addAndMakeVisible(&time);
    
    totalTime.setText("00:00", dontSendNotification);
    addAndMakeVisible(&totalTime);
    
    gain.setText("Gain", dontSendNotification);
    addAndMakeVisible(&gain);
    
    bass.setText("Bass", dontSendNotification);
    addAndMakeVisible(&bass);
    
    mid.setText("Mid", dontSendNotification);
    addAndMakeVisible(&mid);
    
    high.setText("High", dontSendNotification);
    addAndMakeVisible(&high);
    
    playback.setText("Playback Position", dontSendNotification);
    addAndMakeVisible(&playback);
    
    startLoopButton.setButtonText("START LOOP");
    startLoopButton.addListener(this);
    addAndMakeVisible(&startLoopButton);
    
    endLoopButton.setButtonText("END LOOP");
    endLoopButton.addListener(this);
    addAndMakeVisible(&endLoopButton);
    
    syncButton.setButtonText("SYNC");
    syncButton.addListener(this);
    addAndMakeVisible(&syncButton);
    
    playbackPosition.setRange(0, 1);
    playbackPosition.getNumDecimalPlacesToDisplay();
    playbackPosition.addListener(this);
    addAndMakeVisible(&playbackPosition);
    
    fileGain.setRange(0, 1);
    fileGain.setValue(1);
    fileGain.setTextBoxStyle(fileGain.NoTextBox, false, 0, 0);
    fileGain.addListener(this);
    addAndMakeVisible(&fileGain);
    
    LFreq.setSliderStyle(juce::Slider::Rotary);
    LFreq.setRange(-1, 1);
    LFreq.setValue(0);
    LFreq.setTextBoxStyle(LFreq.NoTextBox, false, 0, 0);
    LFreq.addListener(this);
    addAndMakeVisible(&LFreq);
    
    MFreq.setSliderStyle(juce::Slider::Rotary);
    MFreq.setRange(-1, 1);
    MFreq.setValue(0);
    MFreq.setTextBoxStyle(MFreq.NoTextBox, false, 0, 0);
    MFreq.addListener(this);
    addAndMakeVisible(&MFreq);
    
    HFreq.setSliderStyle(juce::Slider::Rotary);
    HFreq.setRange(-1, 1);
    HFreq.setValue(0);
    HFreq.setTextBoxStyle(HFreq.NoTextBox, false, 0, 0);
    HFreq.addListener(this);
    addAndMakeVisible(&HFreq);
    
    bpmSlider.setSliderStyle(juce::Slider::LinearVertical);
    bpmSlider.setRange(-1, 1);
    bpmSlider.setValue(0);
    bpmSlider.setTextBoxStyle(bpmSlider.NoTextBox, false, 0, 0);
    bpmSlider.addListener(this);
    addAndMakeVisible(&bpmSlider);
    
    filterSlider.setSliderStyle(juce::Slider::Rotary);
    filterSlider.setRange(-0.98, 1);
    filterSlider.setValue(0);
    filterSlider.setTextBoxStyle(filterSlider.NoTextBox, false, 0, 0);
    filterSlider.addListener(this);
    addAndMakeVisible(&filterSlider);
    
    delaySlider.setSliderStyle(juce::Slider::Rotary);
    delaySlider.setRange(0, 0.8);
    delaySlider.setValue(0);
    delaySlider.setTextBoxStyle(delaySlider.NoTextBox, false, 0, 0);
    delaySlider.addListener(this);
    addAndMakeVisible(&delaySlider);
    
    reverbSlider.setSliderStyle(juce::Slider::Rotary);
    reverbSlider.setRange(0, 0.1);
    reverbSlider.setValue(0);
    reverbSlider.setTextBoxStyle(reverbSlider.NoTextBox, false, 0, 0);
    reverbSlider.addListener(this);
    addAndMakeVisible(&reverbSlider);
    
    audioWaveform.addChangeListener(this);
    
    
}

FilePlayerGui::~FilePlayerGui()
{
    
}

//Component
void FilePlayerGui::resized()
{
    if (Right == false)
    {
        //Play, Loop, Sync and Zoom Buttons
        Rectangle<int> buttonControlBounds (50, getHeight() - 80, getWidth()/3, 40);
        
        
        playButton.setBounds (buttonControlBounds.getX()
                              , buttonControlBounds.getY()
                              , buttonControlBounds.getWidth()/5
                              , buttonControlBounds.getHeight());
        
        startLoopButton.setBounds(buttonControlBounds.getX() + buttonControlBounds.getWidth()/5
                                  , buttonControlBounds.getY()
                                  , buttonControlBounds.getWidth()/5
                                  , buttonControlBounds.getHeight());
        
        endLoopButton.setBounds(buttonControlBounds.getX() + (buttonControlBounds.getWidth() * 2)/5
                                , buttonControlBounds.getY()
                                , buttonControlBounds.getWidth()/5
                                , buttonControlBounds.getHeight());
        
        zoomMinusButton.setBounds(buttonControlBounds.getX() + (buttonControlBounds.getWidth() * 3)/5
                                  , buttonControlBounds.getY()
                                  , buttonControlBounds.getWidth()/5
                                  , buttonControlBounds.getHeight());
        
        zoomPlusButton.setBounds(buttonControlBounds.getX() + (buttonControlBounds.getWidth() * 4)/5
                                 , buttonControlBounds.getY()
                                 , buttonControlBounds.getWidth()/5
                                 , buttonControlBounds.getHeight());
        
        syncButton.setBounds(buttonControlBounds.getX() + (buttonControlBounds.getWidth() * 5)/5
                             , buttonControlBounds.getY()
                             , buttonControlBounds.getWidth()/5
                             , buttonControlBounds.getHeight());
        
        
        //File Position and Gain Control
        Rectangle<int> fileControlBounds (0, 0, getWidth()/2, 100);
        
        playbackPosition.setBounds(fileControlBounds.getX()
                                   , fileControlBounds.getY() + 5
                                   , fileControlBounds.getWidth()
                                   , fileControlBounds.getHeight()/3);
        
        fileGain.setBounds(fileControlBounds.getX()
                           , fileControlBounds.getY() + 25
                           , fileControlBounds.getWidth()
                           , fileControlBounds.getHeight()/3);
        
        gain.setBounds(fileControlBounds.getWidth()/2
                       , fileControlBounds.getY() + 20
                       , fileControlBounds.getWidth()/2
                       , fileControlBounds.getHeight()/5);
        
        playback.setBounds(fileControlBounds.getWidth()/2
                           , fileControlBounds.getY()
                           , fileControlBounds.getWidth()/2
                           , fileControlBounds.getHeight()/5);
        
        
        //EQ control
        Rectangle<int> eqBounds (getWidth()/6, 50, getWidth()/4, 150);
        
        bass.setBounds(eqBounds.getX() + eqBounds.getWidth()/5
                       , eqBounds.getY() + (eqBounds.getHeight() * 2)/3
                       , getWidth()/2
                       , eqBounds.getHeight()/4);
        
        mid.setBounds(eqBounds.getX() + eqBounds.getWidth()/5
                      , eqBounds.getY() + eqBounds.getHeight()/3
                      , getWidth()/2
                      , eqBounds.getHeight()/4);
        
        high.setBounds(eqBounds.getX() + eqBounds.getWidth()/5
                       , eqBounds.getY()
                       , getWidth()/2
                       , eqBounds.getHeight()/4);
        
        LFreq.setBounds(eqBounds.getX()
                        , eqBounds.getY() + (eqBounds.getHeight() * 2)/3
                        , eqBounds.getWidth()
                        , eqBounds.getHeight()/3);
        
        MFreq.setBounds(eqBounds.getX()
                        , eqBounds.getY() + eqBounds.getHeight()/3
                        , eqBounds.getWidth()
                        , eqBounds.getHeight()/3);
        
        HFreq.setBounds(eqBounds.getX()
                        , eqBounds.getY()
                        , eqBounds.getWidth()
                        , eqBounds.getHeight()/3);
        
        
        //BPM, Key and Time Labels
        Rectangle<int> informationLabelBounds (getWidth()/3 + 40, 50, getWidth()/8, 150);
        
        BPM.setBounds(informationLabelBounds.getX()
                      , informationLabelBounds.getY()
                      , informationLabelBounds.getWidth()
                      , informationLabelBounds.getHeight()/4);
        
        Key.setBounds(informationLabelBounds.getX()
                      , informationLabelBounds.getY() + informationLabelBounds.getHeight()/3
                      , informationLabelBounds.getWidth()
                      , informationLabelBounds.getHeight()/4);
        
        time.setBounds(informationLabelBounds.getX()
                       , informationLabelBounds.getY() + (informationLabelBounds.getHeight() * 2)/3
                       , informationLabelBounds.getWidth()
                       , informationLabelBounds.getHeight()/4);
        
        totalTime.setBounds(informationLabelBounds.getX() + 40
                            , informationLabelBounds.getY() + (informationLabelBounds.getHeight() * 2)/3
                            , informationLabelBounds.getWidth()
                            , informationLabelBounds.getHeight()/4);
        
        
        bpmSlider.setBounds(0, 50, getWidth()/8, 150);
        
        //Effect Sliders
        Rectangle<int> effectsBounds (0, 250, getWidth()/2, 70);
        
        filterSlider.setBounds(effectsBounds.getX()
                               , effectsBounds.getY()
                               , effectsBounds.getWidth()/3
                               , effectsBounds.getHeight());
        
        delaySlider.setBounds(effectsBounds.getX() + effectsBounds.getWidth()/3
                               , effectsBounds.getY()
                               , effectsBounds.getWidth()/3
                               , effectsBounds.getHeight());
        
        reverbSlider.setBounds(effectsBounds.getX() + (effectsBounds.getWidth() * 2)/3
                              , effectsBounds.getY()
                              , effectsBounds.getWidth()/3
                              , effectsBounds.getHeight());
        
    }
    else if (Right == true)
    {
        //Play, Loop, Sync and Zoom Buttons
        Rectangle<int> buttonControlBounds (50 + getWidth()/2, getHeight() - 80, getWidth()/3, 40);
        
        playButton.setBounds (buttonControlBounds.getX()
                              , buttonControlBounds.getY()
                              , buttonControlBounds.getWidth()/5
                              , buttonControlBounds.getHeight());
        
        startLoopButton.setBounds(buttonControlBounds.getX() + buttonControlBounds.getWidth()/5
                                  , buttonControlBounds.getY()
                                  , buttonControlBounds.getWidth()/5
                                  , buttonControlBounds.getHeight());
        
        endLoopButton.setBounds(buttonControlBounds.getX() + (buttonControlBounds.getWidth() * 2)/5
                                , buttonControlBounds.getY()
                                , buttonControlBounds.getWidth()/5
                                , buttonControlBounds.getHeight());
        
        zoomMinusButton.setBounds(buttonControlBounds.getX() + (buttonControlBounds.getWidth() * 3)/5
                                  , buttonControlBounds.getY()
                                  , buttonControlBounds.getWidth()/5
                                  , buttonControlBounds.getHeight());
        
        zoomPlusButton.setBounds(buttonControlBounds.getX() + (buttonControlBounds.getWidth() * 4)/5
                                 , buttonControlBounds.getY()
                                 , buttonControlBounds.getWidth()/5
                                 , buttonControlBounds.getHeight());
        
        syncButton.setBounds(buttonControlBounds.getX() + (buttonControlBounds.getWidth() * 5)/5
                             , buttonControlBounds.getY()
                             , buttonControlBounds.getWidth()/5
                             , buttonControlBounds.getHeight());
        
        
        //File Position and Gain Control
        Rectangle<int> fileControlBounds (0 + getWidth()/2, 0, getWidth()/2, 100);
        
        playbackPosition.setBounds(fileControlBounds.getX()
                                   , fileControlBounds.getY() + 5
                                   , fileControlBounds.getWidth()
                                   , fileControlBounds.getHeight()/3);
        
        fileGain.setBounds(fileControlBounds.getX()
                           , fileControlBounds.getY() + 25
                           , fileControlBounds.getWidth()
                           , fileControlBounds.getHeight()/3);
        
        gain.setBounds(fileControlBounds.getX() + fileControlBounds.getWidth()/2
                       , fileControlBounds.getY() + 20
                       , fileControlBounds.getWidth()/2
                       , fileControlBounds.getHeight()/5);
        
        playback.setBounds(fileControlBounds.getX() + fileControlBounds.getWidth()/2
                           , fileControlBounds.getY()
                           , fileControlBounds.getWidth()/2
                           , fileControlBounds.getHeight()/5);
        
        //EQ Control
        Rectangle<int> eqBounds (getWidth()/6 + getWidth()/2, 50, getWidth()/4, 150);
        
        bass.setBounds(eqBounds.getX() + eqBounds.getWidth()/5
                       , eqBounds.getY() + (eqBounds.getHeight() * 2)/3
                       , getWidth()/2
                       , eqBounds.getHeight()/4);
        
        mid.setBounds(eqBounds.getX() + eqBounds.getWidth()/5
                      , eqBounds.getY() + eqBounds.getHeight()/3
                      , getWidth()/2
                      , eqBounds.getHeight()/4);
        
        high.setBounds(eqBounds.getX() + eqBounds.getWidth()/5
                       , eqBounds.getY()
                       , getWidth()/2
                       , eqBounds.getHeight()/4);
        
        LFreq.setBounds(eqBounds.getX()
                        , eqBounds.getY() + (eqBounds.getHeight() * 2)/3
                        , eqBounds.getWidth()
                        , eqBounds.getHeight()/3);
        
        MFreq.setBounds(eqBounds.getX()
                        , eqBounds.getY() + eqBounds.getHeight()/3
                        , eqBounds.getWidth()
                        , eqBounds.getHeight()/3);
        
        HFreq.setBounds(eqBounds.getX()
                        , eqBounds.getY()
                        , eqBounds.getWidth()
                        , eqBounds.getHeight()/3);
        
        
        //BPM, Key and Time Labels
        Rectangle<int> informationLabelBounds ((getWidth()/3 + 40) + getWidth()/2, 50, getWidth()/8, 150);
        
        BPM.setBounds(informationLabelBounds.getX()
                      , informationLabelBounds.getY()
                      , informationLabelBounds.getWidth()
                      , informationLabelBounds.getHeight()/4);
        
        Key.setBounds(informationLabelBounds.getX()
                      , informationLabelBounds.getY() + informationLabelBounds.getHeight()/3
                      , informationLabelBounds.getWidth()
                      , informationLabelBounds.getHeight()/4);
        
        time.setBounds(informationLabelBounds.getX()
                       , informationLabelBounds.getY() + (informationLabelBounds.getHeight() * 2)/3
                       , informationLabelBounds.getWidth()
                       , informationLabelBounds.getHeight()/4);
        
        totalTime.setBounds(informationLabelBounds.getX() + 40
                            , informationLabelBounds.getY() + (informationLabelBounds.getHeight() * 2)/3
                            , informationLabelBounds.getWidth()
                            , informationLabelBounds.getHeight()/4);
        
        bpmSlider.setBounds(0 + getWidth()/2, 50, getWidth()/8, 150);
        
        //Effect Sliders
        Rectangle<int> effectsBounds (0 + getWidth()/2, 250, getWidth()/2, 70);
        
        filterSlider.setBounds(effectsBounds.getX()
                               , effectsBounds.getY()
                               , effectsBounds.getWidth()/3
                               , effectsBounds.getHeight());
        
        delaySlider.setBounds(effectsBounds.getX() + effectsBounds.getWidth()/3
                              , effectsBounds.getY()
                              , effectsBounds.getWidth()/3
                              , effectsBounds.getHeight());
        
        reverbSlider.setBounds(effectsBounds.getX() + (effectsBounds.getWidth() * 2)/3
                               , effectsBounds.getY()
                               , effectsBounds.getWidth()/3
                               , effectsBounds.getHeight());
    }
    
    
}

//Button Listener
void FilePlayerGui::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        
        filePlayer.setPlaying(!filePlayer.isPlaying());
        timerCallback();
        if (filePlayer.isPlaying())
            playButton.setButtonText ("PAUSE");
        else
            playButton.setButtonText ("PLAY");
        
    }
    
    if (button == &zoomPlusButton)
    {
        zoomNo = zoomNo / 2;
    }
    
    if (button == &zoomMinusButton)
    {
        zoomNo = zoomNo * 2;
    }
    
    if (button == &syncButton)
    {
        if (Master == false)
        {
            setBPM(SyncBpm);
            filePlayer.setBpmRatio(BpmRatio);
            audioAnalysis();
            waveformRatio = BpmRatio;
 
        }
        else if (Master == true)
        {
            
        }
       
    }
//    if (button == &startLoopButton)
//    {
//
//        loop = true;
//        filePlayer.setLooping(loop.get(), filePlayer.getLengthInSeconds() * filePlayer.getPosition(), secondsPerBeat);
//
//    }
//    if (button == &endLoopButton)
//    {
//        loop = false;
//        filePlayer.setLooping(loop.get(), 0, 0);
//    }
}

//FilenameComponentListener
void FilePlayerGui::loadFile(File audioFile)
{
    

    //File audioFile (fileChooser->getSelectedFile(1));
        
        if(audioFile.existsAsFile())
        {
           
                
                filePlayer.loadFile(audioFile);
                audioWaveform.setSource (new FileInputSource (audioFile));
            
                playButton.setButtonText("PLAY");
                waveformRatio = 1;
                totalMeasures = (filePlayer.getLengthInSeconds()/60) * OBpm;
                
            
            
        }
        else
        {
            AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                         "sdaTransport",
                                         "Couldn't open file!\n\n");
        }
    
}

void FilePlayerGui::setBPM(double bpm)
{
    Bpm = bpm;
}

void FilePlayerGui::setOriginalBpm(double originalBpm)
{
    OBpm = originalBpm;
}

void FilePlayerGui::setKey(juce::String key)
{
    key1 = key;
}

void FilePlayerGui::setBGS(float BGS)
{
    beatGridStart = BGS/1000;
}

void FilePlayerGui::setBpmRatio(double bpmRatio)
{
    BpmRatio = bpmRatio;
    
}

void FilePlayerGui::setSyncBpm(double syncBpm)
{
    SyncBpm = syncBpm;
}

void FilePlayerGui::setMaster(bool isMaster)
{
    Master = isMaster;
}

bool FilePlayerGui::getMaster()
{
    return Master;
}

void FilePlayerGui::paint(Graphics& g)
{
    if (Right == false)
    {
        Rectangle<int> thumbnailBounds (getWidth()/2, 65, getWidth()/2, 100);
        Rectangle<int> thumbnail2Bounds (0, 210, getWidth()/2, 30);
        
        if (audioWaveform.getNumChannels() == 0)
        {
            paintIfNoFileLoaded (g, thumbnailBounds);
            paintIfNoFileLoaded(g, thumbnail2Bounds);
        }
        else
        {
            paintIfFileLoaded (g, thumbnailBounds);
            paintIfFileLoaded (g, thumbnail2Bounds);
        }
    }
    if (Right == true)
    {
        Rectangle<int> thumbnailBounds (0, 165, getWidth()/2, 100);
        Rectangle<int> thumbnail2Bounds (0 + getWidth()/2, 210, getWidth()/2, 30);
        
        if (audioWaveform.getNumChannels() == 0)
        {
            paintIfNoFileLoaded (g, thumbnailBounds);
            paintIfNoFileLoaded(g, thumbnail2Bounds);
        }
        else
        {
            paintIfFileLoaded (g, thumbnailBounds);
            paintIfFileLoaded (g, thumbnail2Bounds);
        }
    }
    
    
    
}

void FilePlayerGui::paintIfNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds)
{
    g.setColour (Colours::black);
    g.fillRect (thumbnailBounds);
    g.setColour(Colours::lightslategrey);
    
    g.drawHorizontalLine(thumbnailBounds.getY()
                         , thumbnailBounds.getX()
                         , thumbnailBounds.getX() + thumbnailBounds.getWidth());
    
    g.drawHorizontalLine(thumbnailBounds.getY() + thumbnailBounds.getHeight()
                         , thumbnailBounds.getX()
                         , thumbnailBounds.getX() + thumbnailBounds.getWidth());
    
    g.drawVerticalLine(thumbnailBounds.getX()
                       , thumbnailBounds.getY()
                       , thumbnailBounds.getY() + thumbnailBounds.getHeight());
    
    g.drawVerticalLine(thumbnailBounds.getX() + thumbnailBounds.getWidth()
                       , thumbnailBounds.getY()
                       , thumbnailBounds.getY() + thumbnailBounds.getHeight());
    
    g.setColour (Colours::white);
    g.drawFittedText ("No File Loaded", thumbnailBounds, Justification::centred, 1.0f);
}

void FilePlayerGui::paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds)
{
    g.setColour(Colours::lightslategrey);
    
    g.drawHorizontalLine(thumbnailBounds.getY()
                         , thumbnailBounds.getX()
                         , thumbnailBounds.getX() + thumbnailBounds.getWidth());
    
    g.drawHorizontalLine(thumbnailBounds.getY() + thumbnailBounds.getHeight()
                         , thumbnailBounds.getX()
                         , thumbnailBounds.getX() + thumbnailBounds.getWidth());
    
    g.drawVerticalLine(thumbnailBounds.getX()
                       , thumbnailBounds.getY()
                       , thumbnailBounds.getY() + thumbnailBounds.getHeight());
    
    g.drawVerticalLine(thumbnailBounds.getX() + thumbnailBounds.getWidth()
                       , thumbnailBounds.getY()
                       , thumbnailBounds.getY() + thumbnailBounds.getHeight());
    
    g.setColour (Colours::black);
    g.fillRect (thumbnailBounds);
    
    beatgrid.reset(new BeatGrid(getBpm(), filePlayer.getSampleRate(), getBGS()*(filePlayer.getSampleRate())));
    markers = beatgrid->FindBeats(0, totalTimeInSeconds * (filePlayer.getSampleRate()));
    for (int counter = 0; counter < markers.size(); counter++)
    {
        if (markers[counter].downBeat == true)
        {
            syncCounter = markers[counter].seconds;
            if (thumbnailBounds.getY() == 65 && markers[counter].seconds > timeInSeconds)
            {
                g.setColour(Colours::yellow);
                g.drawLine((((markers[counter].seconds/zoomNo) - (timeInSeconds/zoomNo)) * thumbnailBounds.getWidth()) + getWidth()/2, thumbnailBounds.getY(), (((markers[counter].seconds/zoomNo) - (timeInSeconds/zoomNo)) * thumbnailBounds.getWidth()) + getWidth()/2, thumbnailBounds.getY() + 100);
                nextDownBeatTime = markers[counter].seconds;
            }
            else if (thumbnailBounds.getY() == 165 && markers[counter].seconds < timeInSeconds + (zoomNo))
            {
                g.setColour(Colours::yellow);
                g.drawLine((((markers[counter].seconds/(zoomNo)) - (timeInSeconds/(zoomNo))) * thumbnailBounds.getWidth()), thumbnailBounds.getY(), (((markers[counter].seconds/(zoomNo)) - (timeInSeconds/(zoomNo))) * thumbnailBounds.getWidth()), thumbnailBounds.getY() + 100);
            }
            
            
        }
        else if (markers[counter].downBeat == false)
        {
            if (thumbnailBounds.getY() == 65 && markers[counter].seconds > timeInSeconds)
            {
                g.setColour(Colours::darkgrey);
                g.drawLine((((markers[counter].seconds/zoomNo) - (timeInSeconds/zoomNo)) * thumbnailBounds.getWidth()) + getWidth()/2, thumbnailBounds.getY(), (((markers[counter].seconds/zoomNo) - (timeInSeconds/zoomNo)) * thumbnailBounds.getWidth()) + getWidth()/2, thumbnailBounds.getY() + 100);
            }
            else if (thumbnailBounds.getY() == 165 && markers[counter].seconds < timeInSeconds + (zoomNo))
            {
                g.setColour(Colours::darkgrey);
                g.drawLine((((markers[counter].seconds/(zoomNo)) - (timeInSeconds/(zoomNo))) * thumbnailBounds.getWidth()), thumbnailBounds.getY(), (((markers[counter].seconds/(zoomNo)) - (timeInSeconds/(zoomNo))) * thumbnailBounds.getWidth()), thumbnailBounds.getY() + 100);
            }
        }

        
    }
    

    
    g.setColour (Colours::blue);
    if (thumbnailBounds.getHeight() == 30)
    {
        audioWaveform.drawColouredChannel(g,
                                          thumbnailBounds,
                                          0,
                                          totalTimeInSeconds,
                                          1,
                                          audioWaveform.getApproximatePeak() * 0.8f);
    }
    else
    {
        
            audioWaveform.drawColouredChannel(g,
                                              thumbnailBounds,
                                              timeInSeconds,                                    // start time
                                              (timeInSeconds) + (zoomNo),
                                              1,                                        // end time
                                              audioWaveform.getApproximatePeak() * 0.8f);
        
        
    
    }
    
}

void FilePlayerGui::setSectionPaint(juce::Graphics &g, const Rectangle<int> &sectionBounds)
{
    
}

void FilePlayerGui::changeListenerCallback(ChangeBroadcaster* source)
{
    
    if (source == &audioWaveform)        thumbnailChanged();
}

void FilePlayerGui::transportSourceChanged()
{
    if (filePlayer.getATS().IsPlaying())
        filePlayer.setPlaying(true);
    else
        filePlayer.setPlaying(false);
}

void FilePlayerGui::thumbnailChanged()
{
    repaint();
}



void FilePlayerGui::audioAnalysis()
{
    BPM.setText("BPM = " + String(Bpm), dontSendNotification);
    Key.setText("Key = " + String(key1), dontSendNotification);
}


/*void FilePlayerGui::mouseUp(const juce::MouseEvent &event)
{
    if (&event == &playbackControl)
    {
        if (playbackControl.getScreenY() < 340 && playbackControl.getScreenY() > 240)
        {
            playbackPosition.setValue(playbackControl.getScreenX());
        }
    }
}*/

void FilePlayerGui::sliderValueChanged(Slider* slider)
{
    if (slider == &playbackPosition)
    {
        filePlayer.setPosition(playbackPosition.getValue());
    }
    else if (slider == &fileGain)
    {
        filePlayer.setGain(fileGain.getValue());
    }
    else if (slider == &LFreq)
    {
        filePlayer.setEqFreqGain (LFreq.getValue(), 0);
    }
    else if (slider == &MFreq)
    {
        filePlayer.setEqFreqGain (MFreq.getValue(), 1);
    }
    else if (slider == &HFreq)
    {
        filePlayer.setEqFreqGain (HFreq.getValue(), 2);
    }
    else if (slider == &filterSlider)
    {
        filePlayer.setFilterValue(filterSlider.getValue());
    }
    else if (slider == &delaySlider)
    {
        filePlayer.setDelayValue(delaySlider.getValue());
    }
    else if (slider == &reverbSlider)
    {
        filePlayer.setReverbValue(reverbSlider.getValue());
    }
    else if (slider == &bpmSlider)
    {
        filePlayer.setBpmRatio(1 + (bpmSlider.getValue()/4));
    }

}

float FilePlayerGui::crossfadeFileGainValue()
{
    return fileGain.getValue();
}

void FilePlayerGui::timerCallback()
{
    if (filePlayer.isPlaying() == true)
    {
        startTimer(10);
        
        timeInSeconds = filePlayer.getLengthInSeconds() * filePlayer.getPosition();
        
        totalTimeInSeconds = filePlayer.getLengthInSeconds();
        int time1 = (int)timeInSeconds;
        int time2 = (int)totalTimeInSeconds;
        
        int seconds = time1 % 60;
        int seconds1 = time2 % 60;
        int minutes = (time1 / 60) % 60;
        int minutes1 = (time2 / 60) % 60;
        if (seconds < 10)
        {
            time.setText("0" + String(minutes) + ":" + "0" + String(seconds), dontSendNotification);
        }
        else
        {
        time.setText("0" + String(minutes) + ":" + String(seconds), dontSendNotification);
        }
        if (seconds1 < 10)
        {
            totalTime.setText("0" + String(minutes1) + ":" + "0" + String(seconds1), dontSendNotification);
        }
        else
        {
            totalTime.setText("0" + String(minutes1) + ":" + String(seconds1), dontSendNotification);
        }
        
        playbackPosition.setValue(filePlayer.getPosition(), dontSendNotification);
        thumbnailChanged();
        
       
    
    }
    else if (filePlayer.isPlaying() == false)
    {
        stopTimer();
    }
    
}


