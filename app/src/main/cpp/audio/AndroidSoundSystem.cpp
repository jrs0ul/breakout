#include <cstdio>
#include "AndroidSoundSystem.h"
#include <android/log.h>
#include <cmath>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))

#define SAMPLES_PER_SEC 8000
#define BUF_SAMPLES_MAX SAMPLES_PER_SEC*5
//#define M_PI 180

// sound mixer
static unsigned activeAudioBuffer;

static short buffer[BUF_SAMPLES_MAX];
static volatile bool _bufferActive = false;




SoundBuffer* loadSoundFile(const char* filename, AAssetManager* man){
    SoundBuffer* result = new SoundBuffer();
    LOGI("Opening %s\n", filename);
    AAsset* asset = AAssetManager_open(man, filename, AASSET_MODE_UNKNOWN);
    off_t length = AAsset_getLength(asset);
    result->length = length - sizeof(WAVHeader);
    result->header = new WAVHeader();
    result->buffer = new char[result->length];
    LOGI("length %d\n", result->length);
    AAsset_read(asset, result->header, sizeof(WAVHeader));
    AAsset_read(asset, result->buffer, result->length);
    AAsset_close(asset);
    return result;
}
//---------------------------------------------------
void SoundSystem::loadFiles(const char* BasePath, const char* list, AAssetManager * man){

   /* char buf[255];
    sprintf(buf, "%s%s", BasePath, list);

    AAsset* asset = AAssetManager_open(man, buf, AASSET_MODE_UNKNOWN);

    if (asset) {
        while (!feof(asset)){
            SoundData data;
            data.name[0]=0;
            int result = 0;
            result = fscanf(asset, "%s\n",data.name);
            LOGI("%s\n", data.name);
            audioInfo.add(data);
        }

        AAsset_close(asset);
    }




    for (unsigned int i = 0 ; i<audioInfo.count(); i++){

        sprintf(buf, "%s%s", BasePath, audioInfo[i].name);

        SoundBuffer * s = loadSoundFile(buf, man);
        buffers.add(s);

    }*/

    buffers.add(loadSoundFile("sfx/bang.wav", man));
    buffers.add(loadSoundFile("sfx/byeball.wav", man));
    buffers.add(loadSoundFile("sfx/gunfire.wav", man));
    buffers.add(loadSoundFile("sfx/prize.wav", man));
    buffers.add(loadSoundFile("sfx/reflect.wav", man));
    buffers.add(loadSoundFile("sfx/reflect2.wav", man));

    LOGI("Loaded %u buffers\n", buffers.count());

}

//----------------------------------------------------------
static void PlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    _bufferActive = false;
}
//---------------------------------
bool SoundSystem::init(){
    LOGI("Starting sound service");

    SLresult result;

    // engine
    const SLuint32 engineMixIIDCount = 1;
    const SLInterfaceID engineMixIIDs[] = {SL_IID_ENGINE};
    const SLboolean engineMixReqs[] = {SL_BOOLEAN_TRUE};

    // create engine
    result = slCreateEngine(&engineObj, 0, NULL,
    engineMixIIDCount, engineMixIIDs, engineMixReqs);
    if (result != SL_RESULT_SUCCESS)
        return false;
    result = (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS)
        return false;
    // get interfaces
    result = (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engine);
    if (result != SL_RESULT_SUCCESS)
        return false;
    // mixed output
    const SLuint32 outputMixIIDCount = 0;
    const SLInterfaceID outputMixIIDs[] = {};
    const SLboolean outputMixReqs[] = {};

    // create output
    result = (*engine)->CreateOutputMix(engine, &outPutMixObj,
    outputMixIIDCount, outputMixIIDs, outputMixReqs);
    if (result != SL_RESULT_SUCCESS)
        return false;
    result = (*outPutMixObj)->Realize(outPutMixObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS)
        return false;

    //audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 1};
    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,
            1,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_CENTER,
            SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outPutMixObj};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids[2] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME};
    const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engine)->CreateAudioPlayer(engine, &playerObj, &audioSrc, &audioSnk, 2, ids, req);
    if (result != SL_RESULT_SUCCESS)
        return false;

    result = (*playerObj)->Realize(playerObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS)
        return false;
    result = (*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &soundPlayer);
    if (result != SL_RESULT_SUCCESS)
        return false;
    result = (*playerObj)->GetInterface(playerObj, SL_IID_BUFFERQUEUE,
                                             &bufferQueueObj);
    if (result != SL_RESULT_SUCCESS)
        return false;
    result = (*bufferQueueObj)->RegisterCallback(bufferQueueObj, PlayerCallback, NULL);
    if (result != SL_RESULT_SUCCESS)
        return false;

    result = (*playerObj)->GetInterface(playerObj, SL_IID_VOLUME, &playerVolume);
    if (result != SL_RESULT_SUCCESS)
        return false;
    result = (*soundPlayer)->SetPlayState(soundPlayer, SL_PLAYSTATE_PLAYING);
    if (result != SL_RESULT_SUCCESS)
        return false;

   
    return true;

    

}




//------------------------------------------------
void SoundSystem::playSound(unsigned int index, bool loop){

    //if (_bufferActive) {
    //    return;
    //}


    SLresult result;


    _bufferActive = true;
    result = (*bufferQueueObj)->Enqueue(bufferQueueObj, buffers[index]->buffer, buffers[index]->length);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }

}




//-----------------------------------------------
void SoundSystem::exit(){

    for (unsigned i = 0; i < buffers.count(); i++){
        delete buffers[i];
    }
    buffers.destroy();

    audioInfo.destroy();

    if (playerObj){
        (*playerObj)->Destroy(playerObj);
        playerObj = 0;
    }

    LOGI("Destroying sound output");
    if (outPutMixObj){
        (*outPutMixObj)->Destroy(outPutMixObj);
        outPutMixObj = 0;
    }

    LOGI("Destroy sound engine");
    if (engineObj){
        (*engineObj)->Destroy(engineObj);
        engineObj = 0;
        engine = 0;
    }
}
