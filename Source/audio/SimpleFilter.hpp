/*
 *  SimpleFilter.h
 */

#ifndef __SimpleFilter_h__
#define __SimpleFilter_h__

#include <math.h>
#include "../JuceLibraryCode/JuceHeader.h"

/** an enumeration for Low Pass Filter, High Pass Filter, Band Pass Filter and Band Reject Filter*/
enum { kLPF = 0, kHPF, kBPF, kBRF, kNumberOfFilterTypes };

/** A class to setup filters and filter sample values */
class SimpleFilter
{
public:
    /** Constructor
     @param samplerate the sample rate of the audio that will be filtered*/
    SimpleFilter(Float32 samplerate);
    
    /** Configures a specific filter
     @param type The type of filter that you want, the enumeration above is in reference to this
     @param fc The centre frequency/frequency cutoff
     @param bw The bandwidth - Ignored if using a low pass or high pass filter*/
    void FilterConfig(UInt16 type, Float32 fc, Float32 bw);
    
    /** Uses a bi-quad filter setup to filter sample values
     @param sval The sample value input
     @return The filtered sample*/
    Float32 Filter(Float32 sval);
private:
    Float32 fSampleRate;
    UInt16 iType;
    Float32 fFilta[3], fFiltb[3], fFiltvalx[3], fFiltvaly[3];
};

#endif
