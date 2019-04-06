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
    midiMapper.getState().addListener(this);

    filePlayerChannel0.reset(new FilePlayer(midiMapper.getState()));
    filePlayerChannel1.reset(new FilePlayer(midiMapper.getState()));
    
    audioDeviceManager.addMidiInputCallback ("", this);
    audioDeviceManager.setMidiInputEnabled("MixTrack Pro 3", true);
    audioDeviceManager.addAudioCallback (this);
    
    mixerAudioSource.addInputSource(filePlayerChannel0.get(), false);
    mixerAudioSource.addInputSource(filePlayerChannel1.get(), false);
    
    audioSampleBuffer.setSize(2, 1);
    audioSampleBuffer.clear();
    
    
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
//    ValueTree& audioState = state->getState();
//
//    int channelNum = message.getChannel();
//
//    //DBG(messageDescription);
//    if (isController == true)
//    {
//        int controllerNum = message.getControllerNumber();
//        DBG(controllerNum);
//        int value = message.getControllerValue();
//        if (controllerNum == 3)
//        {
//            audioState.setProperty(state->bassChannel0.toString(), (value * (1/63.5)) - 1, nullptr);
//        }
//
//    }
//    else if (isController == false)
//    {
//        int noteNum = message.getNoteNumber();
//
//    }
    midiMapper.handleIncomingMidiMessage(source, message);
    
    

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
    int currentSaveSample = 0;
    while(numSamples--)
    {
        
        
        
        inSampL = *outL;
        inSampR = *outR;
        
        if (record == true)
        {
            outL = audioSampleBuffer.getWritePointer(0, currentSaveSample);
            outR = audioSampleBuffer.getWritePointer(1, currentSaveSample);
            currentSaveSample++;
            audioSampleBuffer.setSize(2, audioSampleBuffer.getNumSamples() + 1);
        }
        
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
        filePlayerChannel1->setGain((1 - fabsf(sliderValue)) * FileGain1);
    }
    else if (sliderValue > 0)
    {
        filePlayerChannel0->setGain((1 - fabsf(sliderValue)) * FileGain);
    }
    
}

FilePlayer& Audio::getFilePlayer(int fileNum)
{
    if (fileNum == 0)
        return *filePlayerChannel0.get();
        
    else
        return *filePlayerChannel1.get();
}

void Audio::setChannelGain(float sliderValue, int chanNum)
{
    if (chanNum == 0)
    {
        gainValChan0 = sliderValue;
    }
    if (chanNum == 1)
    {
        gainValChan1 = sliderValue;
    }
}

void Audio::saveAudio()
{
    
    FileChooser chooser ("Please select a file...",
                         File::getSpecialLocation (File::userDesktopDirectory), "*.wav");
    if (chooser.browseForFileToSave(true))
    {
        File file (chooser.getResult().withFileExtension (".wav"));
        OutputStream* outStream = file.createOutputStream();
        WavAudioFormat wavFormat;
        AudioFormatWriter* writer = wavFormat.
        createWriterFor (outStream, 44100, 1, 16, NULL, 0);
        writer->writeFromAudioSampleBuffer
        (audioSampleBuffer, 0, audioSampleBuffer.getNumSamples());
        delete writer;
    }
}

void Audio::setSave(bool save)
{
    if (save == true)
    {
        record = save;
    }
    else if (save == false)
    {
        record = save;
        saveAudio();
    }
    
}

void Audio::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
    if (tree.getType() == AudioState::Parameters)
    {
//        if (property == AudioState::crossfader)
//        {
//            crossfadeGain(tree[property], gainValChan0, gainValChan1);
//        }
        if (property == AudioState::masterVolume)
        {
            masterGain(tree[property]);
        }
        if (property == AudioState::fileGainChannel0)
        {
            float cross = tree[AudioState::crossfader];
            float value = tree[property];
            if (cross > 0)
            {
                filePlayerChannel0->setGain(1 - fabs(cross) * value);
            }
            else
                filePlayerChannel0->setGain(value);
            
            DBG (tree[property].toString());
        }
        if (property == AudioState::fileGainChannel1)
        {
            float cross = tree[AudioState::crossfader];
            float value = tree[property];
            if (cross < 0)
            {
                filePlayerChannel1->setGain(1 - fabs(cross) * value);
            }
            else
                filePlayerChannel1->setGain(tree[property]);
            
            DBG (tree[property].toString());
        }
        if (property == AudioState::bassChannel0)
        {
            filePlayerChannel0->setEqFreqGain(tree[property], 0);
            DBG (tree[property].toString());
        }
        if (property == AudioState::bassChannel1)
        {
            filePlayerChannel1->setEqFreqGain(tree[property], 0);
            DBG (tree[property].toString());
        }
        if (property == AudioState::midChannel0)
        {
            filePlayerChannel0->setEqFreqGain(tree[property], 1);
            DBG (tree[property].toString());
        }
        if (property == AudioState::midChannel1)
        {
            filePlayerChannel1->setEqFreqGain(tree[property], 1);
            DBG (tree[property].toString());
        }
        if (property == AudioState::highChannel0)
        {
            filePlayerChannel0->setEqFreqGain(tree[property], 2);
            DBG (tree[property].toString());
        }
        if (property == AudioState::highChannel1)
        {
            filePlayerChannel1->setEqFreqGain(tree[property], 2);
            DBG (tree[property].toString());
        }
        if (property == AudioState::filterChannel0)
        {
            filePlayerChannel0->setFilterValue(tree[property]);
            DBG (tree[property].toString());
        }
        if (property == AudioState::filterChannel1)
        {
            filePlayerChannel1->setFilterValue(tree[property]);
            DBG (tree[property].toString());
        }
        
        
    }
    
    
}

