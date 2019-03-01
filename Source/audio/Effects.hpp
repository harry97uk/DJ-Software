//
//  Effects.hpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 16/01/2019.
//

#ifndef Effects_hpp
#define Effects_hpp

#include <stdio.h>
#include "SimpleFilter.hpp"
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Superpowered/SuperpoweredFilter.h"


class Effects
{
public:
    Effects(float sampleRate);
    
    ~Effects();
    
    float* filterAudio(float* inputAudio, int samplepf, float sliderValue);
    
    
    
    
private:
    SuperpoweredFilter filter;
};

#endif /* Effects_hpp */
