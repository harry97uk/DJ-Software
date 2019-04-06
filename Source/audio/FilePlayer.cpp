

#include "FilePlayer.h"
#include <memory>
#include "AudioState.hpp"

FilePlayer::FilePlayer (ValueTree& state) :  thread ("FilePlayThread"), resampler(&audioSource, false, 2)
{
    thread.startThread();
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

        audioSource.SetSource(reader, true);
       
        
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
    position = audioSource.GetPlayhead()/(totalSamples/BpmRatio);
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

void FilePlayer::setLoopStart(int start)
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

void FilePlayer::setSecondsPerBeat(float secondsPerBeat)
{
    secondsPB = secondsPerBeat;
}


//AudioSource
void FilePlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    samplesPerFrame = samplesPerBlockExpected;
    sRate = sampleRate;
    resampler.prepareToPlay(samplesPerBlockExpected, sampleRate);
    delay[0].initialize(sampleRate);
    delay[1].initialize(sampleRate);
    
        reverb[0].initialize(sampleRate);
        reverb[1].initialize(sampleRate);
    
    
    
    
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

            //EQ
            sample += eq[chan].filterSamples (sample, kBass) * eq[chan].getFreqGain(0);
            sample += eq[chan].filterSamples (sample, kMid)  * eq[chan].getFreqGain(1);
            sample += eq[chan].filterSamples (sample, kHigh) * eq[chan].getFreqGain(2);
            
            //Filter
            if (lastFilterValue != filterValue)
            {
                lastFilterValue += (filterValue - lastFilterValue)/bufferToFill.numSamples;
            }
            
            sample = filter[chan].filterSamples (sample, lastFilterValue, sRate);
            
            //Delay
            if (lastDelayValue != delayValue)
            {
                lastDelayValue += (delayValue - lastDelayValue)/bufferToFill.numSamples;
            }
            
            sample += delay[chan].read(lastDelayValue * (1/secondsPB)) * (lastDelayValue);
            delay[chan].write (sample);
            
            //Reverb
            if (lastReverbValue != reverbValue)
            {
                lastReverbValue += (reverbValue - lastReverbValue)/bufferToFill.numSamples;
            }
           
                if (chan == 0)
                {
                    sample += reverb[chan].read(lastReverbValue * 0.73) * 0.5;
                    
                }
                if (chan == 1)
                {
                    sample += reverb[chan].read(lastReverbValue) * 0.5;
                    
                }
                reverb[chan].write (sample);


            
            
            bufferToFill.buffer->setSample (chan, counter, sample);
            
            
        }
    }
    lastReverbValue = reverbValue;
    lastFilterValue = filterValue;
    lastGain = gain;
    
}


