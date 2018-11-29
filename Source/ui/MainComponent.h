/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../audio/Audio.h"
#include "FilePlayerGui.h"
#include "/Users/harrygardiner/Desktop/SDA/DJ-Software-Application-master/Source/audio/AudioAnalyser.cpp"
#include <stdio.h>


//==============================================================================
/**
    Main Component contains two versions of a graphical user interface for each file player along with a master gain control and crossfade option
*/
class MainComponent   : public Component,
                        public MenuBarModel,
                        public Slider::Listener,
                        public FileBrowserListener,
                        public Button::Listener

{
public:
    //==============================================================================
    /** Constructor */
    MainComponent (Audio& audio_);

    /** Destructor */
    ~MainComponent();

    /** Used to arrange objects in the window*/
    void resized() override;

    /** Slider Listener - called when a slider is moved*/
    void sliderValueChanged(Slider* slider) override;
    
    void buttonClicked(Button* button) override;
    
    /** Function that does something when the selection is changed in the file browser
     @see FileBrowserListener*/
    void selectionChanged() override;
    
    /** Loads selected file if clicked once
     @see FileBrowserListener*/
    void fileClicked (const File& file, const MouseEvent& e) override;
    
    /** Loads selected file if clicked twice
     @see FileBrowserListener*/
    void fileDoubleClicked (const File& file) override;
    
    /** Called if the browser's root changes
     @see FileBrowserListener*/
    void browserRootChanged (const File& newRoot) override;
    
    //MenuBarEnums/Callbacks========================================================
    enum Menus
    {
        FileMenu=0,
        
        NumMenus
    };
    
    enum FileMenuItems
    {
        AudioPrefs = 1,
        
        NumFileItems
    };
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName) override;
    void menuItemSelected (int menuItemID, int topLevelMenuIndex) override;
    
private:
    Audio& audio;
    FilePlayerGui filePlayerGui;
    FilePlayerGui filePlayerGui1;
    Slider masterGain;
    Slider filePan;
    TextButton loadA, loadB;
    WildcardFileFilter wavFiles = WildcardFileFilter("*.wav;*.mp3", "", "audioFiles");
    ScopedPointer<FileBrowserComponent> fileChooser;
    File prepAudioFile;
    std::unique_ptr<AudioSourcePlayer>        sourcePlayer;
    std::unique_ptr<AudioFormatReaderSource>  readerSource;
    std::unique_ptr<AudioFormatManager>       formatManager;
    std::unique_ptr<AudioDeviceManager>       audioDeviceManager;
    std::unique_ptr<AudioAnalyser>            analyser;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
