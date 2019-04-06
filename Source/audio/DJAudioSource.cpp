/* Copyright (C) Sam Spreadborough - All Rights Reserved
 * Written by Sam Spreadborough <spreads.sam@gmail.com>, 2018
 */

#include "DJAudioSource.hpp"

DJAudioSource::DJAudioSource()
: playing  (false)
, playhead (0.0)
, pitch    (1.0)
, gain     (1.f)

{}

DJAudioSource::~DJAudioSource()
{
    Reset();
}

void DJAudioSource::SetSource(AudioFormatReader *formatReader, bool deleteWhenFinished)
{
    const ScopedLock lock(criticalSection);
    
    
   

    Reset();
    reader.set(formatReader, deleteWhenFinished);
    lengthInSamples = formatReader->lengthInSamples;
    numChannels = formatReader->numChannels;
    bufferSize = lengthInSamples * 2;
    buffer.setSize(numChannels, bufferSize);
    
    reader->read(  &buffer
                 , 0
                 , (int)lengthInSamples
                 , 0
                 , true, true);
    

    
}

AudioFormatReader *DJAudioSource::GetSource() const
{
    return reader;
}

void DJAudioSource::SetPlaying(bool newPlaying)
{
    //const ScopedLock lock(criticalSection);
    
    playing = newPlaying;
}

void DJAudioSource::setReverse(bool isReverse)
{
    reverse = isReverse;
}

void DJAudioSource::SetPlayheadSample(double newPlayheadSample)
{
    const ScopedLock lock(criticalSection);
    
    playhead = newPlayheadSample;
}

void DJAudioSource::SetPlayheadSeconds(double newPlayheadSecond)
{
    double samplePos = reader ? newPlayheadSecond * reader->sampleRate
    : 0.0;
    
    SetPlayheadSample(samplePos);
}

void DJAudioSource::SetGain(float newGain)
{
    const ScopedLock lock(criticalSection);
    
    gain = newGain;
}

void DJAudioSource::setLoop(bool isLooping, float endPos, float startPos)
{
    //look for positive to negative change
    loop = isLooping;
    float startSample = buffer.getSample(0, startPos);
    if (startSample > 0)
    {
        while (startSample > 0)
        {
            startSample = buffer.getSample(0, startPos++);
        }
    }
    else if (startSample < 0)
    {
        while (startSample < 0)
        {
            startSample = buffer.getSample(0, startPos++);
        }
    }
    
    float endSample = buffer.getSample(0, endPos);
    if (endSample > 0)
    {
        while (endSample > 0)
        {
            endSample = buffer.getSample(0, endPos++);
        }
    }
    else if (endSample < 0)
    {
        while (endSample < 0)
        {
            endSample = buffer.getSample(0, endPos++);
        }
    }
    endPosition = endPos;
    startPosition = startPos;
}

float    DJAudioSource::GetGain()     const { return gain;     }
bool     DJAudioSource::IsPlaying()   const { return playing;  }
double  DJAudioSource::GetPitch()    const { return pitch;    }
double DJAudioSource::GetPlayhead() const { return playhead; }



float DJAudioSource::GetLengthSamples() const { return reader ? double(reader->lengthInSamples) : 0.0; }

void DJAudioSource::releaseResources()
{
    Reset();
}

void DJAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if (PlayheadIsValid() && IsPlaying())
    {
        
        
        const int64 sourceStartSample = int64 (playhead) - (reverse ? bufferToFill.numSamples : 0);
        bufferToFill.buffer->copyFrom (0, bufferToFill.startSample, buffer, 0, (int)sourceStartSample, bufferToFill.numSamples);
        bufferToFill.buffer->copyFrom (1, bufferToFill.startSample, buffer, 1, (int)sourceStartSample, bufferToFill.numSamples);
        
        if (reverse)
            bufferToFill.buffer->reverse(0, bufferToFill.numSamples);
       

    }
    else
    {
        bufferToFill.clearActiveBufferRegion();
    }
    
    IncrementPlayhead(bufferToFill.numSamples);
}

void DJAudioSource::IncrementPlayhead(int numSamples)
{
    // Increment playhead even if playhead isn't valid
    if (reader && playing)
    {
        const ScopedLock lock(criticalSection);
        
            if (loop == true)
            {
                if (playhead >= endPosition)
                {
                    playhead = startPosition;
                }
                else
                    playhead += numSamples;
            }
            else
                playhead += numSamples;
    }
}

void DJAudioSource::Reset()
{
    const ScopedLock lock(criticalSection);
    
    playhead = 0.0;
    reader.reset();

}

bool DJAudioSource::PlayheadIsValid()
{
    return reader && playhead >= 0.0 && playhead < reader->lengthInSamples;
}
