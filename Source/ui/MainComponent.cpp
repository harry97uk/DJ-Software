/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent (Audio& audio_) : audio (audio_),
filePlayerGui (audio, false),
filePlayerGui1 (audio, true),
sourcePlayer(new AudioSourcePlayer),
readerSource(nullptr),
formatManager(new AudioFormatManager),
audioDeviceManager(new AudioDeviceManager)
{

    ValueTree& audioState = audio.getState();
    formatManager->registerBasicFormats();
    wavFiles.reset(new WildcardFileFilter(formatManager->getWildcardForAllFormats(), "", "Audio file filter"));
    audioDeviceManager->initialiseWithDefaultDevices(2, 2);
    audioDeviceManager->addAudioCallback(sourcePlayer.get());
    
    addAndMakeVisible(filePlayerGui);
    addAndMakeVisible(filePlayerGui1);
    
    fileControlMinus.addListener(this);
    addAndMakeVisible(&fileControlMinus);
    fileControlPlus.addListener(this);
    addAndMakeVisible(&fileControlPlus);
    
    filePan.addListener(this);
    filePan.setTextBoxStyle(filePan.NoTextBox, false, 0, 0);
    addAndMakeVisible(&filePan);
    filePan.setRange(-1, 1);
    filePan.setValue(0);
    
    masterGain.addListener(this);
    masterGain.setValue(1);
    masterGain.setSliderStyle(Slider::Rotary);
    masterGain.setTextBoxStyle(masterGain.NoTextBox, false, 0, 0);
    addAndMakeVisible(&masterGain);
    masterGain.setRange(0, 1);
    
    loadA.addListener(this);
    loadA.setButtonText("A");
    addAndMakeVisible(&loadA);
    
    loadB.addListener(this);
    loadB.setButtonText("B");
    addAndMakeVisible(&loadB);
    
    masterA.addListener(this);
    masterA.setButtonText("Master A");
    addAndMakeVisible(&masterA);
    
    masterB.addListener(this);
    masterB.setButtonText("Master B");
    addAndMakeVisible(&masterB);
    
    record.addListener(this);
    record.setButtonText("Record");
    addAndMakeVisible(&record);
    
    fileChooser = new FileBrowserComponent(FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles | FileBrowserComponent::FileChooserFlags::canSelectDirectories | FileBrowserComponent::FileChooserFlags::useTreeView, File("/Users/harrygardiner/Desktop/Temp dj folder"), wavFiles.get(), nullptr);
    
    
    fileChooser->addListener(this);
    addAndMakeVisible(fileChooser);
    
    filePan.getValueObject().referTo(audioState.getPropertyAsValue(AudioState::crossfader, nullptr));
    masterGain.getValueObject().referTo(audioState.getPropertyAsValue(AudioState::masterVolume, nullptr));
    loadA.getToggleStateValue().referTo(audioState.getPropertyAsValue(AudioState::loadA, nullptr));
    loadB.getToggleStateValue().referTo(audioState.getPropertyAsValue(AudioState::loadB, nullptr));
    masterGain.getValueObject().referTo(audioState.getPropertyAsValue(AudioState::masterVolume, nullptr));
fileControlPlus.getToggleStateValue().referTo(audioState.getPropertyAsValue(AudioState::selectionControlPlus, nullptr));
fileControlMinus.getToggleStateValue().referTo(audioState.getPropertyAsValue(AudioState::selectionControlMinus, nullptr));
    
    setSize(1250, 650);
    
    
    
}

MainComponent::~MainComponent()
{
    audioDeviceManager->removeAudioCallback(sourcePlayer.get());
    sourcePlayer->setSource(nullptr);
    sourcePlayer.reset();
    readerSource.reset();
}

void MainComponent::resized()
{
    
    filePlayerGui.setBounds (0, 0, getWidth() - getWidth()/3, getHeight()-20);
    filePlayerGui1.setBounds (getWidth()/3, 0, getWidth() -  getWidth()/3, getHeight()-20);
    masterGain.setBounds((getWidth()/2) - 120, 0, 60, 60);
    filePan.setBounds((getWidth()/2) - 120, getHeight()-40, 240, 20);
    fileChooser->setBounds(0, 390, getWidth(), 200);
    record.setBounds((getWidth()/2) + 120, 0, 60, 60);
    
    //Load & Master Buttons
    Rectangle<int> loadMasterBounds (getWidth()/3, 300, getWidth()/3, 50);
    
    loadA.setBounds(loadMasterBounds.getX()
                    , loadMasterBounds.getY()
                    , loadMasterBounds.getWidth()/4
                    , loadMasterBounds.getHeight());
    
    loadB.setBounds(loadMasterBounds.getX() + (loadMasterBounds.getWidth() * 3)/4
                    , loadMasterBounds.getY()
                    , loadMasterBounds.getWidth()/4
                    , loadMasterBounds.getHeight());
    
    masterA.setBounds(loadMasterBounds.getX() + (loadMasterBounds.getWidth())/4
                      , loadMasterBounds.getY()
                      , loadMasterBounds.getWidth()/4
                      , loadMasterBounds.getHeight());
    
    masterB.setBounds(loadMasterBounds.getX() + (loadMasterBounds.getWidth() * 2)/4
                      , loadMasterBounds.getY()
                      , loadMasterBounds.getWidth()/4
                      , loadMasterBounds.getHeight());
    
}

void MainComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &masterGain)
    {
        audio.masterGain(masterGain.getValue());
    }
    else if (slider == &filePan)
    {
        audio.setChannelGain(filePlayerGui.crossfadeFileGainValue(), 0);
        audio.setChannelGain(filePlayerGui1.crossfadeFileGainValue(), 1);
        audio.crossfadeGain(filePan.getValue(), filePlayerGui.crossfadeFileGainValue(), filePlayerGui1.crossfadeFileGainValue());
    }
    
    
}

void MainComponent::buttonClicked(Button* button)
{
    
    
}

void MainComponent::buttonStateChanged(juce::Button *button)
{
     if (button->isDown() == true || button->getToggleState() == true)
     {
    if (button == &loadA)
    {
        if (filePlayerGui.getMaster() == true)
        {
            filePlayerGui.setBpmRatio(filePlayerGui1.getBpm()/filePlayerGui.getBpm());
            syncBpm = filePlayerGui1.getBpm();
            filePlayerGui.setSyncBpm(syncBpm);
            filePlayerGui1.setMaster(true);
            filePlayerGui.setMaster(false);
            masterB.setButtonText("Master B On");
            masterA.setButtonText("Master A");
            filePlayerGui.loadFile(prepAudioFile);
            filePlayerGui.setBPM(analyser->getBPM());
            filePlayerGui.setOriginalBpm(analyser->getBPM());
            filePlayerGui.setKey(analyser->getKey());
            filePlayerGui.setBGS(analyser->getBGS());
            filePlayerGui.audioAnalysis();
        }
        else if (filePlayerGui.getMaster() == false)
        {
            filePlayerGui.loadFile(prepAudioFile);
            filePlayerGui.setBPM(analyser->getBPM());
            filePlayerGui.setOriginalBpm(analyser->getBPM());
            filePlayerGui.setKey(analyser->getKey());
            filePlayerGui.setBGS(analyser->getBGS());
            filePlayerGui.audioAnalysis();
        }
        
        
    }
    else if (button == &loadB)
    {
        if (filePlayerGui1.getMaster() == true)
        {
            filePlayerGui1.setBpmRatio(filePlayerGui.getBpm()/filePlayerGui1.getBpm());
            syncBpm = filePlayerGui.getBpm();
            filePlayerGui1.setSyncBpm(syncBpm);
            filePlayerGui.setMaster(true);
            filePlayerGui1.setMaster(false);
            masterA.setButtonText("Master A On");
            masterB.setButtonText("Master B");
            filePlayerGui1.loadFile(prepAudioFile);
            filePlayerGui1.setBPM(analyser->getBPM());
            filePlayerGui1.setOriginalBpm(analyser->getBPM());
            filePlayerGui1.setKey(analyser->getKey());
            filePlayerGui1.setBGS(analyser->getBGS());
            filePlayerGui1.audioAnalysis();
        }
        else if (filePlayerGui1.getMaster() == false)
        {
            filePlayerGui1.loadFile(prepAudioFile);
            filePlayerGui1.setBPM(analyser->getBPM());
            filePlayerGui1.setOriginalBpm(analyser->getBPM());
            filePlayerGui1.setKey(analyser->getKey());
            filePlayerGui1.setBGS(analyser->getBGS());
            filePlayerGui1.audioAnalysis();
        }
        
        
    }
    else if (button == &masterA)
    {
        filePlayerGui1.setBpmRatio(filePlayerGui.getBpm()/filePlayerGui1.getBpm());
        syncBpm = filePlayerGui.getBpm();
        filePlayerGui1.setSyncBpm(syncBpm);
        filePlayerGui.setMaster(true);
        filePlayerGui1.setMaster(false);
        masterA.setButtonText("Master A On");
        masterB.setButtonText("Master B");
        masterA.setColour(masterA.buttonColourId, juce::Colours::red);
        masterB.setColour(masterB.buttonColourId, juce::Colours::darkgrey);
        
    }
    else if (button == &masterB)
    {
        filePlayerGui.setBpmRatio(filePlayerGui1.getBpm()/filePlayerGui.getBpm());
        syncBpm = filePlayerGui1.getBpm();
        filePlayerGui.setSyncBpm(syncBpm);
        filePlayerGui1.setMaster(true);
        filePlayerGui.setMaster(false);
        masterB.setButtonText("Master B On");
        masterA.setButtonText("Master A");
        masterB.setColour(masterB.buttonColourId, juce::Colours::red);
        masterA.setColour(masterA.buttonColourId, juce::Colours::darkgrey);
        
    }
    else if (button == &record)
    {
        audio.setSave(!save);
        save = !save;
        if (save == true)
        {
            record.setColour(record.buttonColourId, Colours::red);
        }
    }
//    else if (button == &fileControlPlus)
//    {
//        fileDoubleClicked(fileChooser->getSelectedFile(fileNum));
//        fileNum = fileNum + 1;
//        fileControlPlus.setToggleState(false, dontSendNotification);
//    }
//    else if (button == &fileControlMinus)
//    {
//        fileDoubleClicked(fileChooser->getSelectedFile(fileNum));
//        fileNum = fileNum - 1;
//        fileControlMinus.setToggleState(false, dontSendNotification);
//    }
     }
}

void MainComponent::selectionChanged()
{
    
}

void MainComponent::fileClicked(const File& file, const MouseEvent& e)
{
    sourcePlayer->setSource(nullptr);
    
    File audioFile (fileChooser->getSelectedFile(1));
    
    if(audioFile.existsAsFile())
    {
        if (wavFiles->isFileSuitable(audioFile))
        {
            
            prepAudioFile = audioFile;
            readerSource.reset(new AudioFormatReaderSource(formatManager->createReaderFor(audioFile), true));
            sourcePlayer->setSource(readerSource.get());
            
            analyser.reset(new AudioAnalyser(formatManager->createReaderFor(audioFile)));
            analyser->launchThread(7);
            
        }
        else
        {
            AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                         "sdaTransport",
                                         "Couldn't open file!\n\n");
        }
        
    }
    else
    {
        AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                     "sdaTransport",
                                     "Couldn't open file!\n\n");
    }
}

void MainComponent::fileDoubleClicked(const File& file)
{
    File audioFile (fileChooser->getSelectedFile(1));
    
    if(audioFile.existsAsFile())
    {
        prepAudioFile = audioFile;
    }
    else
    {
        AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                     "sdaTransport",
                                     "Couldn't open file!\n\n");
    }
}

void MainComponent::browserRootChanged(const File& newroot)
{
    
}

void MainComponent::paint(juce::Graphics &g)
{
   
}

void MainComponent::lookAndFeelChanged()
{
    
}




//MenuBarCallbacks==============================================================
StringArray MainComponent::getMenuBarNames()
{
    const char* const names[] = { "File", 0 };
    return StringArray (names);
}

PopupMenu MainComponent::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
    PopupMenu menu;
    if (topLevelMenuIndex == 0)
        menu.addItem(AudioPrefs, "Audio Preferences", true, false);
    return menu;
}

void MainComponent::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
    if (topLevelMenuIndex == FileMenu)
    {
        if (menuItemID == AudioPrefs)
        {
            AudioDeviceSelectorComponent audioSettingsComp (audio.getAudioDeviceManager(),
                                                            0, 2, 2, 2, true, true, true, false);
            audioSettingsComp.setSize (450, 350);
            DialogWindow::showModalDialog ("Audio Settings",
                                           &audioSettingsComp, this, Colours::lightgrey, true);
        }
    }
}

