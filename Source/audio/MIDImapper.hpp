//
//  MIDImapper.hpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 12/03/2019.
//

#ifndef MIDImapper_hpp
#define MIDImapper_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioState.hpp"


class MidiMapper : public MidiInputCallback
{
public:
    
    MidiMapper();
    
    ~MidiMapper();
    
    ValueTree& getState() {return state->getState();}

    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override;
    
    
private:
    AudioDeviceManager audioDeviceManager;
    std::unique_ptr<AudioState> state;
    bool noteOn = false;
    
};

#endif /* MIDImapper_hpp */

