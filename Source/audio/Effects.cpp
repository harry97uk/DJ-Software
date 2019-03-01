//
//  Effects.cpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 16/01/2019.
//

#include "Effects.hpp"

Effects::Effects(float sampleRate) : filter{SuperpoweredFilter_Resonant_Highpass, static_cast<unsigned int>(sampleRate)}
{
    
}

Effects::~Effects()
{
    
}

float* Effects::filterAudio(float* inputAudio, int samplepf, float sliderValue)
{
    if (sliderValue >= 0)
    {
        filter.setResonantParameters((1 - fabs(sliderValue)) * 20000, 0);
    }
    else if (sliderValue < 0)
    {
        filter.setResonantParametersAndType(sliderValue * 20000, 0, SuperpoweredFilter_Resonant_Lowpass);
    }

   
    
    filter.process(inputAudio, inputAudio, samplepf);
    
    return inputAudio;
}
