#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "CppSingleton.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

struct engine {
    struct android_app* app;

    ASensorManager* sensorManager;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
    Singleton* game;
    bool loaded;
};


//-------------------------------------------
long getTicks() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long msecs = now.tv_nsec / 1000000;
    return (long)now.tv_sec * 1000 + msecs;
}
//-------------------------------------------------
static int engine_init_display(struct engine* engine) {

    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;
    EGLDisplay display;

    if (!engine->loaded) {
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(display, 0, 0);
    }
    else{
        display = engine->display;
    }
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    if (!engine->loaded)
        context = eglCreateContext(display, config, NULL, NULL);
    else
        context = engine->context;

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    if (!engine->loaded) {
        eglQuerySurface(display, surface, EGL_WIDTH, &w);
        eglQuerySurface(display, surface, EGL_HEIGHT, &h);
        engine->width = w;
        engine->height = h;
    }

    engine->display = display;
    engine->context = context;
    engine->surface = surface;

    engine->state.angle = 0;

    glDisable(GL_DEPTH_TEST);


    if (!engine->loaded) {
        if (engine->game) {
            engine->game->init();
            engine->loaded = true;
        }
    }

    engine->animating = 1;

    return 0;
}
//-----------------------------------------------------
//
static void engine_draw_frame(struct engine* engine) {
    if (engine->display == NULL) {
        return;
    }


    if (engine->game) {
        if (getTicks() > engine->game->tick) {

            engine->game->DeltaTime = (getTicks() - engine->game->TimeTicks) / 1000.0f;
            engine->game->TimeTicks = getTicks();

            engine->game->Accumulator += engine->game->DeltaTime;


            while (engine->game->Accumulator >= engine->game->DT){
                engine->game->logic();
                engine->game->Accumulator -= engine->game->DT;
            }


            engine->game->render();
            glFlush();
            eglSwapBuffers(engine->display, engine->surface);

            engine->game->tick = getTicks() + 1000/60;
        }

    }





}
//-------------------------------------------------------
/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) {
    //if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        //if (engine->context != EGL_NO_CONTEXT) {
        //    eglDestroyContext(engine->display, engine->context);
        //}
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        //eglTerminate(engine->display);


    //}
    //engine->display = EGL_NO_DISPLAY;
    //engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
    //engine->animating = 0;
}
//-------------------------------------
/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;

    float scaleX = 640.0f/engine->width;
    float scaleY = 480.0f/engine->height;
    int32_t inputType = AInputEvent_getType(event);
    int32_t act = AMotionEvent_getAction(event);
    if (inputType == AINPUT_EVENT_TYPE_MOTION) {
        switch(act) {
            case AMOTION_EVENT_ACTION_UP: {
                Vector3D v = Vector3D(AMotionEvent_getX(event, 0) * scaleX, AMotionEvent_getY(event, 0) * scaleY, 0);
                engine->game->touches.up.add(v);
            }break;
            case AMOTION_EVENT_ACTION_DOWN :{
                    Vector3D v = Vector3D(AMotionEvent_getX(event, 0) * scaleX, AMotionEvent_getY(event, 0) * scaleY,
                                          0);
                    engine->game->touches.down.add(v);
            }break;
            case AMOTION_EVENT_ACTION_MOVE: {
                    Vector3D v = Vector3D(AMotionEvent_getX(event, 0) * scaleX, AMotionEvent_getY(event, 0) * scaleY,
                                          0);
                    engine->game->touches.move.add(v);
            }
        }

        return 1;
    }

    return 0;
}
//----------------------------------
/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {

                engine->game->AssetManager = app->activity->assetManager;
                engine_init_display(engine);
                engine_draw_frame(engine);

            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS: engine->animating = 1; break;
        case APP_CMD_LOST_FOCUS:
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
        case APP_CMD_DESTROY:{


            if (engine->game) {
                engine->game->destroy();
            }

            if (engine->context != EGL_NO_CONTEXT)
                eglDestroyContext(engine->display, engine->context);

            if (engine->surface != EGL_NO_SURFACE)
                eglDestroySurface(engine->display, engine->surface);

            eglTerminate(engine->display);

            delete engine->game;
        }
    }
}





//-------------------------------------------

void android_main(struct android_app* state) {
    struct engine engine;

    // Make sure glue isn't stripped.
    app_dummy();

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;


    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }

    ((struct engine*)(state->userData))->game = new Singleton();
    ((struct engine*)(state->userData))->loaded = false;

    // loop waiting for stuff to do.
    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;



        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                                      (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

          
            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) {
            engine_draw_frame(&engine);
        }
    }
}
