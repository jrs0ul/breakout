#ifndef _CPP_SINGLETON
#define _CPP_SINGLETON

#include <ctime>
#ifndef __ANDROID__
#include "audio/SoundSystem.h"
#include "audio/OggStream.h"
#endif
#include "TextureLoader.h"
#include "Vectors.h"
#include "TouchData.h"
#include "gui/Text.h"
#include "SysConfig.h"
#include "Utils.h"
#include "Particles2D.h"
#include "Padle.h"
#include "Ball.h"
#include "HighScore.h"
#include "BBullets.h"
#include "gui/OnScreenBoard.h"

//-------------------------------------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


enum GameStates{TITLE, GAME};
enum PrizeTypes{GROW, SHRINK, MAGNET, NOCLIP,
                MULTIPLY, SLOWDOWN, GUNS, DEATH};


#ifndef __IPHONEOS__
//   #define _DEBUG_ 1
#endif

struct Prizas{
    Vector3D pos;
    Vector3D vel;
    int type;
};



//============================================
class Singleton{
    
    void bulletLogic();
    void onPadDeath();
    void onPrizePickup(unsigned i);
    void updateParticleSystems();
    void drawWallpaper();
    void scrollWallpaper();
    void onTitleScreen();
    void clearBricks();

public:
  
#ifdef __ANDROID__
    AAssetManager* AssetManager;
#endif

    GameStates gameState;

    DArray<Prizas> Prizai;
    DArray<ColidedBrick> cbricks;

    int ScreenWidth;
    int ScreenHeight;
    bool windowed;

    char DocumentPath[255];

    unsigned char Keys[20];
    unsigned char OldKeys[20];
    Vector3D gamepad;

#ifndef __ANDROID__
    SoundSystem ss;
    OggStream music;
#endif
    
    PicsContainer pics;
    
    long TimeTicks;
    
    int MouseXOld, MouseYOld, MouseX, MouseY, MouseButton, MouseButtonOld;
    
    
    Vector3D oldMoveTouch;
    
    char Key, OldKey;
    
    
    unsigned globalKEY;
    
    
    
    bool useAccel;
    Vector3D _acceleration;
    Vector3D firstTouch;
    Vector3D pos;
    
    SystemConfig sys;
    
    int clickCount;
    
    TouchData touches;
    
    
    bool Works;

    //---


    long tick;
    BreakOutMap Map;
    DArray<Ball> balls;
    Padle padle;

    bool fall;

    int lives;
    long Score;

    char nameToEnter[256];

    bool ShowDebugText;
    bool GeneratingTexture;
    bool GamePaused;
    int bgpushy;
    int bgpushx;
    int scrooltim;

    bool  padleKilled;
    float padleAlpha;
    bool  padleRevive;

    OnScreenKeyboard NameBox;


    DArray<Particle2DSystem> PSystems;

    HighScores Scores;

    BulletContainer Bullets;
    bool ReflectBricks;
    int NextLevelTimer;

    int PadX;

        
    void CreatePrize(float x, float y);
    void EliminateBrick(int tx, int ty,float speed);
    void GenerateTexture(unsigned width, unsigned height);
    void RenderScreen();
    void GameLoop();
    void TakeNewLife();
    void KillPadd();
    void ResetGame();
    void GoNextLevel();
    void DisplayLives(int x, int y);
    void DrawDebugText();
    int  FPS();
    
    Singleton(){


#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        ScreenWidth = 640;
        ScreenHeight = 480;
#else
        ScreenWidth = 640;
        ScreenHeight = 480;
        windowed = false;
#endif

        clickCount = 0; 
                    
                    
        
                //---
                    
                
                    
        oldMoveTouch = Vector3D(-1,-1,0);

        Works = true;

        tick = 0;
        fall = false;
        lives = 3;
        Score = 0;
        ShowDebugText = true;
        GeneratingTexture = false;
        GamePaused = false;
        bgpushy = 0;
        bgpushx = 0;
        scrooltim = 0;
        padleKilled = false;
        padleAlpha = 1.0f;
        padleRevive = false;
        ReflectBricks = true;
        NextLevelTimer = 0;

        PadX = 0;
                
    }
    
    void init();
    void loadConfig();
    void logic();
    void render();
    void destroy();
    
};


#endif //_CPP_SINGLETON

