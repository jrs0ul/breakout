#include <cstdio>
#include "AndroidSoundSystem.h"
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))


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


    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
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
    /*result = (*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &playerPlay);
    if (result != SL_RESULT_SUCCESS)
        return false;
    result = (*playerObj)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE,
                                             &bqPlayerBufferQueue);
    if (result != SL_RESULT_SUCCESS)
        return false;
    result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, NULL);
    if (result != SL_RESULT_SUCCESS)
        return false;
    result = (*playerObj)->GetInterface(playerObj, SL_IID_VOLUME, &bqPlayerVolume);
    assert(SL_RESULT_SUCCESS == result);
    result = (*bqPlayerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
    assert(SL_RESULT_SUCCESS == result);

    // Render and enqueue a first buffer. (or should we just play the buffer empty?)
    curBuffer = 0;
    audioCallback(buffer[curBuffer], BUFFER_SIZE_IN_SAMPLES);

    result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, buffer[curBuffer], sizeof(buffer[curBuffer]));
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }*/

    return true;

    

}

//-----------------------------------------------
void SoundSystem::exit(){

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
