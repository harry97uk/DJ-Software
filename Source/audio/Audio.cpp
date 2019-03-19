/*
  ==============================================================================

    Audio.cpp
    Created: 13 Nov 2014 8:14:40am
    Author:  Tom Mitchell

  ==============================================================================
*/

#include "Audio.h"

Audio::Audio()
{
    audioDeviceManager.initialiseWithDefaultDevices (2, 2);//2 inputs, 2 outputs

    //load the filePlayer into the audio source
    audioSourcePlayer.setSource(&mixerAudioSource);
    
    
    audioDeviceManager.addMidiInputCallback ("", this);
    audioDeviceManager.setMidiInputEnabled("Party Mix", true);
    audioDeviceManager.addAudioCallback (this);
    
    mixerAudioSource.addInputSource(&filePlayer[0], false);
    mixerAudioSource.addInputSource(&filePlayer[1], false);
    

    
    
    }

Audio::~Audio()
{
    audioDeviceManager.removeAudioCallback (this);
    audioDeviceManager.removeMidiInputCallback ("", this);
    
    mixerAudioSource.removeAllInputs();
}


void Audio::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    //All MIDI inputs arrive here
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
//            
//        }
//    }
    //midiMapper.handleIncomingMidiMessage(source, message);
    

}

void Audio::audioDeviceIOCallback (const float** inputChannelData,
                                           int numInputChannels,
                                           float** outputChannelData,
                                           int numOutputChannels,
                                           int numSamples)
{
    // get the audio from our file player - player puts samples in the output buffer
    audioSourcePlayer.audioDeviceIOCallback (inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples);
    
    
    

    
    
    //All audio processing is done here
    const float *inL = inputChannelData[0];
    const float *inR = inputChannelData[1];
    float *outL = outputChannelData[0];
    float *outR = outputChannelData[1];
    float inSampL;
    float inSampR;
    while(numSamples--)
    {
        
        
        
        inSampL = *outL;
        inSampR = *outR;
        

        
        *outL = inSampL * 1.f;
        *outR = inSampR * 1.f;
        
        

        
        inL++;
        inR++;
        outL++;
        outR++;

    }
    
}


void Audio::audioDeviceAboutToStart (AudioIODevice* device)
{
    audioSourcePlayer.audioDeviceAboutToStart (device);
    
    
    
}

void Audio::audioDeviceStopped()
{
    audioSourcePlayer.audioDeviceStopped();
    
}

void Audio::masterGain(float sliderValue)
{
    audioSourcePlayer.setGain(sliderValue);
    
}

void Audio::crossfadeGain(float sliderValue, float FileGain, float FileGain1)
{
    if (sliderValue < 0)
    {
        filePlayer[1].setGain((1 - fabsf(sliderValue)) * FileGain1);
    }
    else if (sliderValue > 0)
    {
        filePlayer[0].setGain((1 - fabsf(sliderValue)) * FileGain);
    }
    
}



