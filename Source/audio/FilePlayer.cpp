/*
  ==============================================================================

    FilePlayer.cpp
    Created: 22 Jan 2013 2:49:14pm
    Author:  tj3-mitchell

  ==============================================================================
*/

#include "FilePlayer.h"
#include <memory>

FilePlayer::FilePlayer() :  thread ("FilePlayThread"), resampler(&audioSource, false, 2)
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
    gain = sliderValue;
}

float FilePlayer::getGain()
{
    return gain;
}

void FilePlayer::setEqFreqGain (float gain, int eqIndex)
{
    eq[0].setFreqGain (gain, eqIndex);
    eq[1].setFreqGain (gain, eqIndex);
}

int64 FilePlayer::getTotalSamples()
{
    return totalSamples;
}

void FilePlayer::setLoopStart(float start)
{
    loopSample = start;
}

void FilePlayer::setBpmRatio(double bpmRatio)
{
    BpmRatio = bpmRatio;
    
}

void FilePlayer::setFilterValue(float filterVal)
{
    filterValue = filterVal;
}

void FilePlayer::setDelayValue(float delayVal)
{
    delayValue = delayVal;
    
}

void FilePlayer::setReverbValue(float reverbVal)
{
    reverbValue = reverbVal;
}

void FilePlayer::setLooping(bool newState, float secondsPerBeat, int bars)
{
    numBars = bars;
    float loopLength = (bars * secondsPerBeat) * getSampleRate();
    float endPos = loopSample + loopLength;
    endBounds = endPos/getSampleRate();
    
    audioSource.setLoop(newState, endPos, loopSample);
    
}





//AudioSource
void FilePlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    samplesPerFrame = samplesPerBlockExpected;
    sRate = sampleRate;
    resampler.prepareToPlay(samplesPerBlockExpected, sampleRate);
    delay[0].initialize(sampleRate);
    delay[1].initialize(sampleRate);
    for (int counter = 0; counter < 16; counter++)
    {
        reverb[0][counter].initialize(sampleRate);
        reverb[1][counter].initialize(sampleRate);
    }
    
}

void FilePlayer::releaseResources()
{
    resampler.releaseResources();
}

void FilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
//    audioSource.SetPitch(BpmRatio);
    resampler.setResamplingRatio(BpmRatio);
    resampler.getNextAudioBlock (bufferToFill);
    bufferToFill.buffer->applyGainRamp(bufferToFill.startSample, bufferToFill.numSamples, lastGain, gain);
    
    for (int counter = bufferToFill.startSample; counter < bufferToFill.numSamples; counter++)
    {
        for (int chan = 0; chan < bufferToFill.buffer->getNumChannels(); chan++)
        {
            float sample = bufferToFill.buffer->getSample (chan, counter);

            sample += eq[chan].filterSamples (sample, kBass, 0, 0, 0) * eq[chan].getFreqGain(0);
            sample += eq[chan].filterSamples (sample, kMid, 0, 0, 0)  * eq[chan].getFreqGain(1);
            sample += eq[chan].filterSamples (sample, kHigh, 0, 0, 0) * eq[chan].getFreqGain(2);
            
            sample = eq[chan].filterSamples (sample, kGlobalFilter, lastFilterValue, filterValue, bufferToFill.numSamples);
            
            sample += delay[chan].delayRamp(lastDelayValue
                                            , delayValue
                                            , bufferToFill.numSamples);
            delay[chan].write (sample);
            
            for (int counter = 0; counter < 16; counter++)
            {
                reverb[chan][counter].write (sample);
            }
            for (int counter = 0; counter < 16; counter++)
            {
                sample += eq[chan].filterSamples (reverb[chan][counter].read(reverbValue/1.5) * 0.3, kReverbFilter, 0, 0, 0);
            }

            
            
            bufferToFill.buffer->setSample (chan, counter, sample);
            
            
        }
    }
    lastDelayValue = delayValue;
    lastFilterValue = filterValue;
    lastGain = gain;
    
}
