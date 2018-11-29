/*
  ==============================================================================

    FilePlayer.cpp
    Created: 22 Jan 2013 2:49:14pm
    Author:  tj3-mitchell

  ==============================================================================
*/

#include "FilePlayer.h"
#include <memory>

FilePlayer::FilePlayer() : thread("FilePlayThread")
                           
                           

{
    thread.startThread();
    currentAudioFileSource = NULL;
    formatManager.registerBasicFormats();
}

/**
 Destructor
 */
FilePlayer::~FilePlayer()
{
    audioTransportSource.setSource(nullptr);//unload the current file
    deleteAndZero(currentAudioFileSource);//delete the current file
    thread.stopThread(100);
}

/**
 Starts or stops playback of the looper
 */
void FilePlayer::setPlaying (const bool newState)
{
    if(newState == true)
    {
        audioTransportSource.start();
    }
    else
    {
        audioTransportSource.stop();
    }
}

/**
 Gets the current playback state of the looper
 */
bool FilePlayer::isPlaying () const
{
    return audioTransportSource.isPlaying();
}


/**
 Loads the specified file into the transport source
 */
void FilePlayer::loadFile(const File& newFile)
{
    // unload the previous file source and delete it..
    setPlaying(false);
    audioTransportSource.setSource (0);
    deleteAndZero (currentAudioFileSource);
    
    // create a new file source from the file..
    // get a format manager and set it up with the basic types (wav, ogg and aiff).
    
    
    
    
    
    AudioFormatReader* reader = formatManager.createReaderFor (newFile);
    
    if (reader != 0)
    {
        //currentFile = audioFile;
        currentAudioFileSource = new AudioFormatReaderSource (reader, true);
        
        // ..and plug it into our transport source
        audioTransportSource.setSource (currentAudioFileSource,
                                   currentAudioFileSource->getTotalLength(),     //buffer this many samples ahead
                                   &thread,
                                   reader->sampleRate);
        
        

        
        waveformFile = newFile;
        
        
        

    }
}

void FilePlayer::setPosition(float newPosition)
{
    audioTransportSource.setPosition(newPosition * audioTransportSource.getLengthInSeconds());
}

float FilePlayer::getPosition()
{
    float position;
    position = audioTransportSource.getCurrentPosition()/audioTransportSource.getLengthInSeconds();
    return position;
}

double FilePlayer::getLengthInSeconds()
{
    return audioTransportSource.getLengthInSeconds();
}

void FilePlayer::setGain(float sliderValue)
{
    audioTransportSource.setGain(sliderValue);
}

float FilePlayer::getGain()
{
    float gain;
    gain = audioTransportSource.getGain();
    return gain;
}



float FilePlayer::getNextSample()
{
    return currentSample;
}

//void FilePlayer::setLooping(bool newState, float startPos, float secondsPerBeat)
//{
//    ScopedLock sl(loopLock);
//
//    while (newState == true)
//    {
//        float endPos = startPos + (4 * secondsPerBeat);
//
//        float currentPos = audioTransportSource.getCurrentPosition() * audioTransportSource.getLengthInSeconds();
//
//        if (currentPos == endPos || currentPos > endPos)
//        {
//            audioTransportSource.setPosition(startPos / audioTransportSource.getLengthInSeconds());
//        }
//    }
//}





//AudioSource
void FilePlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    sRate = sampleRate;
    samplesPerFrame = samplesPerBlockExpected;
    audioTransportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void FilePlayer::releaseResources()
{
    audioTransportSource.releaseResources();
}

void FilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    

    audioTransportSource.getNextAudioBlock (bufferToFill);
    
    
    
    for (int counter = bufferToFill.startSample; counter < bufferToFill.numSamples; counter++)
    {
        float bass[2], mid[2], high[2];
        
//        timeStretching.setBufferDetails(sRate, samplesPerFrame, counter, bufferToFill.numSamples);
//        
//        timeStretching.run();

        for (int numChans = 0; numChans < bufferToFill.buffer->getNumChannels(); numChans++)
        {
            float sample = bufferToFill.buffer->getSample(numChans, counter);



            sampleLevel = sample;

            currentSample = counter;

            bass[numChans] = eq.filterSamples(sample, kBass);
            mid[numChans] = eq.filterSamples(sample, kMid);
            high[numChans] = eq.filterSamples(sample, kHigh);


            bufferToFill.buffer->setSample(numChans, counter, ((sample +(bass[numChans] * eq.getFreqGain(0)) + (mid[numChans] * eq.getFreqGain(1)) + (high[numChans] * eq.getFreqGain(2)))));

        }
    }
    
    
    
}
