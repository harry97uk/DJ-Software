

#ifndef BeatGrid_hpp
#define BeatGrid_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

/* Copyright (C) Sam Spreadborough - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Sam Spreadborough <spreads.sam@gmail.com>, 2018
 */

//==============================================================================
/** Data representing a beat grid marker
 */
struct BeatMarkerData
{
    const float sample;
    const float seconds;
    const int      index;
    const bool     downBeat;
};

//==============================================================================
/**
 Creates an infinite beat grid for a track using the BPM, sample rate
 and the beat grid start.
 
 Find the next, previous and closest beat grid markers for a given sample
 position, and get a range of beat markers given a range of sample positions.
 
 BeatMarkerData holds the sample position, the number of seconds it represents,
 the index of the marker, and whether or not that marker is a downbeat
 (assuming that all music is in 4/4... which it is!)
 
 TODO: Support multiple time signatures
 */
class BeatGrid
{
public:
    //==============================================================================
    BeatGrid(float bpm, float sampleRate, float firstBeatSample);
    BeatGrid(BeatGrid &other);
    BeatGrid(BeatGrid &&other);
    
    /** @return the set bpm
     */
    float GetBpm() const;
    
    /** @return the set sample rate
     */
    float GetSampleRate() const;
    
    /** @return the sample of the first beat
     */
    float GetFirstBeatSample() const;
    
    /** @return length in samples
     */
    float GetBeatLength() const;
    
    /** @param sample the sample to check
     @return the percentage progress between previous beat marker and the
     next one. Value is between 0.0 and 1.0
     */
    double GetBeatFraction(float sample) const;
    
    /**
     Given a sample, will return the next sample position that adheres to the
     grid.
     
     @param sample the sample to find the next grid marker from
     @return the found BeatMarkerData
     */
    BeatMarkerData FindNextBeat(float sample) const;
    
    /**
     Given a sample, will return the previous sample position that adheres to the
     grid.
     
     @param sample the sample to find the previous grid marker from
     @return the found BeatMarkerData
     */
    BeatMarkerData FindPrevBeat (float sample) const;
    
    /**
     Given a sample, will find the closest beat marker to it
     
     @param sample the sample to find the closest grid marker from
     @return the found BeatMarkerData
     */
    BeatMarkerData FindClosestBeat(float sample) const;
    
    /**
     Will return a std::vector of BeatMarkerData representing the grid
     given a start and end position.
     
     @param startSample the sample to start the range from
     @param stopSample the sample to end the range at
     @return a container of BeatMarkerData representing the given beat grid range
     */
    std::vector<BeatMarkerData> FindBeats (float startSample, float stopSample) const;
    

    
private:
    //==============================================================================
    float    bpm;
    float sampleRate;
    float firstBeatSample;
};

#endif /* BeatGrid_hpp */
