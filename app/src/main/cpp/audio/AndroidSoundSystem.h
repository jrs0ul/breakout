#ifndef ANDROID_SOUND_SYSTEM_H
#define ANDROID_SOUND_SYSTEM_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "../DArray.h"


//----------

class SoundSystem{

    // engine
    SLObjectItf engineObj;
    SLEngineItf engine;
    // output
    SLObjectItf outPutMixObj;


    // sound
    int SLObjectItf  playerObj;
    int soundPlayer;
    int soundVolume;
    SLObjectItf soundQueue;
    // sound mixer
    unsigned activeAudioOutSoundBuffer;

public:
    SoundSystem(){ outPutMixObj = 0; engineObj = 0; }
    bool init();
    void loadFiles(const char* BasePath, const char* list);
    void setupListener(float * pos, float * orientation);
    void setSoundPos(unsigned int index, float * pos);
    void playsound(unsigned int index, bool loop=false);
    void freeData();
    bool isPlaying(unsigned int index);
    void stopAll();
    void exit();

};




#endif //ANDROID_SOUND_SYSTEM_H
