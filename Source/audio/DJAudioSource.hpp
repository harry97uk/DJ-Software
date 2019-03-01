/* Copyright (C) Sam Spreadborough - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Sam Spreadborough <spreads.sam@gmail.com>, 2018
 */

//==============================================================================
/** An AudioSource that can play audio from an AudioFormatReader with settable
 pitch and playhead position.
 
 Pitch and position are settable, and pitch can be set negatively (causing
 the audio to be played in reverse), making it very useful when developing
 DJ applications.
 
 Position can be outside of the file's audio bounds. If this happens, no
 audio will be read from the file, but the playhead will be incremented (useful
 for scratching audio when the playhead is < bounds.
 
 When reading from file, this class will not copy the entire file into
 memory so it's memory usage is very low.
 
 For best results, use with a MixerAudioSource.
 
 @see AudioSource, MixerAudioSource
 */
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioSource
: public  AudioSource
{
public:
    //==============================================================================
    /** Constructs a new DJAudioSource with no source.
     
     To start the DJAudioSource playing, call SetSource().
     */
    DJAudioSource();
    
    ~DJAudioSource();
    
    /** Set the source for this AudioSource to use.
     
     DJAudioSource can optionally take ownership of the AudioFormatReader,
     which will cause the AudioFormatReader to be deleted when its no longer
     needed.
     
     Setting this source to nullptr will clear the current AudioSource and
     no audio will be played.
     
     @param formatReader an AudioFormatReader to read from
     @param deleteWhenFinished whether this AudioSource takes ownership of the
     reader
     */
    void SetSource(AudioFormatReader *formatReader, bool deleteWhenFinished = true);
    
    /** @return the current source.
     */
    AudioFormatReader *GetSource() const;
    
    /** Sets whether this AudioSource is playing or not.
     
     With a source set and playing false the playhead will not be incremented
     and any buffer that gets passed in to fill will just be cleared.
     
     @param newPlaying the new playing state
     */
    void SetPlaying(bool newPlaying);
    
    /** Sets the new pitch.
     
     Pitch should be relative, not absolute BPM values.
     
     Pitch can be negative, and this will cause the AudioSource to play in
     reverse.
     
     @param newPitch the new pitch value to set.
     */
    void SetPitch(double newPitch);
    
    /** Sets the playhead position in samples.
     
     Playhead can be outside of the file sample length bounds, as any playhead
     position outside these bounds won't cause the file to be read from.
     
     @param newPlayheadSample the playhead position to set this AudioSource to
     */
    void SetPlayheadSample(double newPlayheadSample);
    
    /** Sets the playhead position in seconds.
     
     Playhead can be outside of the file sample length bounds, as any playhead
     position outside these bounds won't cause the file to be read from.
     
     @param newPlayheadSecond the playhead position to set this AudioSource to.
     */
    void SetPlayheadSeconds(double newPlayheadSecond);
    
    /** Applies a gain multiple to all the audio data.
     
     @param newGain the gain to set.
     */
    void SetGain(float newGain);
    
    /** @return whether the AudioSource is currently playing
     */
    bool IsPlaying() const;
    
    /** @return the current pitch
     */
    double GetPitch() const;
    
    /** @return the current playhead position
     */
    double GetPlayhead() const;
    
    /** @return the current playhead position in seconds
     */
    double GetSeconds() const;
    
    /** @return the length in seconds
     */
    int GetLengthSeconds() const;
    
    /** @return the length in samples
     */
    float GetLengthSamples() const;
    
    /** @return the set gain multiplier
     */
    float GetGain() const;
    
    /** If the playhead requires custom setting behaviour (e.g looping, rolls etc)
     then set that behaviour here.
     
     This processing will be done in realtime for every audio block.
     
     @code
     // Loop
     
     ProcessNewPosition = [=]( sample_t currentPosition
     , pitch_t  currentPitch
     , int      numSamples
     , bool     playheadIsValid ) -> sample_t
     {
     if (currentPosition > loopEnd)
     return loopStart
     else if (currentPosition > loopEnd)
     return loopEnd
     else
     return currentPosition + (currentPitch * numSamples);
     };
     
     @endcode
     
     If no std::function is set, then DJAudioSource will simply
     increment the playhead by the pitch.
     */
    std::function<double (  double currentPosition
                            , double  currentPitch
                            , int      numSamples
                            , bool     playheadIsValid)> customPlayheadProcessing;
    
    
    
    
    //  ===============================================================================
    /** @internal */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override {}
    /** @internal */
    void releaseResources() override;
    /** @internal */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
private:
    //==============================================================================
    void Reset();
    bool PlayheadIsValid();
    void IncrementPlayhead(int numSamples);
    
    CriticalSection criticalSection;
    
    AudioSampleBuffer buffer;
    int bufferSize;
    
    CatmullRomInterpolator interpolator[2];
    OptionalScopedPointer<juce::AudioFormatReader> reader;
    
    bool     playing;
    double playhead;
    double  pitch;
    float    gain;
};
