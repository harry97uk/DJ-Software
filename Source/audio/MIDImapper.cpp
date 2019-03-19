////
////  MIDImapper.cpp
////  JuceBasicAudio - App
////
////  Created by Harry Gardiner on 12/03/2019.
////
//
//#include "MIDImapper.hpp"
//
//MidiMapper::MidiMapper(FilePlayerGui& filePlayerGui_)
//:  filePlayerGui (filePlayerGui_)
//{
//    audioDeviceManager.initialiseWithDefaultDevices (2, 2);
//    audioDeviceManager.addMidiInputCallback ("", this);
//    audioDeviceManager.setMidiInputEnabled("Party Mix", true);
//    
//}
//
//MidiMapper::~MidiMapper()
//{
//    audioDeviceManager.removeMidiInputCallback ("", this);
//}
//
//void MidiMapper::handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message)
//{
//    String messageDescription = message.getDescription();
//    bool isController = message.isController();
//    int noteNum = message.getNoteNumber();
//    DBG(messageDescription);
//    if (isController == true)
//    {
//        
//    }
//    else if (isController == false)
//    {
//        if (noteNum == 0)
//        {
//            filePlayerGui.midiAction(0);
//        }
//    }
//}
//
//int MidiMapper::buttonType()
//{
//    return buttonNum;
//}

