#define _CRT_SECURE_NO_DEPRECATE 1
#define WIN32_LEAN_AND_MEAN


#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")


#pragma comment(lib,"vorbis_static.lib")
#pragma comment(lib,"vorbisfile_static.lib")
#pragma comment(lib,"ogg.lib")

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"winmm.lib")



#include <windows.h>  //windows stuff
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdio>     //sprinf
#include <ctime>
#include <mmsystem.h> // timeGetTime & wave


#include "input.h"    //inputo klase
#include "soundfx.h"
#include "oggplay.h"
#include "Usefull.h"
#include "Renderer3d.h"
#include "Picture3d.h"
#include "WriteText.h"
#include "SystemConf.h"
#include "Darray.h"
#include "map.h"
#include "Particles2D.h"
#include "Padd.h"
#include "Ball.h"
#include "EditBox.h"
#include "HighScore.h"
#include "BBullets.h"




#ifdef _DEBUG
#include <crtdbg.h>//ieskoti memory leaku
#endif



//===================================GLOBALS

struct Prizas{
    D3DXVECTOR2 pos;
    D3DXVECTOR2 vel;
    int type;
};


const float DEFAULT_SPEED=3.5f;
const float MAX_SPEED=8.0f;


//inputs
XInput input;

Renderer3D Renderer;


LPDIRECTSOUND8 pSound; 


WNDCLASSEX windowsclass;
HWND hwndMain;
HINSTANCE hinstanceMain;



DArray<Prizas> Prizai;


PictureContainer img;
Gabalas* smotai; // wav samplai
unsigned int imgCount=0;
unsigned int maxwavs=0;

unsigned char keys[7]={0};

SystemConf sys;

OggPlayer Music;


DWORD tick=0;

bool TitleScreen=true;


BreakOutMap Map;

DArray<Ball> balls;
Padd padd;

bool fall=false;

int lives=3;
long Score=0;

int oldkey=0;
long mousepowx,mousepowy;
bool ShowDebugText=false;
bool GeneratingTexture=true;
bool GamePaused=false;
int bgpushy=0;
int bgpushx=0;
int scrooltim=0;

bool PaddKilled=false;
float PaddAlpha=1.0f;
bool PaddRevive=false;

EditBox NameBox;

unsigned char LeterKey=0;
unsigned char GlobalKey=0;

DArray<Particle2DSystem> PSystems;

HighScores Scores;

BulletContainer Bullets;
bool ReflectBricks=true;
int NextLevelTimer=0;
 

//=====================================================
void AppendLog(const char * text){
    FILE* logas;
    logas=fopen("logas.txt","at+");
    fputs(text,logas);
    fclose(logas);
}



//==================================================
int FPS (void){
    static int time = 0, FPS = 0, frames = 0, frames0 = 0;
    if ((int)timeGetTime() >= time) {
        FPS = frames-frames0;
        time = timeGetTime()+1000;
        frames0 = frames;
    }
    frames = frames+1;
    return FPS;
}


//-----------------------------------

void DeleteAudio(){
  
    if (maxwavs){
     for (unsigned int i =0;i<maxwavs;i++)
      smotai[i].release();
     delete []smotai;
    }

    if (Music.IsPlaying()){
        Music.Stop();
        Music.Close();
    }

    if (pSound)
        pSound->Release(); 
    
    
    
   
}

//------------------------------
void PlaySong(const char* name){
    if (sys.musicVolume>-10000){

        if (Music.IsPlaying()){
            Music.Stop();
            Music.Close();
        }

        if (Music.OpenOgg(pSound,name)){
            Music.setVolume(sys.musicVolume);
            Music.Play(true);
        }
    }

}


//----------------------------------
HRESULT InitAudio()
{

   //dsoundas
   if (SUCCEEDED(DirectSoundCreate8( NULL, &pSound, NULL ))){
   
       if (SUCCEEDED(pSound->SetCooperativeLevel( hwndMain,  DSSCL_PRIORITY))){
    
    
   //loadinam wavus 

    FILE* list;

    char mas[20][60]={0};
    char buf[50];
    list=fopen("sfx/list.txt","rt");
    
    if (list){
     fscanf(list,"%u",&maxwavs);
     if (maxwavs){
      smotai = new Gabalas[maxwavs];
      fgetc(list);
      char c=0;
      int a;
      for (unsigned int i=0; i<maxwavs; i++){
         
       a=0;
       while((c!='\n')&&(c!=EOF)){
          c=fgetc(list); 
          if ((c!='\n')&&(c!=EOF))
              mas[i][a]=c;
          else 
              mas[i][a]='\0';
          a++;
       }
       sprintf(buf,"sfx/%s",mas[i]);
       smotai[i].open(pSound,buf);
       smotai[i].setVolume(sys.soundFXVolume);
       c=0;
      }
     }
     fclose(list);
    }
    
       }
   }
 
 
   PlaySong("music/music.ogg");
       
  
return 0;
}


//-------------------
void LoadTextures(){
    img.initContainer("pics/list.txt");
    for (int i=0;i<img.list.count();i++)
        img.loadpicture(Renderer.device,i); 
}


//------------------

HRESULT InitD3D(){

    //HRESULT hr;
    Renderer.init();

    Renderer.setmode(hwndMain,sys.width,sys.height,sys.bits,sys.windowed,sys.enableDepthStencil);

    LoadTextures();

    return 0;

}




//-------------------------
void DeleteInput(){
 input.deleteJoystick();
 input.deleteKeyboard();
 input.deleteMouse();
 input.deleteInput();   
}

//----------------------------------------

HRESULT InitInput(){

    HRESULT res;
    if (input.initInput(hinstanceMain)){

        res=input.initKeyboard(hwndMain);
        if (FAILED(res)){ 
            DeleteInput();
            return res;
        }

        
        //sys.joypresent=input.initJoystick(hwndMain);

        if (!input.initMouse(hwndMain)){
            DeleteInput();
            return E_FAIL;
        }




    } else 
        return E_FAIL;

    return S_OK;
}

//------------------------------------

void ReadKeys(){
  
  memset(keys,0,7);
  mousepowx=mousepowy=0l;
  
  input.updateKeyData();
  input.updateMouseData();
  
   if (input.keydata[DIK_UP]& 0x80)    keys[0]=1;
   if (input.keydata[DIK_DOWN]& 0x80)  keys[1]=1;
   if (input.keydata[DIK_LEFT]& 0x80)  keys[3]=1;
   if (input.keydata[DIK_RIGHT]& 0x80) keys[2]=1;
   if ((input.keydata[DIK_LCONTROL]& 0x80)||
       (input.keydata[DIK_RCONTROL]& 0x80)){
           keys[4]=1;
       }
   if (input.keydata[DIK_TAB]& 0x80) {keys[5]=1; }
   if (input.keydata[DIK_SPACE]& 0x80) keys[6]=1; 

   

   if (input.mstate.lX!=0) 
        mousepowx=input.mstate.lX;
  
    if (input.mstate.lY!=0) 
        mousepowy=input.mstate.lY;

    if (input.mstate.rgbButtons[0]) 
     keys[4]=1;
   
   
 /*if (sys.joypresent){

     input.updateJoyData();
  
    if ( input.jstate.lY < 0 ) keys[0]=1;
    if ( input.jstate.lY > 0 ) keys[1]=1;
    if ( input.jstate.lX > 0 ) keys[2]=1;
    if ( input.jstate.lX < 0 ) keys[3]=1;
    
    if(input.jstate.rgbButtons[0]&0x80) {keys[4]=1;}
    if(input.jstate.rgbButtons[1]&0x80) {keys[5]=1;}
    if(input.jstate.rgbButtons[2]&0x80) keys[6]=1;
    
 } */


  

}

//-------------------

void DrawSomeText(){
    
    char buf[80];
    
    
    sprintf(buf,"FPS : %d",FPS());
    WriteText(20,20,Renderer.sprite,&img.images[0],buf);
    sprintf(buf,"Lives : %d",lives);
    WriteText(20,40,Renderer.sprite,&img.images[0],buf);
    sprintf(buf,"Brickcount : %d",Map.brickCount);
    WriteText(20,60,Renderer.sprite,&img.images[0],buf);
       
}
//--------------------------------------
void GenerateTexture(int width, int height){

 PicData picd;
 strcpy(picd.name,"procedural");
 picd.theight=width;
 picd.twidth=height;

 img.list.add(picd);
 Picture pic;
 
 D3DXCreateTexture(Renderer.device,width,height,0,0,D3DFMT_X8R8G8B8,D3DPOOL_MANAGED ,&pic.image);
 pic.info.Height=height;
 pic.info.Width=width;
 pic.tileheight=height;
 pic.tilewidth=width;
 
 
 D3DLOCKED_RECT lr;
 LPDIRECT3DSURFACE9 texsurf;
 pic.image->GetSurfaceLevel(0,&texsurf);
 texsurf->LockRect(&lr,0,0);
 
  DWORD* buffer = (DWORD*)lr.pBits;
 
  int rnd=5;
  for (int a=0;a<height/2+1;a+=4){
    
      if (rnd)
        rnd=5+a*2;
      for (unsigned i=0;i<width/2;i+=4){
          int r=rand()%(rnd+1);
          int g=rand()%(rnd+1);
          int b=rand()%(rnd+1);
          if (rnd)
            rnd+=8;
         
          buffer[a*lr.Pitch/4+i]=RGB(b,g,r);
          buffer[a*lr.Pitch/4+(i+1)]=buffer[a*lr.Pitch/4+i]+RGB(20,20,20);
          buffer[a*lr.Pitch/4+(i+2)]=buffer[a*lr.Pitch/4+i]+RGB(30,30,30);
          buffer[a*lr.Pitch/4+(i+3)]=buffer[a*lr.Pitch/4+i];
      }

      for (unsigned i=width/2;i<width;i++){
          buffer[a*lr.Pitch/4+i]=buffer[a*lr.Pitch/4+(width/2-(i-width/2))-1];
      }

      for (unsigned i=0;i<width;i++){
          buffer[(a+1)*lr.Pitch/4+i]=buffer[a*lr.Pitch/4+i]+RGB(20,20,20);
          buffer[(a+2)*lr.Pitch/4+i]=buffer[a*lr.Pitch/4+i]+RGB(30,30,30);
          buffer[(a+3)*lr.Pitch/4+i]=buffer[a*lr.Pitch/4+i];
      }

  }
  
  for (int a=height/2;a<height;a++){
  
   for (unsigned i=0;i<width;i++){
       buffer[a*lr.Pitch/4+i]=buffer[(height/2-(a+1-height/2))*lr.Pitch/4+i];
   }
   
  }
 texsurf->UnlockRect();
  
 
 
 img.images.add(pic);
}



//------------------------------
void CreatePrize(float x, float y){
    Prizas newprize;
    newprize.pos.x=x*16+16;
    newprize.pos.y=y*16+16;

    newprize.vel.x=0;//(-5+(rand()%7))/(rand()%2+1);
    newprize.vel.y=2.5;

    newprize.type=rand()%8;
    if ((padd.length==1)&&(newprize.type==1))
        newprize.type=0;
    if ((padd.length>5)&&(newprize.type==0))
        newprize.type=1;
    if ((padd.isMagnet)&&(newprize.type==2))
        newprize.type=4;
    if ((padd.canShoot)&&(newprize.type==6))
        newprize.type=7;
    if ((!ReflectBricks)&&(newprize.type==3))
        newprize.type=7;

    Prizai.add(newprize);
}



//-------------------------
void EliminateBrick(int tx, int ty,float speed){

    int brickid=0;
    int x,y;
    if (Map.tiles[ty][tx]%2!=0)
        x=tx*16+16;
    else
        x=tx*16;
    y=ty*16+8;

    if (brickid=Map.removetile(tx,ty)){
        if (brickid==3)
            CreatePrize(tx,ty);
        Score+=5*round(speed/DEFAULT_SPEED);
        if (brickid<5){
            if (!smotai[2].isPlaying())
                smotai[2].play();
            else{
                LPDIRECTSOUNDBUFFER bufas;
                smotai[2].duplicate(pSound,&bufas);
                bufas->Play(0,0,0);;
            }
            Particle2DSystem ps;


            ps.setxy(x,y);
            ps.setDirIntervals(20,20,20,20);
            ps.setParticleLifetime(50);
            ps.setSystemLifetime(50);
            ps.setColors(D3DXCOLOR(1,0,0,1),D3DXCOLOR(1,1,0,0));
            PSystems.add(ps);

        }


    }

}
//--------------------------
void TakeNewLife(){
    Ball newball;
    newball.moving=false;
    newball.angle=0;
    newball.setxy(padd.x,padd.y-16);
    balls.add(newball);
    padd.canShoot=false;
    padd.isMagnet=false;
    ReflectBricks=true;
    lives--;

}

//------------------------------
void KillPadd(){
    PaddKilled=true;
    smotai[1].play();
    Particle2DSystem ps;


    ps.setxy(padd.x,padd.y-4);
    ps.setDirIntervals(padd.length*16,padd.length*16,10,10);
    ps.setParticleLifetime(50);
    ps.setSystemLifetime(100);
    ps.setColors(D3DXCOLOR(1,1,1,1),D3DXCOLOR(1,1,1,0));
    PSystems.add(ps);

}
//---------------------------------
void ResetGame(){
    Score=0;
    Scores.destroy();
        Bullets.destroy();
        Prizai.destroy();
        Scores.load("scores.dat",10);
        lives=3;
        padd.length=1;
        balls.destroy();
        Ball newball;
        newball.setxy(padd.x,padd.y-16);
        newball.setangle(0);
        newball.moving=false;
        balls.add(newball);
        Map.destroy();
        Map.brickCount=0;
        Map.create(40,30);
        Map.generate();
        padd.canShoot=false;
        padd.isMagnet=false;
        ReflectBricks=true;
        //---------
        GeneratingTexture=true;
        img.images.remove(img.count()-1);
        img.list.remove(img.count()-1);
        GenerateTexture(64,64);
        GeneratingTexture=false;
        if (Score>Scores.getScore(9).score)
            NameBox.activate();
        TitleScreen=true;
        PlaySong("music/music.ogg");
        
}

//---------------------------------
void GoNextLevel(){
    balls.destroy();
    Ball newball;
    newball.angle=0;
    newball.setxy(padd.x,padd.y-16);
    newball.moving=false;
    balls.add(newball);
    GeneratingTexture=true;
    img.images.remove(img.count()-1);
    img.list.remove(img.count()-1);
    GenerateTexture(64,64);
    GeneratingTexture=false;
    Map.generate();
    padd.canShoot=false;
    padd.isMagnet=false;
    ReflectBricks=true;
    Bullets.destroy();
    Prizai.destroy();
    padd.length=1;


}


//-----------------------------

void GameLoop(){

    if (Music.IsPlaying())
        Music.Update();


    if (TitleScreen){ //vardo ivedimas

        if (NameBox.active()){
            if (!NameBox.entered){
                NameBox.getInput(LeterKey,GlobalKey);
            }
            else{

                Scores.addScore(NameBox.text,Score,10);
                Scores.write("scores.dat",10);
                NameBox.deactivate();
                NameBox.reset();
                Score=0;
            }
        }


        if ((scrooltim>=0)&&(scrooltim<5)){
            bgpushy++;
            if (bgpushy>=64){
                bgpushy=0;
                scrooltim++;
            }
        }
        else{
            bgpushx++;
            if (bgpushx>=64){
                bgpushx=0;
                ++scrooltim;
                if (scrooltim==10)
                    scrooltim=0;
            }
        }

        if ((oldkey==1)&&(!keys[4])&&(!NameBox.active())){
            TitleScreen=false;
            PlaySong("music/musicingame.ogg");
            bgpushy=0;
            bgpushx=0;
        }

        if (keys[4])
            oldkey=1;
        else
            oldkey=0;

    }

    else{ //GAME------------->


        if (!GamePaused){


            

                padd.tic++;
                if (padd.tic>50)
                    padd.tic=0;

                for (unsigned int i=0;i<PSystems.count();i++){
                    PSystems[i].updateSystem();

                    if (PSystems[i].isDead()){
                        PSystems[i].destroy();
                        PSystems.remove(i);
                    }

                }   
                //--Judinam pada
                if ((padd.x+padd.length*16+16+mousepowx<640)&&(padd.x-16-padd.length*16+mousepowx>0)&&((!PaddKilled)||(PaddRevive))&&(!NextLevelTimer)){
                    unsigned int i = 0;
                    if (balls.count()){
                        while ((!balls[i].colidepadd(balls[i].speed,0,padd.x,padd.y,padd.length)&&(i<balls.count())))
                            i++;
                        if ((i>=0)&&(i<balls.count())){
                            if ((balls[i].x>8)&&(balls[i].x<632))
                                padd.move(mousepowx,0);
                        }
                        else
                            padd.move(mousepowx,0);

                        for (unsigned int i=0;i<balls.count();i++){
                            if (!balls[i].moving)
                                balls[i].x+=mousepowx;
                        }   
                    }
                }
                //--------
                if (PaddKilled){
                    if (!PaddRevive){
                        PaddAlpha-=0.01f;
                        if ((PaddAlpha<0)&&(lives))
                            PaddRevive=true;
                        if ((!lives)&&(PaddAlpha<0)){
                            PaddKilled=false;
                            lives--;
                            PaddAlpha=1;
                        }
                    }
                    else{
                        PaddAlpha+=0.01f;
                    
                        if (PaddAlpha>1){
                            TakeNewLife();
                            PaddKilled=false;
                            PaddRevive=false;
                            PaddAlpha=1;
                        }
                    }

                }


                //------------------
                Bullets.iteratorReset();
                Bullet* b=0;
                b=Bullets.getCurrent();
                bool isNext=false;
                if (b)
                    isNext=true;
                while (isNext){
                    b->y-=4;
                    if ((b->y<4)||(Map.tiles[b->y/16][b->x/16])){
                        if (Map.tiles[b->y/16][b->x/16])
                            EliminateBrick(b->x/16,b->y/16,DEFAULT_SPEED*2.0f);
                        Bullets.removeCurrent();

                    }
                    else
                        isNext=Bullets.iteratorGoNext();
                    b=Bullets.getCurrent();
                    if (!b)
                        isNext=false;
                }



                //--------------
                if ((keys[4])&&(!NextLevelTimer)){

                    if ((padd.canShoot)&&(!PaddKilled)){

                        if (padd.reloadtic==0){
                            if (!smotai[3].isPlaying())
                                smotai[3].play();
                            else{
                                LPDIRECTSOUNDBUFFER bufas;
                                smotai[3].duplicate(pSound,&bufas);
                                bufas->Play(0,0,0);;
                            }
                            Bullets.add(padd.x+padd.length*16+12,padd.y-20);
                            Bullets.add(padd.x-padd.length*16-12,padd.y-20);
                            padd.reloadtic=1;
                        }
                    }
                    if (padd.reloadtic>=1){
                        padd.reloadtic++;
                        if (padd.reloadtic>10)
                            padd.reloadtic=0;

                    }

                    for (unsigned int i=0;i<balls.count();i++){
                        if (!balls[i].moving){
                            balls[i].moving=true;
                            balls[i].speed=DEFAULT_SPEED;
                            balls[i].angle=3.14f/2.0f;

                        }

                    }
                }




                if (!NextLevelTimer){
                for (unsigned int i=0;i<balls.count();i++){

                    if (balls[i].moving){

                        if (balls[i].speed<MAX_SPEED)
                        balls[i].speed+=0.001f;

                        if (!balls[i].colidepadd(balls[i].speed,0,padd.x,padd.y,padd.length)){
                            ColidedBrick kalad[3];
                            if (balls[i].move(balls[i].speed,0,&Map, kalad,ReflectBricks)){
                                if (!smotai[5].isPlaying())
                                    smotai[5].play();
                                else{
                                    LPDIRECTSOUNDBUFFER bufas;
                                    smotai[5].duplicate(pSound,&bufas);
                                    bufas->Play(0,0,0);;
                                }

                            }


                            //Naikinam kaladeles----
                            int brickid=0;
                            if (kalad[0].colided){

                                if ((kalad[1].x==kalad[0].x)&&(kalad[1].y==kalad[0].y))
                                    kalad[1].colided=false;
                                else
                                    if (((kalad[1].x==kalad[0].x+1)&&(kalad[1].y==kalad[0].y)&&(Map.tiles[kalad[0].y][kalad[0].x]%2==0))||
                                        ((kalad[1].x==kalad[0].x-1)&&(kalad[1].y==kalad[0].y)&&(Map.tiles[kalad[0].y][kalad[0].x]%2!=0))){
                                            kalad[1].colided=false;
                                    }

                                    if ((kalad[2].x==kalad[0].x)&&(kalad[2].y==kalad[0].y))
                                        kalad[2].colided=false;
                                    else
                                        if (((kalad[2].x==kalad[0].x+1)&&(kalad[2].y==kalad[0].y)&&(Map.tiles[kalad[0].y][kalad[0].x]%2==0))||
                                            ((kalad[2].x==kalad[0].x-1)&&(kalad[2].y==kalad[0].y)&&(Map.tiles[kalad[0].y][kalad[0].x]%2!=0))){
                                                kalad[2].colided=false;
                                        }

                                        EliminateBrick(kalad[0].x,kalad[0].y,balls[i].speed);


                            }
                            //**
                            if (kalad[1].colided){

                                if ((kalad[2].x==kalad[1].x)&&(kalad[2].y==kalad[1].y))
                                    kalad[2].colided=false;
                                else
                                    if (((kalad[2].x==kalad[1].x+1)&&(kalad[2].y==kalad[1].y)&&(Map.tiles[kalad[1].y][kalad[1].x]%2==0))||
                                        ((kalad[2].x==kalad[1].x-1)&&(kalad[2].y==kalad[1].y)&&(Map.tiles[kalad[1].y][kalad[1].x]%2!=0))){
                                            kalad[2].colided=false;
                                    }

                                    EliminateBrick(kalad[1].x,kalad[1].y,balls[i].speed);

                            }
                            //**
                            if (kalad[2].colided){

                                EliminateBrick(kalad[2].x,kalad[2].y,balls[i].speed);
                            }
                            //------------------------


                        } else{ //jei atsitrenkia i pada;


                            if ((padd.isMagnet)&&
                                (balls[i].x+8.0f<=padd.x+((padd.length+1)*16-8))&&
                                (balls[i].x-8.0f>=padd.x-((padd.length+1)*16-8))){
                                    balls[i].angle=0;
                                    balls[i].moving=false;
                            }
                            else{
                                if (!smotai[0].isPlaying())
                                    smotai[0].play();
                                else{
                                    LPDIRECTSOUNDBUFFER bufas;
                                    smotai[0].duplicate(pSound,&bufas);
                                    bufas->Play(0,0,0);;
                                }
                                float kprc=((3.14f/8.0f)*2.0f)/100.0f;
                                float iprc=((padd.length+1)*16-8)/100.0f;
                                balls[i].angle=(3.14f/2.0f)-((balls[i].x-padd.x)/iprc)*kprc;
                                if ((balls[i].x+mousepowx>8)&&(balls[i].x+mousepowx<632))
                                    balls[i].x+=mousepowx;
                            }




                        }



                        if (balls[i].y>padd.y+8){
                            balls.remove(i);

                            if (balls.count()<1)
                                KillPadd();
                        }

                    }
                }
                }




                //------

                if ((Map.brickCount<1)&&(NextLevelTimer==0)){
                    NextLevelTimer=1;
                }

                //-----

                if (!PaddKilled){
                    for (unsigned i=0;i<Prizai.count();i++){
                    /*if (Prizai[i].pos.x>640){
                        //D3DXVECTOR2 vec
                        //D3DXVec2Normalize(&vec,&vec);
                        Prizai[i].vel=Vec2Reflection(Prizai[i].vel,D3DXVECTOR2(0,-1));
                    }
                    if (Prizai[i].pos.x<0){
                        Prizai[i].vel=Vec2Reflection(Prizai[i].vel,D3DXVECTOR2(0,1));
                    }*/
                    Prizai[i].pos+=Prizai[i].vel;

                    if ((round(Prizai[i].pos.x)+16>padd.x-padd.length*16-16)&&(round(Prizai[i].pos.x)-16<padd.x+padd.length*16+16)){
                        if ((padd.y-8<round(Prizai[i].pos.y)+16)){
                            if (!smotai[4].isPlaying())
                                smotai[4].play();
                            else{
                                LPDIRECTSOUNDBUFFER bufas;
                                smotai[4].duplicate(pSound,&bufas);
                                bufas->Play(0,0,0);;
                            }
                            switch(Prizai[i].type){
                    case 0: padd.length++; break; //padd ++
                    case 1:
                        if (padd.length>1) //padd --
                            padd.length--;
                        break;
                    case 2:{
                        padd.isMagnet=true;
                           } break;
                    case 3:{
                        ReflectBricks=false;
                           } break;
                    case 4:{ //dauginasi kamuoliukai
                        if (balls.count()){
                            unsigned cnt=balls.count();
                            for (unsigned int i=0;i<cnt;i++){
                                if (!balls[i].moving){
                                    balls[i].moving=true;
                                    balls[i].speed=DEFAULT_SPEED;
                                    balls[i].angle=3.14f/2.0f;
                                }
                                Ball newball;
                                newball.setxy(balls[i].x,balls[i].y);
                                newball.moving=true;
                                newball.speed=balls[i].speed;
                                newball.angle=-balls[i].angle;
                                balls.add(newball);
                            }
                        }
                           }break;

                    case 5:{ //slow balls :)
                        for (unsigned int i=0; i<balls.count(); i++)
                            balls[i].speed=DEFAULT_SPEED/2.0f;
                           } break;

                    case 6:{
                        padd.canShoot=true;
                           }break;
                    case 7:{ //death
                        balls.destroy();
                        KillPadd();
                           }break;
                            }
                            Prizai.remove(i);
                        }

                    }
                    else
                        if (Prizai[i].pos.y-16>480)
                            Prizai.remove(i);


                }

                if (lives<0){
                    ResetGame();
                }
            

                if (NextLevelTimer>0){
                    NextLevelTimer++;
                    if (NextLevelTimer>100){
                        NextLevelTimer=0;
                        GoNextLevel();
                    }
                }
        }
}
    }


}
//------------------------------------
void DisplayLives(){

    Padd padas;
    padas.length=0;
    padas.y=10;
    for (int i=0;i<lives;i++){
        padas.x=300+i*32;
        padas.draw(img.images[1],Renderer.sprite);
    }
}


//---------------------------

void RenderScreen(){

    if (Renderer.device){

        if (SUCCEEDED(Renderer.device->BeginScene())){  

            Renderer.device->Clear( 0, 0, D3DCLEAR_TARGET,D3DXCOLOR(0,0,0,0), 0, 0 );

            if (Renderer.sprite){
                Renderer.sprite->Begin(D3DXSPRITE_ALPHABLEND);

                if (!GeneratingTexture){
                    for (int i=0;i<10;i++){
                        for (int a=0; a<12; a++)
                            img.images[img.images.count()-1].Blt(Renderer.sprite,(a-2)*64+bgpushx,(i-2)*64+bgpushy,0,(100-NextLevelTimer)/100.0f);
                    }
                        
                    if (TitleScreen){
                        Scores.display(Renderer.sprite,&img.images[0],10,0,0);
                    }

                    if (NameBox.active()){
                        
                        NameBox.draw(Renderer.device,Renderer.sprite,&img.images[0]);
                    }
                    if (!TitleScreen){ //in game

                        
                        Map.draw(img.images[3],Renderer.sprite,0,0);
                        for (unsigned int i=0;i<Prizai.count();i++){
                            img.images[4].Blt(Renderer.sprite,round(Prizai[i].pos.x),round(Prizai[i].pos.y),Prizai[i].type,1,1,1,0,1,1,1,true);
                        }
                        padd.draw(img.images[1],Renderer.sprite,PaddAlpha);
                        for (unsigned i=0;i<balls.count();i++)
                            balls[i].draw(img.images[2],Renderer.sprite);
                        for (unsigned i=0;i<PSystems.count();i++)
                            PSystems[i].drawParticles(Renderer.sprite,&img.images[6]);

                        Bullets.draw(&img.images[6],Renderer.sprite);

                        DisplayLives();
                        DrawNumber(0,0,Score,Renderer.sprite,&img.images[5]);
                        if (GamePaused)
                            WriteText(150,220,Renderer.sprite,&img.images[0],"GAME PAUSED (press P to unpause)",0.8f,0.8f,1.0f);

                    }
                    else {
                        img.images[7].Blt(Renderer.sprite,200,200,0);
                        
                        WriteText(150,360,Renderer.sprite,&img.images[0],"push left mouse button to play...",0.8f,0.8f,1.0f);
                    }
                }
                else
                    WriteText(200,150,Renderer.sprite,&img.images[0],"processing...");


                if (ShowDebugText)
                    DrawSomeText();  

            }

            Renderer.sprite->End();
        }

        /*line->Begin();
        D3DXVECTOR2 vec[2];
        vec[0].x=ball.x;
        vec[0].y=ball.y;
        vec[1]= MakeVector(100.0f,0,ball.angle);
        vec[1].x=ball.x+vec[1].x;
        vec[1].y=ball.y-vec[1].y;
        line->Draw(vec,2,0xffffffff);

        //statmenas
        vec[1]= MakeVector(100.0f,0,ball.angle);
        float tmp=vec[1].x;
        vec[1].x=-vec[1].y;
        vec[1].y=tmp;
        vec[1].x=ball.x+vec[1].x;
        vec[1].y=ball.y-vec[1].y;
        line->Draw(vec,2,0xff0000ff);

        //vp (vq•vq) - 2*vq (vq•vp)    vp - vektorius, vq - plokstumos normale
        //kazkoks tai vektorius vp
        D3DXVECTOR2 vp;
        vp.x=100;
        vp.y=100;
        vec[1]=vp;
        vec[1].x=ball.x+vec[1].x;
        vec[1].y=ball.y-vec[1].y;
        line->Draw(vec,2,0xffff00ff);

        //atspindis
        D3DXVECTOR2 vq= MakeVector(100.0f,0,ball.angle);//plokstumos vektorius
        vec[1]=vp*D3DXVec2Dot(&vq,&vq)-2*vq*D3DXVec2Dot(&vq,&vp);
        D3DXVec2Normalize(&vec[1],&vec[1]);
        vec[1]*=100;
        vec[1].x=ball.x+vec[1].x;
        vec[1].y=ball.y-vec[1].y;
        line->Draw(vec,2,0xffff0000);
        //--


        line->End();*/

        Renderer.device->EndScene();

        Renderer.device->Present( NULL, NULL, NULL, NULL ); 


    }


}


//-----------------------------------
void DeletePics(){
    
    img.deleteContainer();  
}

//------------------------------------------


void DeleteD3D(){

    Renderer.destroy();
   
    
}
//------------------------------------

HRESULT FullScreenSwitch(){
    
    HRESULT res;

    if (sys.windowed){   
        AppendLog("Going fullscreen...\n");
        sys.windowed=false;
        
        if (FAILED(res=Renderer.reset(hwndMain,sys.width,sys.height,sys.bits,sys.windowed)))
            return res;
        
        AppendLog("Ok.\n");
    }//is windowed
    else {
        AppendLog("Going windowed...\n");
        sys.windowed=true;   
        if (FAILED(res=Renderer.reset(hwndMain,sys.width,sys.height,sys.bits,sys.windowed)))
            return res;
        AppendLog("Ok.\n");
        SetWindowPos(hwndMain,0,100,100,sys.width+4,sys.height+25,0);

    }

    return S_OK;
}


//------------------------------------

void QuitApp(){
 DeleteAudio();
 DeleteInput();
 DeletePics();
 DeleteD3D();
 PostQuitMessage(0);
}

//----------------------------------
void ClipMouse(){
    RECT rcClip;
    GetWindowRect(hwndMain, &rcClip);  
    rcClip.top+=30;
    ClipCursor(&rcClip); 
}

//-----------------------------------

LRESULT CALLBACK MainWinProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
    LeterKey=0;
    //SysKeyPressed=false;
    switch(msg){

  case WM_DESTROY:{
      QuitApp(); 
      break;
                  }

  case WM_CHAR:{

      LeterKey = (int)wparam;

               }break;

  case WM_ACTIVATE:{

      if (wparam==WA_INACTIVE){
          ClipCursor(0);
      }
      else{
          ClipMouse();
      }

      if( WA_INACTIVE != wparam && input.pKeyboard ){
          if (input.pKeyboard)
              input.pKeyboard->Acquire();
          if (input.pMouse)
              input.pMouse->Acquire();      
      }
                   }
                   break;



  case WM_SYSKEYUP:{
      if (wparam==VK_RETURN){
          if (FAILED(FullScreenSwitch())){
              sys.shutdown=true;
          }
          if (sys.windowed)
               ClipMouse();
          
      }
                   }break;

    
  case WM_KEYUP:{
      GlobalKey=0;
     
        switch (wparam){
            case VK_RETURN:{
                            GlobalKey=13;
                            
                           } break;
            case VK_F1: ShowDebugText=!ShowDebugText; break;
            case VK_F8: Renderer.writeScreenshot("screenshot.bmp"); break;
            case 8 : GlobalKey=8; break;
            case 'P': {
                if (!TitleScreen){
                        GamePaused=!GamePaused; 
                        if (sys.windowed){
                            if (GamePaused){  
                                ShowCursor(TRUE);
                                ClipCursor(0);
                            }
                            else{
                                ShowCursor(FALSE);
                                ClipMouse();
                            }   
                        }
                }
                        break;
                      }
            case VK_ESCAPE:  {
                if (!NameBox.active()){
                    if (TitleScreen)
                        sys.shutdown=true; 
                    else{
                        ResetGame();
                    }
                }
                else
                    GlobalKey=27;
            }
        }


      break;
        }

    }

    return DefWindowProc(hwnd, msg,wparam, lparam);
}


//-----------------------------------------

int WINAPI WinMain( HINSTANCE hinstance,
                   HINSTANCE hprevinstance,
                   LPSTR lpcmdline,
                   int ncmdshow)
{


    MSG msg;


    windowsclass.cbSize = sizeof(WNDCLASSEX);
    windowsclass.style = 0;
    windowsclass.lpfnWndProc = MainWinProc;
    windowsclass.cbClsExtra = 0;
    windowsclass.cbWndExtra = 0;
    windowsclass.hInstance = hinstance;
    windowsclass.hIcon = LoadIcon(hinstance,MAKEINTRESOURCE(101));
    windowsclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    windowsclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowsclass.lpszMenuName = 0;
    windowsclass.lpszClassName = "WINDOWSCLASS";
    windowsclass.hIconSm = LoadIcon(hinstance,MAKEINTRESOURCE(101));
    hinstanceMain=hinstance;




    if (!RegisterClassEx(&windowsclass))
        return(0);
    if (!(hwndMain = CreateWindowEx(
        NULL, "WINDOWSCLASS", "Breakout",
        WS_MINIMIZEBOX|WS_SYSMENU|WS_VISIBLE|WS_CAPTION,100,100,644,505,0,0,hinstance,0)))
        return(0);

    srand((unsigned int)time(0));

    FILE* ft=fopen("logas.txt","wt+");
    fclose(ft);
    AppendLog("======================Loging started\n");
    AppendLog("Loading config...\n");

    sys.load("config.cfg",false);
    sys.width=640; //overide :)
    sys.height=480;

    AppendLog("config loaded...\n");
    char buf[255];
    sprintf(buf,"screen: %d x %d @ %d\n",sys.width,sys.height,sys.bits);
    AppendLog(buf);

    AppendLog("generating map...\n");
    Map.create(40,30);
    Map.generate();


    Scores.load("scores.dat",10);
    
    NameBox.init(250,150,"Enter your name:",10);


    Ball newball;
    newball.setxy(20*16,30*16-24);
    newball.setangle(0);
    newball.moving=false;
    balls.add(newball);

    padd.length=1;

    padd.setxy(20*16,30*16-8);

    AppendLog("Initializing D3D...\n");

    if (SUCCEEDED(InitD3D())){

        AppendLog("D3D initialized OK!\n");
        AppendLog("Initializing input..\n");
        if (SUCCEEDED(InitInput())){
            AppendLog("Input initialized OK!\n");
            AppendLog("Initializing Audio System..\n");
            InitAudio();
            AppendLog("Generating background texture..\n");
            GenerateTexture(64,64);
            GeneratingTexture=false;
            AppendLog("Locking mouse..\n");
            ClipMouse();
            ShowCursor(FALSE);
            AppendLog("Displaying window..\n");
            ShowWindow(hwndMain, SW_SHOWDEFAULT);
            UpdateWindow(hwndMain);
            AppendLog("Starting program loop\n");

            LONGLONG cur_time;       // current time
            DWORD  time_count=8;    // ms per frame, default if no performance counter
            LONGLONG perf_cnt;       // performance timer frequency
            BOOL perf_flag=FALSE;    // flag determining which timer to use
            LONGLONG next_time=0;    // time to render next frame
            BOOL move_flag=TRUE;     // flag noting if we have moved yet

            if (QueryPerformanceFrequency((LARGE_INTEGER *) &perf_cnt)) {
                perf_flag=TRUE;
                time_count=(DWORD)perf_cnt/70;        
                QueryPerformanceCounter((LARGE_INTEGER *) &next_time);
            } 
            else {
                next_time=timeGetTime();
            }

            ZeroMemory(&msg, sizeof(msg));
            while(msg.message != WM_QUIT){
                if (PeekMessage(&msg,0,0,0,PM_REMOVE)){
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else{


                    if (move_flag) {
                        GameLoop();
                        ReadKeys();
                        move_flag=FALSE;
                    }

                    if (perf_flag)
                        QueryPerformanceCounter((LARGE_INTEGER *) &cur_time);
                    else
                        cur_time=timeGetTime();

                    if (cur_time>next_time){
                        RenderScreen(); 
                        next_time += time_count;

                        if (next_time < cur_time)
                            next_time = cur_time + time_count;

                        move_flag=TRUE;
                    }

                    if (sys.shutdown)
                        QuitApp();   

                    if (sys.priority)
                        Sleep(sys.priority);


                }        
            }
        }
    }

    Prizai.destroy();
    for (unsigned int i=0;i<PSystems.count();i++)
        PSystems[i].destroy();
    PSystems.destroy();

    balls.destroy();

    Map.destroy();

    Scores.destroy();
    Bullets.destroy();



#ifdef _DEBUG
    _CrtDumpMemoryLeaks();
#endif

    return 0;



}
