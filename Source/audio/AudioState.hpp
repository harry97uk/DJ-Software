//
//  AudioState.hpp
//  JuceBasicAudio - App
//
//  Created by Harry Gardiner on 20/03/2019.
//

#ifndef AudioState_hpp
#define AudioState_hpp

#include "../../JuceLibraryCode/JuceHeader.h"

class AudioState
{
public:
    static const Identifier Parameters;
    static const Identifier bassChannel0;
    static const Identifier bassChannel1;
    static const Identifier midChannel0;
    static const Identifier midChannel1;
    static const Identifier highChannel0;
    static const Identifier highChannel1;
    static const Identifier fileGainChannel0;
    static const Identifier fileGainChannel1;
    static const Identifier bpmChannel0;
    static const Identifier bpmChannel1;
    static const Identifier filterChannel0;
    static const Identifier filterChannel1;
    static const Identifier masterVolume;
    static const Identifier crossfader;
    static const Identifier selectionControlPlus;
    static const Identifier selectionControlMinus;
    
    static const Identifier playButtonChan0;
    static const Identifier playButtonChan1;
    static const Identifier syncButtonChan0;
    static const Identifier syncButtonChan1;
    static const Identifier inLoopChan0;
    static const Identifier inLoopChan1;
    static const Identifier outLoopChan0;
    static const Identifier outLoopChan1;
    static const Identifier eightbarChan0;
    static const Identifier eightbarChan1;
    static const Identifier halfLoopChan0;
    static const Identifier halfLoopChan1;
    static const Identifier loadA;
    static const Identifier loadB;
    
    
    AudioState() : audioState (Parameters)
    {
        setDefaults();
    }
    
    void setDefaults()
    {
        audioState.setProperty (bassChannel0, 0.0, nullptr);
        audioState.setProperty (bassChannel1, 0.0, nullptr);
        audioState.setProperty(midChannel0, 0.0, nullptr);
        audioState.setProperty(midChannel1, 0.0, nullptr);
        audioState.setProperty(highChannel0, 0.0, nullptr);
        audioState.setProperty(highChannel1, 0.0, nullptr);
        audioState.setProperty(fileGainChannel0, 1.0, nullptr);
        audioState.setProperty(fileGainChannel1, 1.0, nullptr);
        audioState.setProperty(bpmChannel0, 0.0, nullptr);
        audioState.setProperty(bpmChannel1, 0.0, nullptr);
        audioState.setProperty(filterChannel0, 0.0, nullptr);
        audioState.setProperty(filterChannel1, 0.0, nullptr);
        audioState.setProperty(masterVolume, 1.0, nullptr);
        audioState.setProperty(crossfader, 0.0, nullptr);
        audioState.setProperty(selectionControlPlus, false, nullptr);
        audioState.setProperty(selectionControlMinus, false, nullptr);
        
        audioState.setProperty(playButtonChan0, false, nullptr);
        audioState.setProperty(playButtonChan1, false, nullptr);
        audioState.setProperty(syncButtonChan0, false, nullptr);
        audioState.setProperty(syncButtonChan1, false, nullptr);
        audioState.setProperty(inLoopChan0, false, nullptr);
        audioState.setProperty(inLoopChan1, false, nullptr);
        audioState.setProperty(outLoopChan0, false, nullptr);
        audioState.setProperty(outLoopChan1, false, nullptr);
        audioState.setProperty(eightbarChan0, false, nullptr);
        audioState.setProperty(eightbarChan1, false, nullptr);
        audioState.setProperty(halfLoopChan0, false, nullptr);
        audioState.setProperty(halfLoopChan1, false, nullptr);
        audioState.setProperty(loadA, false, nullptr);
        audioState.setProperty(loadB, false, nullptr);
    }
    
    ValueTree& getState() {return audioState;}
private:
    ValueTree audioState;
};

#endif /* AudioState_hpp */
