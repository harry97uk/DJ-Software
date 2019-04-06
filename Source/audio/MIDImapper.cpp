//
//  MIDImapper.cpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 12/03/2019.
//

#include "MIDImapper.hpp"

MidiMapper::MidiMapper()
{
    state.reset(new AudioState);
    state->setDefaults();
}

MidiMapper::~MidiMapper()
{
    
}

void MidiMapper::handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message)
{
    String messageDescription = message.getDescription();
    //bool isController = message.isController();
    ValueTree& audioState = state->getState();
    
    DBG(messageDescription);
    if (message.isController())
    {
        int controllerNum = message.getControllerNumber();
        DBG(controllerNum);
        int value = message.getControllerValue();
        int channel = message.getChannel();
        float overallVal = 0.0;
        if (channel == 2)
        {
            switch (controllerNum)
            {
                case 1:
                    audioState.setProperty(state->highChannel0, (value * (1/63.5)) - 1, nullptr);
                    break;
                case 2:
                    audioState.setProperty(state->midChannel0, (value * (1/63.5)) - 1, nullptr);
                    break;
                case 3:
                    audioState.setProperty(state->bassChannel0, (value * (1/63.5)) - 1, nullptr);
                    break;
                case 4:
                    audioState.setProperty(state->filterChannel0, (value * (1/63.5)) - 1, nullptr);
                    break;
                case 5:
                    overallVal = value/127.0;
                    audioState.setProperty(state->fileGainChannel0, overallVal, nullptr);
                    break;
                
                default:
                    break;
            }
        }
        else if (channel == 3)
        {
            switch (controllerNum)
            {
                case 1:
                    audioState.setProperty(state->highChannel1, (value * (1/63.5)) - 1, nullptr);
                    break;
                case 2:
                    audioState.setProperty(state->midChannel1, (value * (1/63.5)) - 1, nullptr);
                    break;
                case 3:
                    audioState.setProperty(state->bassChannel1, (value * (1/63.5)) - 1, nullptr);
                    break;
                case 4:
                    audioState.setProperty(state->filterChannel1, (value * (1/63.5)) - 1, nullptr);
                    break;
                case 5:
                    audioState.setProperty(state->fileGainChannel1, value * (1/127.0), nullptr);
                    break;
                    
                default:
                    break;
            }
        }
        else
        {
            if (controllerNum == 10)
            {
                audioState.setProperty(state->masterVolume, value * (1/127.0), nullptr);
            }
            else if (controllerNum == 7)
                audioState.setProperty(state->crossfader, (value * (1/63.5)) - 1, nullptr);
            else if (controllerNum == 23)
            {
                if (message.getControllerValue() == 1)
                {
                    audioState.setProperty(state->selectionControlPlus, true, nullptr);
                }
                else if (message.getControllerValue() == 127)
                    audioState.setProperty(state->selectionControlMinus, true, nullptr);
                
            }
            
        }

        
        
    }
    else if (message.isNoteOnOrOff())
    {
        int noteNum = message.getNoteNumber();
        int channel = message.getChannel();
        int value = message.getVelocity();
        
        
        if (channel == 2)
        {
            
                switch (noteNum)
                {
                    case 1:
                        audioState.setProperty(state->playButtonChan0, message.isNoteOn(), nullptr);
                        break;
                    case 2:
                        audioState.setProperty(state->syncButtonChan0, message.isNoteOn(), nullptr);
                        break;
                    case 19:
                        audioState.setProperty(state->inLoopChan0, message.isNoteOn(), nullptr);
                        break;
                    case 20:
                        audioState.setProperty(state->outLoopChan0, message.isNoteOn(), nullptr);
                        break;
                    case 21:
                        audioState.setProperty(state->eightbarChan0, message.isNoteOn(), nullptr);
                        break;
                    case 22:
                        audioState.setProperty(state->halfLoopChan0, message.isNoteOn(), nullptr);
                        break;
 
                    default:
                        break;
                }
            
            
        }
        else if (channel == 3)
        {
            
                switch (noteNum)
                {
                    case 1:
                        audioState.setProperty(state->playButtonChan1, message.isNoteOn(), nullptr);
                        break;
                    case 2:
                        audioState.setProperty(state->syncButtonChan1, message.isNoteOn(), nullptr);
                        break;
                    case 19:
                        audioState.setProperty(state->inLoopChan1, message.isNoteOn(), nullptr);
                        break;
                    case 20:
                        audioState.setProperty(state->outLoopChan1, message.isNoteOn(), nullptr);
                        break;
                    case 21:
                        audioState.setProperty(state->eightbarChan1, message.isNoteOn(), nullptr);
                        break;
                    case 22:
                        audioState.setProperty(state->halfLoopChan1, message.isNoteOn(), nullptr);
                        break;
                        
                    default:
                        break;
                }
            
            
        }
        else if (channel == 1)
        {
            if (noteNum == 16)
            {
                audioState.setProperty(state->loadA, value/127.0, nullptr);
            }
            else if (noteNum == 17)
                audioState.setProperty(state->loadB, value/127.0, nullptr);
            
            
        }
        DBG(noteNum);
        DBG(channel);
        
    }
}



