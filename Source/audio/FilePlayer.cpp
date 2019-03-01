/*
  ==============================================================================

    FilePlayer.cpp
    Created: 22 Jan 2013 2:49:14pm
    Author:  tj3-mitchell

  ==============================================================================
*/

#include "FilePlayer.h"
#include <memory>

FilePlayer::FilePlayer() :  thread ("FilePlayThread"),
                            effects(getSampleRate())
{
    thread.startThread();
    currentAudioFileSource = NULL;
    formatManager.registerBasicFormats();
    BpmRatio = 1;
}

/**
 Destructor
 */
FilePlayer::~FilePlayer()
{
    //unload the current file
    thread.stopThread(100);
}

/**
 Starts or stops playback of the looper
 */
void FilePlayer::setPlaying (const bool newState)
{
    if(newState == true)
    {
        audioSource.SetPlaying(true);
    }
    else
    {
        audioSource.SetPlaying(false);
    }
}

/**
 Gets the current playback state of the looper
 */
bool FilePlayer::isPlaying () const
{
    return audioSource.IsPlaying();
}


/**
 Loads the specified file into the transport source
 */
void FilePlayer::loadFile(const File& newFile)
{
    // unload the previous file source and delete it..
    setPlaying(false);
    
    
    // create a new file source from the file..
    // get a format manager and set it up with the basic types (wav, ogg and aiff).
    
    AudioFormatReader* reader = formatManager.createReaderFor (newFile);
    
    if (reader != 0)
    {
        //currentFile = audioFile;

        audioSource.SetSource(reader, false);
       
        
        // ..and plug it into our transport source
  
       
        
        totalSamples = reader->lengthInSamples;
        
        waveformFile = newFile;
    }
}

void FilePlayer::setPosition(float newPosition)
{
    audioSource.SetPlayheadSample(newPosition * totalSamples);
}

float FilePlayer::getPosition()
{
    float position;
    position = audioSource.GetPlayhead()/totalSamples;
    return position;
}

double FilePlayer::getLengthInSeconds()
{
    return (totalSamples/BpmRatio) / getSampleRate();
    
}

void FilePlayer::setGain(float sliderValue)
{
    audioSource.SetGain(sliderValue);
}

float FilePlayer::getGain()
{
    float gain;
    gain = audioSource.GetGain();
    return gain;
}



float FilePlayer::getNextSample()
{
    return currentSample;
}

int64 FilePlayer::getTotalSamples()
{
    return totalSamples;
}

void FilePlayer::setBpmRatio(double bpmRatio)
{
    BpmRatio = bpmRatio;
    
}

void FilePlayer::setFilterValue(float filterVal)
{
    filterValue = filterVal;
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
    
    samplesPerFrame = samplesPerBlockExpected;
    sRate = sampleRate;
    
}

void FilePlayer::releaseResources()
{
    audioSource.releaseResources();
}

void FilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    audioSource.SetPitch(BpmRatio);
    audioSource.getNextAudioBlock (bufferToFill);
    
    
    
    for (int counter = bufferToFill.startSample; counter < bufferToFill.numSamples; counter++)
    {
        float bass[2], mid[2], high[2];



        for (int numChans = 0; numChans < bufferToFill.buffer->getNumChannels(); numChans++)
        {
            float sample = bufferToFill.buffer->getSample(numChans, counter);



            sampleLevel = sample;

            currentSample = counter;


            bass[numChans] = eq.filterSamples(sample, kBass);
            mid[numChans] = eq.filterSamples(sample, kMid);
            high[numChans] = eq.filterSamples(sample, kHigh);


            bufferToFill.buffer->setSample(numChans, counter, (((sample +(bass[numChans] * eq.getFreqGain(0)) + (mid[numChans] * eq.getFreqGain(1)) + (high[numChans] * eq.getFreqGain(2))))));






        }
    }
    
    
    
}
