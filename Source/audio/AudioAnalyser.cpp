//
//  AudioAnalyser.cpp
//  JuceBasicAudio - App
//
//  Sam Spreadborough
//

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "../Superpowered/SuperpoweredAnalyzer.h"


#define WAVEFORM_RESOLUTION 150

class AudioAnalyser
: public ThreadWithProgressWindow
{
public:
    AudioAnalyser(AudioFormatReader *newReader)
    : ThreadWithProgressWindow("Analysing..."
                               , true
                               , false)
    , reader(newReader)
    , analyser(nullptr)
    {}
    
    // ThreadWithProgressWindow
    void run() override
    {
        // Initialise variables
        analyser.reset(new SuperpoweredAnalyzer(unsigned(reader->sampleRate), 0, int(reader->lengthInSamples / reader->sampleRate)));
        
        unsigned char **averageWaveform         = new unsigned char*[WAVEFORM_RESOLUTION];
        unsigned char **peakWaveform            = new unsigned char*[WAVEFORM_RESOLUTION];
        unsigned char **lowWaveform             = new unsigned char*[WAVEFORM_RESOLUTION];
        unsigned char **midWaveform             = new unsigned char*[WAVEFORM_RESOLUTION];
        unsigned char **highWaveform            = new unsigned char*[WAVEFORM_RESOLUTION];
        unsigned char **notes                   = new unsigned char*[WAVEFORM_RESOLUTION];
        int            waveformSize             = 0;
        char          **overviewWaveform        = nullptr;
        int            overviewSize             = 0;
        float          averageDecibel           = 0.f;
        float          loudpartsAverageDecibel  = 0.f;
        float          peakDecibel              = 0.f;
        float          bpm                      = 0.f;
        float          beatgridStartMs          = 0.f;
        int            keyIndex                 = 0;
        
        const int64  totalSamples = reader->lengthInSamples;
        const int64  blockSize    = 256;
        
        int64 completed = 0;
        
        AudioSampleBuffer tempBuffer(reader->numChannels, 0);
        
        // Analyse in small block sizes to prevent entire audio file
        // from being loaded into memory
        while (completed < totalSamples)
        {
            const int samplesToProcess = (totalSamples - blockSize) > 0 ? int(blockSize)
            : int(totalSamples);
            
            // Reset temp buffers
            tempBuffer.clear();
            tempBuffer.setSize(reader->numChannels, samplesToProcess);
            
            
            // Read audio from file
            reader->read(&tempBuffer
                         , 0
                         , samplesToProcess
                         , completed
                         , true, true);
            
            // Interleave audio
            
            const int samples = samplesToProcess;
            const int chans = reader->numChannels;
            
            jassert (chans == 2);
            
            auto d = new float[samples * chans];
            
            unsigned int dCount = 0;
            for (int step = 0; step < samplesToProcess; step++)
            {
                for (int chan = 0; chan < chans; chan++)
                {
                    d[dCount++] = tempBuffer.getSample(chan, step);
                }
            }
            
            // Analyse audio
            analyser->process(d, samples);
            
            // Increment the completion and update progress bar
            completed += samplesToProcess;
            setProgress(double(completed) / double(totalSamples));
        }
        
        // Get results
        analyser->getresults(  averageWaveform
                             , peakWaveform
                             , lowWaveform
                             , midWaveform
                             , highWaveform
                             , notes
                             , &waveformSize
                             , overviewWaveform
                             , &overviewSize
                             , &averageDecibel
                             , &loudpartsAverageDecibel
                             , &peakDecibel
                             , &bpm
                             , &beatgridStartMs
                             , &keyIndex
                             );
        
        analysedBpm = bpm;
        keyIndex1 = musicalChordNames[keyIndex];
        beatGridStart = beatgridStartMs;
        
        
        
    }
    
    void threadComplete (bool userPressedCancel) override
    {
        
    }
    
    float getBPM()
    {
        return analysedBpm;
    }
    
    String getKey()
    {
        return keyIndex1;
    }
    
    float getBGS()
    {
        return beatGridStart;
    }
    
    std::unique_ptr<AudioFormatReader>     reader;
    std::unique_ptr<SuperpoweredAnalyzer> analyser;
    
    float analysedBpm;
    float beatGridStart = 0;
    String keyIndex1;
};
