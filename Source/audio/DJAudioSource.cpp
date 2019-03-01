/* Copyright (C) Sam Spreadborough - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Sam Spreadborough <spreads.sam@gmail.com>, 2018
 */

#include "DJAudioSource.hpp"

DJAudioSource::DJAudioSource()
: playing  (true)
, playhead (0.0)
, pitch    (1.0)
, gain     (1.f)
{}

DJAudioSource::~DJAudioSource()
{
    
}

void DJAudioSource::SetSource(AudioFormatReader *formatReader, bool deleteWhenFinished)
{
    const ScopedLock lock(criticalSection);
    
    
    
    Reset();
    reader.set(formatReader, deleteWhenFinished);
}

AudioFormatReader *DJAudioSource::GetSource() const
{
    return reader;
}

void DJAudioSource::SetPlaying(bool newPlaying)
{
    const ScopedLock lock(criticalSection);
    
    playing = newPlaying;
}

void DJAudioSource::SetPitch(double newPitch)
{
    const ScopedLock lock(criticalSection);
    
    pitch = newPitch;
    
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
        const bool reverse = pitch < 0.f;
        const int  tempSampleSize = GetLengthSamples() * (2 * fabs(pitch));
        bufferSize = tempSampleSize;
        
        buffer.setSize(bufferToFill.buffer->getNumChannels(), tempSampleSize);
        
        reader->read(  &buffer
                     , 0
                     , bufferToFill.numSamples
                     , int64(playhead) - (reverse ? bufferToFill.numSamples : 0)
                     , true, true);
        
        buffer.applyGain(gain);
        
        if (reverse)
            buffer.reverse(0, bufferToFill.numSamples);
        
        interpolator[0].process(  fabs(pitch)
                                , buffer.getReadPointer(0)
                                , bufferToFill.buffer->getWritePointer(0)
                                , bufferToFill.numSamples);
        
        interpolator[1].process(  fabs(pitch)
                                , buffer.getReadPointer(1)
                                , bufferToFill.buffer->getWritePointer(1)
                                , bufferToFill.numSamples);
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
        
        if (customPlayheadProcessing)
        {
            playhead = customPlayheadProcessing(  playhead
                                                , pitch
                                                , numSamples
                                                , PlayheadIsValid());
        }
        else
        {
            playhead += numSamples * pitch;
        }
    }
}

void DJAudioSource::Reset()
{
    const ScopedLock lock(criticalSection);
    
    playhead = 0.0;
    reader.reset();
    interpolator[0].reset();
    interpolator[1].reset();
}

bool DJAudioSource::PlayheadIsValid()
{
    return reader && playhead >= 0.0 && playhead < reader->lengthInSamples;
}
