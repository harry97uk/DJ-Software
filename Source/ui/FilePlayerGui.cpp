/*
  ==============================================================================

    FilePlayerGui.cpp
    Created: 22 Jan 2013 2:49:07pm
    Author:  tj3-mitchell

  ==============================================================================
*/

#include "FilePlayerGui.h"

FilePlayerGui::FilePlayerGui (FilePlayer& filePlayer_, EQ& eq_, bool right)
 :  filePlayer (filePlayer_),
    eq (eq_),
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
    
    zoomPlusButton.setButtonText("Zoom +");
    zoomPlusButton.addListener(this);
    addAndMakeVisible(&zoomPlusButton);
    
    zoomMinusButton.setButtonText("Zoom -");
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
    fileGain.addListener(this);
    addAndMakeVisible(&fileGain);
    
    LFreq.setSliderStyle(juce::Slider::Rotary);
    LFreq.setRange(-1, 1);
    LFreq.setValue(0);
    LFreq.addListener(this);
    addAndMakeVisible(&LFreq);
    
    MFreq.setSliderStyle(juce::Slider::Rotary);
    MFreq.setRange(-1, 1);
    MFreq.setValue(0);
    MFreq.addListener(this);
    addAndMakeVisible(&MFreq);
    
    HFreq.setSliderStyle(juce::Slider::Rotary);
    HFreq.setRange(-1, 1);
    HFreq.setValue(0);
    HFreq.addListener(this);
    addAndMakeVisible(&HFreq);
    
    bpmSlider.setSliderStyle(juce::Slider::LinearVertical);
    bpmSlider.setRange(getBpm() - 20, getBpm() + 20);
    bpmSlider.setValue(getBpm());
    bpmSlider.addListener(this);
    addAndMakeVisible(&bpmSlider);
    
    filterSlider.setSliderStyle(juce::Slider::Rotary);
    filterSlider.setRange(-1, 1);
    filterSlider.setValue(0);
    filterSlider.addListener(this);
    addAndMakeVisible(&filterSlider);
    
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
        playButton.setBounds ((getWidth()/8)*3, getHeight()-80, 60, 40);
        startLoopButton.setBounds((getWidth()/8), getHeight()-80, 60, 40);
        endLoopButton.setBounds((getWidth()/8)*1.5, getHeight()-80, 60, 40);
        zoomMinusButton.setBounds((getWidth()/8)*2, getHeight()-80, 60, 40);
        zoomPlusButton.setBounds((getWidth()/8)*2.5, getHeight()-80, 60, 40);
        syncButton.setBounds((getWidth()/4)*0.5, getHeight()-80, 60, 40);
        
        playbackPosition.setBounds(0, 20, getWidth()/2, 20);
        fileGain.setBounds(0, 40, getWidth()/2, 20);
        gain.setBounds(getWidth()/4, 30, getWidth()/4, 20);
        playback.setBounds(getWidth()/4, 10, getWidth()/4, 20);
        
        bass.setBounds(getWidth()/8, 170, getWidth()/2, 40);
        mid.setBounds(getWidth()/8, 110, getWidth()/2, 40);
        high.setBounds(getWidth()/8, 50, getWidth()/2, 40);
        LFreq.setBounds(0, 180, getWidth()/2, 60);
        MFreq.setBounds(0, 120, getWidth()/2, 60);
        HFreq.setBounds(0, 60, getWidth()/2, 60);
        
        bpmSlider.setBounds(0, 250, getWidth()/3, 100);
        filterSlider.setBounds(getWidth()/4, 250, getWidth()/3, 100);
        BPM.setBounds(getWidth()/2 * 0.75, 50, getWidth()/4, 40);
        Key.setBounds(getWidth()/2 * 0.75, 170, getWidth()/4, 40);
        time.setBounds(getWidth()/2 * 0.75, 110, getWidth()/4, 40);
        totalTime.setBounds(getWidth()/2 * 0.85, 110, getWidth()/4, 40);
    }
    else if (Right == true)
    {
        playButton.setBounds (((getWidth()/8)*3) + getWidth()/2, getHeight()-80, 60, 40);
        startLoopButton.setBounds((getWidth()/8) + getWidth()/2, getHeight()-80, 60, 40);
        endLoopButton.setBounds(((getWidth()/8)*1.5) + getWidth()/2, getHeight()-80, 60, 40);
        zoomMinusButton.setBounds(((getWidth()/8)*2) + getWidth()/2, getHeight()-80, 60, 40);
        zoomPlusButton.setBounds(((getWidth()/8)*2.5) + getWidth()/2, getHeight()-80, 60, 40);
        syncButton.setBounds(((getWidth()/8)*0.5) + getWidth()/2, getHeight()-80, 60, 40);
        
        playbackPosition.setBounds(0 + getWidth()/2, 20, getWidth()/2, 20);
        fileGain.setBounds(0 + getWidth()/2, 40, getWidth()/2, 20);
        gain.setBounds(getWidth()/4 + getWidth()/2, 30, getWidth()/4, 20);
        playback.setBounds(getWidth()/4 + getWidth()/2, 10, getWidth()/4, 20);
        
        bass.setBounds(getWidth()/8 + getWidth()/2, 170, getWidth()/2, 40);
        mid.setBounds(getWidth()/8 + getWidth()/2, 110, getWidth()/2, 40);
        high.setBounds(getWidth()/8 + getWidth()/2, 50, getWidth()/2, 40);
        LFreq.setBounds(0 + getWidth()/2, 180, getWidth()/2, 60);
        MFreq.setBounds(0 + getWidth()/2, 120, getWidth()/2, 60);
        HFreq.setBounds(0 + getWidth()/2, 60, getWidth()/2, 60);
        
        bpmSlider.setBounds(0 + getWidth()/2, 250, getWidth()/3, 100);
        filterSlider.setBounds(getWidth()/4 + getWidth()/2, 250, getWidth()/3, 100);
        BPM.setBounds((getWidth()/2 * 0.75) + getWidth()/2, 50, getWidth()/4, 40);
        Key.setBounds((getWidth()/2 * 0.75)+ getWidth()/2, 170 , getWidth()/4, 40);
        time.setBounds((getWidth()/2 * 0.75)+ getWidth()/2, 110, getWidth()/4, 40);
        totalTime.setBounds((getWidth()/2 * 0.85) + getWidth()/2, 110, getWidth()/4, 40);
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
    g.setColour (Colours::darkgrey);
    g.fillRect (thumbnailBounds);
    g.setColour (Colours::white);
    g.drawFittedText ("No File Loaded", thumbnailBounds, Justification::centred, 1.0f);
}

void FilePlayerGui::paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds)
{
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

void FilePlayerGui::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &filePlayer.getATS())  transportSourceChanged();
    if (source == &audioWaveform)        thumbnailChanged();
}

void FilePlayerGui::transportSourceChanged()
{
    if (filePlayer.getATS().isPlaying())
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
        eq.setFreqGain(LFreq.getValue(), 0);
    }
    else if (slider == &MFreq)
    {
        eq.setFreqGain(MFreq.getValue(), 1);
    }
    else if (slider == &HFreq)
    {
        eq.setFreqGain(HFreq.getValue(), 2);
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


