

#include "BeatGrid.hpp"

/* Copyright (C) Sam Spreadborough - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Sam Spreadborough <spreads.sam@gmail.com>, 2018
 */

namespace
{
    BeatMarkerData CreateBeatMarker(int beat, float sample, float sampleRate)
    {
        return { sample
            , sample / sampleRate
            , beat
            , beat % 4 == 0 };
    }
}

BeatGrid::BeatGrid(float bpm, float sampleRate, float firstBeatSample)
: bpm             (bpm)
, sampleRate      (sampleRate)
, firstBeatSample (firstBeatSample)
{}

BeatGrid::BeatGrid(BeatGrid &other)
: bpm             (other.bpm)
, sampleRate      (other.sampleRate)
, firstBeatSample (other.firstBeatSample)
{}

BeatGrid::BeatGrid(BeatGrid &&other)
: bpm             (std::move(other.bpm))
, sampleRate      (std::move(other.sampleRate))
, firstBeatSample (std::move(other.firstBeatSample))
{}

float    BeatGrid::GetBpm()             const { return bpm; }
float BeatGrid::GetSampleRate()      const { return sampleRate; }
float BeatGrid::GetFirstBeatSample() const { return firstBeatSample; }

BeatMarkerData BeatGrid::FindNextBeat(float sample) const
{
    double nextBeat       = std::ceil (GetBeatFraction(sample));
    double nextBeatSample = std::floor(nextBeat * GetBeatLength() + firstBeatSample);
    
    return CreateBeatMarker(nextBeat, nextBeatSample, sampleRate);
}

BeatMarkerData BeatGrid::FindPrevBeat(float sample) const
{
    double prevBeat       = std::floor(GetBeatFraction(sample));
    double prevBeatSample = std::floor(prevBeat * GetBeatLength() + firstBeatSample);
    
    return CreateBeatMarker(prevBeat, prevBeatSample, sampleRate);
}

BeatMarkerData BeatGrid::FindClosestBeat(float sample) const
{
    double nextBeat          = std::ceil (GetBeatFraction(sample));
    double prevBeat          = std::floor(GetBeatFraction(sample));
    double closestBeat       = (nextBeat - sample > sample - prevBeat) ? prevBeat : nextBeat;
    double closestBeatSample = std::floor(closestBeat * GetBeatLength() + firstBeatSample);
    
    return CreateBeatMarker(closestBeat, closestBeatSample, sampleRate);
}

std::vector<BeatMarkerData> BeatGrid::FindBeats(float startSample, float stopSample) const
{
    std::vector<BeatMarkerData> markers;
    
    while (startSample < stopSample)
    {
        markers.push_back(FindNextBeat(startSample));
        startSample += GetBeatLength();
    }
    
    return markers;
}

float BeatGrid::GetBeatLength() const
{
    return double(60.0 * sampleRate / bpm);
}

double BeatGrid::GetBeatFraction(float sample) const
{
    return (sample - firstBeatSample) / GetBeatLength();
}


