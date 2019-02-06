/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent (Audio& audio_) : audio (audio_),
filePlayerGui(audio.getFilePlayer(0), audio.getFilePlayer(0).getEQ(), false),
filePlayerGui1(audio.getFilePlayer(1), audio.getFilePlayer(1).getEQ(), true),
sourcePlayer(new AudioSourcePlayer),
readerSource(nullptr),
formatManager(new AudioFormatManager),
audioDeviceManager(new AudioDeviceManager)
{

   
    formatManager->registerBasicFormats();
    wavFiles.reset(new WildcardFileFilter(formatManager->getWildcardForAllFormats(), "", "Audio file filter"));
    audioDeviceManager->initialiseWithDefaultDevices(2, 2);
    audioDeviceManager->addAudioCallback(sourcePlayer.get());
    
    addAndMakeVisible(filePlayerGui);
    addAndMakeVisible(filePlayerGui1);
    
    filePan.addListener(this);
    addAndMakeVisible(&filePan);
    filePan.setRange(-1, 1);
    filePan.setValue(0);
    
    masterGain.addListener(this);
    masterGain.setValue(1);
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
    
    fileChooser = new FileBrowserComponent(FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles | FileBrowserComponent::FileChooserFlags::canSelectDirectories | FileBrowserComponent::FileChooserFlags::useTreeView, File("/Users/harrygardiner/Music"), wavFiles.get(), nullptr);
    
    fileChooser->addListener(this);
    addAndMakeVisible(fileChooser);
    
    
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
    masterGain.setBounds(0, getHeight()-20, getWidth(), 20);
    filePan.setBounds(0, getHeight()-40, getWidth(), 20);
    fileChooser->setBounds(0, 350, getWidth()/3, 200);
    loadA.setBounds((getWidth()/2) - 20, 420, 50, 50);
    loadB.setBounds((getWidth()/2) + 20, 420, 50, 50);
    masterA.setBounds((getWidth()/2) - 60, 420, 50, 50);
    masterB.setBounds((getWidth()/2) + 60, 420, 50, 50);
    
}

void MainComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &masterGain)
    {
        audio.masterGain(masterGain.getValue());
    }
    else if (slider == &filePan)
    {
        audio.crossfadeGain(filePan.getValue(), filePlayerGui.crossfadeFileGainValue(), filePlayerGui1.crossfadeFileGainValue());
    }
    
    
}

void MainComponent::buttonClicked(Button* button)
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
    g.setColour(Colours::red);
    g.drawLine(getWidth()/2, 65, getWidth()/2, 265);
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
        menu.addItem(AudioPrefs, "Audio Prefrences", true, false);
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

