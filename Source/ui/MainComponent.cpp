/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent (Audio& audio_) : audio (audio_),
filePlayerGui(audio.getFilePlayer(0), audio.getFilePlayer(0).getEQ()),
filePlayerGui1(audio.getFilePlayer(1), audio.getFilePlayer(1).getEQ()),
sourcePlayer(new AudioSourcePlayer),
readerSource(nullptr),
formatManager(new AudioFormatManager),
audioDeviceManager(new AudioDeviceManager)
{

   
    formatManager->registerBasicFormats();
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
    
    fileChooser = new FileBrowserComponent(FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles | FileBrowserComponent::FileChooserFlags::canSelectDirectories | FileBrowserComponent::FileChooserFlags::useTreeView, File("/Users/harrygardiner/Desktop"), &wavFiles, nullptr);
    
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
    
    filePlayerGui.setBounds (0, 0, getWidth()/2, getHeight()-20);
    filePlayerGui1.setBounds (getWidth()/2, 0, getWidth()/2, getHeight()-20);
    masterGain.setBounds(0, getHeight()-20, getWidth(), 20);
    filePan.setBounds(0, getHeight()-40, getWidth(), 20);
    fileChooser->setBounds(0, 350, getWidth()/3, 200);
    loadA.setBounds((getWidth()/2) - 20, 420, 50, 50);
    loadB.setBounds((getWidth()/2) + 20, 420, 50, 50);
    
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
        filePlayerGui.loadFile(prepAudioFile);
        filePlayerGui.audioAnalysis(analyser->getBPM(), analyser->getKey(), analyser->getBGS());
    }
    else if (button == &loadB)
    {
        filePlayerGui1.loadFile(prepAudioFile);
        filePlayerGui1.audioAnalysis(analyser->getBPM(), analyser->getKey(), analyser->getBGS());
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
        if (audioFile.getFileExtension() == ".wav" || audioFile.getFileExtension() == ".m4a" || audioFile.getFileExtension() == ".mp3")
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

