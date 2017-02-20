#ifdef WIN32
    #ifdef  _MSC_VER
        #define _CRT_SECURE_NO_DEPRECATE 1
        #pragma comment(lib,"SDL.lib")
        #pragma comment(lib,"SDLmain.lib")
        #pragma comment(lib,"OpenGl32.lib")
        #pragma comment(lib,"GLU32.lib")
        #pragma comment(lib,"openal32.lib")

        #ifdef _DEBUG
            #pragma comment(lib,"ogg_d.lib")
            #pragma comment(lib,"vorbis_d.lib")
            #pragma comment(lib,"vorbisfile_d.lib")
        #else
            #pragma comment(lib,"ogg.lib")
            #pragma comment(lib,"vorbis.lib")
            #pragma comment(lib,"vorbisfile.lib")
        #endif
    #endif
#endif



#include <ctime>
#include "CppSingleton.h"
#include "SDLVideo.h"
#include "Utils.h"
#ifdef __APPLE__
#include <limits.h>
#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>
#endif


//const int WIDTH = 800;
//const int HEIGHT = 600;
//bool windowed = false;

SDLVideo SDL;
SDL_Joystick *Joy = 0;
int JoyX = 0;
int JoyY = 0;
int MouseX, MouseY; //relative mouse coords
int _MouseX, _MouseY;
unsigned long tick;

Singleton Game;


void ConfGL(){
    Game.init();
}
//-----------------
void RenderScreen(){
    Game.render();
    glFlush();

    SDL_GL_SwapBuffers( );
}
//-----------------
void Logic(){
    Game.logic();
}
//-----------------
static void  process_events(){
    
    SDL_Event event;
    float scaleX = 640.0f/Game.ScreenWidth;
    float scaleY = 480.0f/Game.ScreenHeight;

    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {

        case SDL_KEYDOWN:{

            Game.globalKEY = (char)event.key.keysym.unicode;
            switch( event.key.keysym.sym ) {
                default:{} break;
                case SDLK_q: { Game.Works = false;}
            }
        } break;
        case SDL_MOUSEBUTTONUP:{
            Vector3D pos(event.button.x * scaleX, event.button.y * scaleY, 0);
            //printf("up x:%f y:%f\n", pos.x() , pos.y());
            Game.touches.up.add(pos);
            Game.touches.allfingersup = true;
        } break;
        case SDL_MOUSEBUTTONDOWN:{
            Vector3D pos(event.button.x * scaleX, event.button.y * scaleY, 0);
            //printf("down x:%f y:%f\n", pos.x() , pos.y());
            Game.touches.down.add(pos);
            Game.touches.allfingersup = false;

        } break;
        case SDL_MOUSEMOTION:{
            if(SDL_GetMouseState(0, 0)&SDL_BUTTON_LMASK){
                Vector3D pos(event.button.x * scaleX, event.button.y * scaleY, 0);
                //printf("motion x:%f y:%f\n", pos.x() , pos.y());
                Game.touches.move.add(pos);
                Game.touches.allfingersup = false;
            }
        }break;


        case SDL_QUIT:{
            Game.Works = false;
        }break;
    
        }

    }
}
//--------------------
void checkKeys(){
    float scaleX = 640.0f/Game.ScreenWidth;
    float scaleY = 480.0f/Game.ScreenHeight;
    Uint8 * keys;
    int JoyNum = 0;
    
    keys = SDL_GetKeyState ( NULL );
    JoyNum = SDL_NumJoysticks();

    if (JoyNum > 0) {

        //printf("%s\n", SDL_JoystickName(0));

        SDL_JoystickUpdate ();
        JoyX = SDL_JoystickGetAxis(Joy, 0);
        JoyY = SDL_JoystickGetAxis(Joy, 1);
    }

    
    SDL_GetRelativeMouseState ( &MouseX,&MouseY );
    SDL_GetMouseState(&_MouseX, &_MouseY);

    Game.gamepad.v[0] = JoyX/ 1000.0f;
    Game.gamepad.v[1] = JoyY/ 1000.0f;
    
    Game.MouseX = _MouseX*scaleX;
    Game.MouseY = _MouseY*scaleY;

    //Game.gamepad.v[0] = MouseX * 10.0f;
    //Game.gamepad.v[1] = MouseY * 10.0f;


    memcpy(Game.OldKeys, Game.Keys, 20);
    memset(Game.Keys, 0, 20);

    if ( keys[SDLK_w] )    Game.Keys[0] = 1;
    if ( keys[SDLK_s] )    Game.Keys[1] = 1;
    if ( keys[SDLK_a] )    Game.Keys[2] = 1;
    if ( keys[SDLK_d] )    Game.Keys[3] = 1;
    if ( keys[SDLK_UP] )   Game.Keys[0] = 1;
    if ( keys[SDLK_DOWN])  Game.Keys[1] = 1;
    if ( keys[SDLK_LEFT])  Game.Keys[2] = 1;
    if ( keys[SDLK_RIGHT]) Game.Keys[3] = 1;
    if ( keys[SDLK_SPACE]) Game.Keys[4] = 1;
    if ( keys[SDLK_RETURN]) Game.Keys[5] = 1;
    if ( keys[SDLK_ESCAPE]) Game.Keys[6] = 1;

    if (JoyNum){
        if (SDL_JoystickGetButton (Joy, 0))
            Game.Keys[4] = 1;
        if (SDL_JoystickGetButton (Joy, 1))
            Game.Keys[5] = 1;
    }
}
//--------------------
int main( int   argc, char *argv[] ){
    
    srand(time(0));

    char buf[255];
    GetHomePath(buf);
    sprintf(Game.DocumentPath, "%s.breakout", buf);
    MakeDir(Game.DocumentPath);
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)){
        puts("Um...Error?");
    }
    CFRelease(resourcesURL);
    chdir(path);
#endif
    Game.loadConfig();
    

    printf("%d %d\n", Game.ScreenWidth, Game.ScreenHeight);
    SDL.setMetrics(Game.ScreenWidth, Game.ScreenHeight);
    if (!SDL.InitWindow("breakout", "icon.bmp", Game.windowed)){
        Game.Works = false;
    }

    if (!Game.windowed)
        SDL_ShowCursor(SDL_DISABLE);

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    if(SDL_NumJoysticks() > 0){
        Joy = SDL_JoystickOpen(0);
    }
    SDL_EnableUNICODE(SDL_ENABLE);
    ConfGL();
    //LoadExtensions();

    Game.TimeTicks = SDL_GetTicks();    

    while (Game.Works){


        if ((SDL_GetTicks() > tick)){

            Game.DeltaTime = (SDL_GetTicks() - Game.TimeTicks) / 1000.0f;
            Game.TimeTicks = SDL_GetTicks();

            Game.Accumulator += Game.DeltaTime;

            while (Game.Accumulator >= Game.DT){
                Logic();
                Game.Accumulator -= Game.DT;
            }

            checkKeys();
            RenderScreen();

            tick = SDL_GetTicks() + 1000/60;
        }

        SDL_Delay(1);

        process_events();



    }

    Game.destroy();

    SDL.Quit();

    return 0;
}

