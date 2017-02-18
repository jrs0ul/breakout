
#include "CppSingleton.h"
#include <ctime>







//--------------------------------------
void Singleton::loadConfig(){
#ifndef __ANDROID__
    char buf[255];
    sprintf(buf, "%s/settings.cfg", DocumentPath);
    sys.load(buf);
    ScreenWidth = sys.ScreenWidth;
    ScreenHeight = sys.ScreenHeight;
    windowed = sys.useWindowed;

    sys.write(buf);
#endif

}
//----------------------------------------
void Singleton::init(){
        
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


#ifndef __ANDROID__
    ss.init(0);
    ss.loadFiles("sfx/", "list.txt");
    music.open("music/music.ogg");
    music.setVolume(sys.musicVolume);
    music.playback();
#endif

#ifndef __ANDROID__
    pics.load("pics/list.txt");
#else
    pics.loadFile("pics/chars.tga",   0, 16, 16, 0, AssetManager);
    pics.loadFile("pics/padd.tga",    1, 16, 16, 0, AssetManager);
    pics.loadFile("pics/ball.tga",    2, 16, 16, 0, AssetManager);
    pics.loadFile("pics/bricks.tga",  3, 16, 16, 0, AssetManager);
    pics.loadFile("pics/prize.tga",   4, 32, 32, 1, AssetManager);
    pics.loadFile("pics/digits.tga",  5, 16, 32, 0, AssetManager);
    pics.loadFile("pics/kakutis.tga", 6, 8,  8,  0, AssetManager);
    pics.loadFile("pics/title.tga",   7, 256,128,0, AssetManager);
#endif

    useAccel = false;

    srand((unsigned int)time(0));




    Map.create(40,30);
    Map.generate();

    //TODO: let's fix this
    //Scores.load("scores.dat",10);
    
    char letters[] = "1234567890QWERTYUIOPASDFGHJKL:ZXCVBNM@$&*-/\\!#?_=() .,";
    NameBox.setChars((const unsigned char*)letters);
    NameBox.setCharWidth(40);
    NameBox.setRowLen(10);
    NameBox.setpos(30, 60);



    Ball newball;
    newball.setxy(20*16,30*16-24);
    newball.setangle(0);
    newball.moving=false;
    balls.add(newball);

    padd.length=1;

    padd.setxy(20*16,30*16-8);


    GenerateTexture(64,64);
    GeneratingTexture=false;


}
//-----------------------

void Singleton::GenerateTexture(unsigned width, unsigned height){

    Image pic;
    pic.width = width;
    pic.height = height;
    pic.bits = 24;

    printf("Texture w:%u h:%u\n", width, height);
    pic.data = (unsigned char*)malloc(width * height * (pic.bits / 8));
 
    int rnd=5;
    
    for (unsigned a = 0; a < height / 2; a += 4){
    
        if (rnd)
            rnd = 5 + a * 2;

        for (unsigned i=0;i < width / 2; i += 4){
            unsigned char r = rand()%(rnd+1);
            unsigned char g = rand()%(rnd+1);
            unsigned char b = rand()%(rnd+1);
            if (rnd)
                rnd+=8;
            //first pixel
            pic.data[(a * width * 3) + i*3] = b;
            pic.data[(a * width * 3) + i*3 + 1] = g;
            pic.data[(a * width * 3) + i*3 + 2] = r;
            //second
            pic.data[(a * width * 3) + i*3+3] = (unsigned char)(pic.data[(a * width *3) + i*3] + 20);
            pic.data[(a * width * 3) + i*3+4] = (unsigned char)(pic.data[(a * width *3) + i*3+1] + 20);
            pic.data[(a * width * 3) + i*3+5] = (unsigned char)(pic.data[(a * width *3) + i*3+2] + 20);
            //third    
            pic.data[(a*width*3) + (i*3+6)] = pic.data[a*width*3+i*3]+30;
            pic.data[(a*width*3) + (i*3+7)] = pic.data[a*width*3+i*3+1]+30;
            pic.data[(a*width*3) + (i*3+8)] = pic.data[a*width*3+i*3+2]+30;
            //fourth
            pic.data[a*width*3 + (i*3+9)] = pic.data[a*width*3+i*3];
            pic.data[a*width*3 + (i*3+10)] = pic.data[a*width*3+i*3+1];
            pic.data[a*width*3 + (i*3+11)] = pic.data[a*width*3+i*3+2];
        }

        for (unsigned i = width / 2; i < width; i++){
            pic.data[a*width*3 + i*3] =   pic.data[a*width*3 + ((width/2-(i-width/2))-1)*3];
            pic.data[a*width*3 + i*3+1] = pic.data[a*width*3 + ((width/2-(i-width/2))-1)*3+1];
            pic.data[a*width*3 + i*3+2] = pic.data[a*width*3 + ((width/2-(i-width/2))-1)*3+2];
        }

      for (unsigned i = 0; i < width; i++){
          //first
          pic.data[(a+1)*width*3+i*3] =   (unsigned char)(pic.data[a*width*3+i*3] + 20);
          pic.data[(a+1)*width*3+i*3+1] = (unsigned char)(pic.data[a*width*3+i*3+1] + 20);
          pic.data[(a+1)*width*3+i*3+2] = (unsigned char)(pic.data[a*width*3+i*3+2] + 20);
          //second
          pic.data[(a + 2) * width*3+i*3]   = (unsigned char)(pic.data[a*width*3 + i*3] + 30);
          pic.data[(a + 2) * width*3+i*3+1] = (unsigned char)(pic.data[a*width*3 + i*3+1] + 30);
          pic.data[(a + 2) * width*3+i*3+2] = (unsigned char)(pic.data[a*width*3 + i*3+2] + 30);
          //third
          pic.data[(a+3)*width*3+i*3] = pic.data[a*width*3 + i*3];
          pic.data[(a+3)*width*3+i*3+1] = pic.data[a*width*3 + i*3+1];
          pic.data[(a+3)*width*3+i*3+2] = pic.data[a*width*3 + i*3 +2];
      }

  }
  
        for (unsigned a = height / 2; a < height; a++){
  
            for (unsigned i=0;i<width;i++){
                pic.data[a*width*3 + i*3] = pic.data[(height/2-(a+1-height/2))*width*3+i*3];
                pic.data[a*width*3 + i*3+1] = pic.data[(height/2-(a+1-height/2))*width*3+i*3+1];
                pic.data[a*width*3 + i*3+2] = pic.data[(height/2-(a+1-height/2))*width*3+i*3+2];
            }
   
        }
        pics.makeTexture(pic, "procedural",pics.count(), width, height);
    

}

//-------------------
void Singleton::render(){
    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glRotatef(-90, 0, 0, 1);
        glOrthof(0.0, (GLfloat) SCREEN_WIDTH, (GLfloat) SCREEN_HEIGHT, 0.0, 400, -400);
    #elif __ANDROID__
        glOrthof(0.0, (GLfloat) SCREEN_WIDTH, (GLfloat) SCREEN_HEIGHT, 0.0, 400, -400);
    #else
        glOrtho(0.0, (GLfloat) SCREEN_WIDTH, (GLfloat) SCREEN_HEIGHT, 0.0, 400, -400);
    #endif
    

    RenderScreen();


    glEnable(GL_TEXTURE_2D);
    pics.drawBatch(666);   
}

//---------------------
void Singleton::logic(){
#ifndef __ANDROID__
    if (music.playing())
        music.update();
#endif
    
    if (touches.down.count()){
        PadX = touches.down[0].v[0] - padd.x;
    }
    if (touches.move.count()){
        PadX = touches.move[0].v[0] - padd.x;
    }
    if (touches.up.count()){
        PadX = 0;
    }
   
    GameLoop();
    
    touches.oldDown.destroy();
    for (unsigned long i = 0; i < touches.down.count(); i++ ){
        Vector3D v = touches.down[i];
        touches.oldDown.add(v);
    }
    touches.up.destroy();
    touches.down.destroy();
    touches.move.destroy();
}
//-------------------------
void Singleton::destroy(){
    puts("Bye");

    Prizai.destroy();
    for (unsigned int i=0;i<PSystems.count();i++)
        PSystems[i].destroy();
    PSystems.destroy();

    balls.destroy();

    Map.destroy();

    Scores.destroy();
    Bullets.destroy();


#ifndef __ANDROID__
    music.release();
    ss.freeData();
    ss.exit();
#endif
    pics.destroy();
}

//==================================================
//TODO: fix time(0) 
int FPS (void){
    static int ctime = 0, FPS = 0, frames = 0, frames0 = 0;
    if ((int)time(0) >= ctime) {
        FPS = frames-frames0;
        ctime = time(0)+1000;
        frames0 = frames;
    }
    frames = frames+1;
    return FPS;
}

//------------------------------
void Singleton::CreatePrize(float x, float y){
    Prizas newprize;
    newprize.pos.set(x * 16 + 16, y * 16 + 16, 0);

    newprize.vel.set(0, //(-5+(rand()%7))/(rand()%2+1);
                     2.5, 0);

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
void Singleton::EliminateBrick(int tx, int ty,float speed){

    int brickid = 0;
    int x,y;
    if (Map.tiles[ty][tx]%2!=0)
        x=tx*16+16;
    else
        x=tx*16;
    y=ty*16+8;

    brickid = Map.removetile(tx,ty);
    if (brickid){
        if (brickid==3)
            CreatePrize(tx,ty);
        Score+=5*round(speed/DEFAULT_SPEED);
        if (brickid<5){
#ifndef __ANDROID__
            ss.playsound(2);
#endif
            
            Particle2DSystem ps;


            ps.setPos(x, y, 0);
            ps.setDirIntervals(Vector3D(1,1,1) , 20);
            ps.setParticleLifetime(50);
            ps.setSystemLifetime(50);
            ps.setColors(COLOR(1,0,0,1),COLOR(1,1,0,0));
            PSystems.add(ps);

        }


    }

}
//--------------------------
void Singleton::TakeNewLife(){
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
//---------------------
void Singleton::KillPadd(){
    PaddKilled=true;
#ifndef __ANDROID__
    ss.playsound(1);
#endif
    Particle2DSystem ps;


    ps.setPos(padd.x,padd.y-4, 0);
    ps.setDirIntervals(Vector3D(1,1,0),360);
    ps.setParticleLifetime(50);
    ps.setSystemLifetime(100);
    ps.setColors(COLOR(1,1,1,1),COLOR(1,1,1,0));
    PSystems.add(ps);

}
//---------------------------------
void Singleton::ResetGame(){
    Score = 0;
    Scores.destroy();
    Bullets.destroy();
    Prizai.destroy();
    //TODO fix it
    //Scores.load("scores.dat",10);
    lives = 3;
    padd.length=1;
    balls.destroy();
    Ball newball;
    newball.setxy(padd.x,padd.y-16);
    newball.setangle(0);
    newball.moving = false;
    balls.add(newball);
    Map.destroy();
    Map.brickCount = 0;
    Map.create(40, 30);
        Map.generate();
        padd.canShoot=false;
        padd.isMagnet=false;
        ReflectBricks=true;
        //---------
        GeneratingTexture=true;
        pics.remove(pics.count()-1);
        GenerateTexture(64,64);
        GeneratingTexture=false;

        if (Score > Scores.getScore(9).score)
            NameBox.activate();
        TitleScreen = true;
#ifndef __ANDROID__
        music.stop();
        music.open("music/music.ogg");
        music.playback();
#endif
        
}
//---------------------------------
void Singleton::GoNextLevel(){

    balls.destroy();
    Ball newball;
    newball.angle = 0;
    newball.setxy(padd.x, padd.y - 16);
    newball.moving = false;
    balls.add(newball);
    GeneratingTexture = true;
    pics.remove(pics.count() - 1);
    GenerateTexture(64, 64);
    GeneratingTexture = false;
    Map.generate();
    padd.canShoot = false;
    padd.isMagnet = false;
    ReflectBricks = true;
    Bullets.destroy();
    Prizai.destroy();
    padd.length = 1;


}

//---------------------------
void Singleton::bulletLogic(){

    Bullets.iteratorReset();
    Bullet* b = 0;
    b = Bullets.getCurrent();
    bool isNext = false;
    if (b)
        isNext = true;
    while (isNext){
        b->y-=4;
        if ((b->y<4)||(Map.tiles[b->y/16][b->x/16])){
            if (Map.tiles[b->y/16][b->x/16])
            EliminateBrick(b->x/16,b->y/16,DEFAULT_SPEED*2.0f);
            Bullets.removeCurrent();
        }
        else
            isNext = Bullets.iteratorGoNext();
        b = Bullets.getCurrent();
        if (!b)
            isNext = false;
    }

}

//-----------------------------
void Singleton::onPadDeath(){

    if (!PaddRevive){
        PaddAlpha -= 0.01f;
        if ((PaddAlpha<0) && (lives))
            PaddRevive = true;
        if ((!lives) && (PaddAlpha < 0)){
            PaddKilled = false;
            lives--;
            PaddAlpha = 1.0f;
        }
    }
    else{
        PaddAlpha += 0.01f;
                    
        if (PaddAlpha > 1){
            TakeNewLife();
            PaddKilled = false;
            PaddRevive = false;
            PaddAlpha = 1;
        }
    }
}
//-----------------------------
void Singleton::onPrizePickup(unsigned i){
#ifndef __ANDROID__
    ss.playsound(4);
#endif
    switch(Prizai[i].type){
        case 0: padd.length++; break; //padd ++
        case 1: if (padd.length>1) //padd --
                    padd.length--;
                break;
        case 2: padd.isMagnet = true; break;
        case 3: ReflectBricks = false; break;
        case 4:{ //multiply ball
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
//-------------------------
void Singleton::updateParticleSystems(){
    for (unsigned int i=0;i<PSystems.count();i++){
        PSystems[i].updateSystem();

        if (PSystems[i].isDead()){
            PSystems[i].destroy();
            PSystems.remove(i);
        }

    } 

}

//---------------------------
void Singleton::scrollWallpaper(){

    if ((scrooltim >= 0)&&(scrooltim < 5)){
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

}

//-----------------------------

void Singleton::GameLoop(){



    if (TitleScreen){ //vardo ivedimas

        if (NameBox.active()){


            float mx = -1;
            float my = -1;
            if (touches.up.count()){
                mx = touches.up[0].v[0];
                my = touches.up[0].v[1];
            }


            unsigned result = NameBox.getInput(globalKEY, mx, my);
            if (result){
                //printf("%c\n", globalKEY);
                if ((result != 8) && (result != 13) && (strlen(nameToEnter) < 11)&&(result < 127)){

                //printf("char %d\n", result);

                    unsigned pos = strlen(nameToEnter);
                    nameToEnter[pos] = (char)result;
                    nameToEnter[pos + 1] = 0;
                }
                else{
                    if (result == 8){
                        nameToEnter[strlen(nameToEnter) - 1] = 0;
                    }
                    else{

                        /*printf("char %d\n", result);
                        if ((strlen(nameToEnter) < 11)){
                            unsigned pos = strlen(nameToEnter);
                            nameToEnter[pos] = (char)result;
                            nameToEnter[pos + 1] = 0;
                        }*/

                    }
                }
            }

    
            if (NameBox.entered){


                Scores.addScore(nameToEnter,Score,10);
                Scores.write("scores.dat",10);
                NameBox.deactivate();
                NameBox.reset();
                Score=0;
            }
        }


        scrollWallpaper();

            
        //start game
        if ((((OldKeys[4])&&(!Keys[4]))
                ||(touches.up.count()))
                &&(!NameBox.active())){
            TitleScreen=false;
#ifndef __ANDROID__
            music.stop();
            music.open("music/musicingame.ogg");
            music.playback();
#endif
            bgpushy = 0;
            bgpushx = 0;
        }


    }

    else{ //GAME------------->


        if (!GamePaused){


                padd.tic++;
                if (padd.tic>50)
                    padd.tic=0;


                updateParticleSystems();

                 
                //--Judinam pada
                if ((padd.x + padd.length * 16 + 16 + PadX < SCREEN_WIDTH)&&
                    (padd.x-16-padd.length * 16 + PadX > 0)&&
                    ((!PaddKilled) || (PaddRevive)) && (!NextLevelTimer)){

                    unsigned i = 0;
                    if (balls.count()){
                        while ((!balls[i].colidepadd(balls[i].speed,0,padd.x,padd.y,padd.length) &&
                                (i<balls.count())))
                            i++;
                        if ((i>=0)&&(i<balls.count())){
                            if ((balls[i].x > 8) && (balls[i].x < 632))
                                padd.move(PadX, 0);
                        }
                        else
                            padd.move(PadX, 0);

                        for (unsigned int i=0;i < balls.count(); i++){
                            if (!balls[i].moving)
                                balls[i].x += PadX;
                        }   
                    }
                }
                //--------
                if (PaddKilled)
                    onPadDeath();

                bulletLogic();

                //--------------
                if ((Keys[4] || touches.up.count()) && (!NextLevelTimer)){ //space to launch ball
                    if ((padd.canShoot)&&(!PaddKilled)){

                        if (padd.reloadtic==0){
#ifndef __ANDROID__
                            ss.playsound(3);
#endif
                            Bullets.add(padd.x+padd.length*16+12,padd.y-20);
                            Bullets.add(padd.x-padd.length*16-12,padd.y-20);
                            padd.reloadtic = 1;
                        }
                    }
                    if (padd.reloadtic >= 1){
                        padd.reloadtic++;
                        if (padd.reloadtic > 10)
                            padd.reloadtic = 0;

                    }

                    for (unsigned i = 0; i < balls.count(); i++){
                        if (!balls[i].moving){
                            balls[i].moving = true;
                            balls[i].speed = DEFAULT_SPEED;
                            balls[i].angle = 3.14f/2.0f;

                        }
                    }
                }




                if (!NextLevelTimer){
                for (unsigned int i = 0; i < balls.count(); i++){

                    if (balls[i].moving){

                        if (balls[i].speed < MAX_SPEED)
                        balls[i].speed += 0.001f;

                        if (!balls[i].colidepadd(balls[i].speed,0,padd.x,padd.y,padd.length)){
                            ColidedBrick kalad[3];
                            if (balls[i].move(balls[i].speed, 0, &Map, kalad, ReflectBricks)){
#ifndef __ANDROID__
                                ss.playsound(5);
#endif
                            }


                            //Naikinam kaladeles----
                            if (kalad[0].colided){

                                if ((kalad[1].x==kalad[0].x)&&(kalad[1].y==kalad[0].y))
                                    kalad[1].colided = false;
                                else{
                                    if (((kalad[1].x==kalad[0].x+1) && (kalad[1].y==kalad[0].y)
                                        && (Map.tiles[kalad[0].y][kalad[0].x]%2==0))||
                                        ((kalad[1].x==kalad[0].x-1)&&(kalad[1].y==kalad[0].y)&&
                                         (Map.tiles[kalad[0].y][kalad[0].x]%2!=0))){
                                            kalad[1].colided=false;
                                    }
                                }

                                    if ((kalad[2].x==kalad[0].x)&&(kalad[2].y==kalad[0].y))
                                        kalad[2].colided=false;
                                    else{
                                        if (((kalad[2].x==kalad[0].x+1)&&(kalad[2].y==kalad[0].y)&&
                                             (Map.tiles[kalad[0].y][kalad[0].x]%2==0))||
                                            ((kalad[2].x==kalad[0].x-1)&&(kalad[2].y==kalad[0].y)&&
                                             (Map.tiles[kalad[0].y][kalad[0].x]%2!=0))){
                                                kalad[2].colided=false;
                                        }

                                        EliminateBrick(kalad[0].x,kalad[0].y,balls[i].speed);
                                    }

                            }
                            //**
                            if (kalad[1].colided){

                                if ((kalad[2].x==kalad[1].x)&&(kalad[2].y==kalad[1].y))
                                    kalad[2].colided=false;
                                else{
                                    if (((kalad[2].x==kalad[1].x+1)&&(kalad[2].y==kalad[1].y)&&(Map.tiles[kalad[1].y][kalad[1].x]%2==0))||
                                        ((kalad[2].x==kalad[1].x-1)&&(kalad[2].y==kalad[1].y)&&(Map.tiles[kalad[1].y][kalad[1].x]%2!=0))){
                                            kalad[2].colided=false;
                                    }

                                    EliminateBrick(kalad[1].x,kalad[1].y, balls[i].speed);
                                }
                            }
                            //**
                            if (kalad[2].colided){

                                EliminateBrick(kalad[2].x,kalad[2].y, balls[i].speed);
                            }
                            //------------------------


                        } else{ //if ball hits paddle;


                            if ((padd.isMagnet)&&
                                (balls[i].x+8.0f<=padd.x+((padd.length+1)*16-8))&&
                                (balls[i].x-8.0f>=padd.x-((padd.length+1)*16-8))){

                                    balls[i].angle = 0;
                                    balls[i].moving = false;
                            }
                            else{
#ifndef __ANDROID__
                                ss.playsound(0);
#endif
                                float kprc = ((3.14f/8.0f)*2.0f)/100.0f;
                                float iprc = ((padd.length+1)*16-8)/100.0f;
                                balls[i].angle = (3.14f/2.0f)-((balls[i].x-padd.x)/iprc)*kprc;
                                if ((balls[i].x + PadX > 8) && (balls[i].x + PadX < SCREEN_WIDTH - 8))
                                    balls[i].x += PadX;
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


                PadX = 0;
                //------
                if ((Map.brickCount < 1) && (NextLevelTimer == 0))
                    NextLevelTimer = 1;
                //-----

                if (!PaddKilled){
                    for (unsigned i = 0; i < Prizai.count(); i++){

                    Prizai[i].pos = Prizai[i].pos + Prizai[i].vel;

                    //TODO: use rectangle v circle collision
                    if ((_round(Prizai[i].pos.x()) + 16 > padd.x-padd.length * 16 - 16)&&
                            (_round(Prizai[i].pos.x()) - 16 < padd.x+padd.length * 16 + 16)){
                        
                        if ((padd.y-8 < _round(Prizai[i].pos.y())+16)){
                            onPrizePickup(i);
                        }

                    }
                    else
                    if (Prizai[i].pos.y()-16 > SCREEN_HEIGHT)
                        Prizai.remove(i);
                }

                if (lives < 0)
                    ResetGame();

                if (NextLevelTimer > 0){
                    NextLevelTimer++;
                    if (NextLevelTimer > 100){
                        NextLevelTimer = 0;
                        GoNextLevel();
                    }
                }
            }


        }
    }
}

//------------------------------------
void Singleton::DisplayLives(int x, int y){

    Padd padas;
    padas.length = 0;
    padas.y = y + 8;
    for (int i=0;i<lives;i++){
        padas.x = x + i * 32;
        padas.draw(pics, 1);
    }
}
//---------------------------
void Singleton::drawWallpaper(){

    for (int i = 0; i < 10; i++){
        for (int a = 0; a < 14; a++)
            pics.draw(pics.count() - 1,
                      (a-2)*64+bgpushx,
                      (i-2)*64+bgpushy,
                       0, (100-NextLevelTimer)/100.0f);
    }
    
}

//---------------------------

void Singleton::RenderScreen(){

    if (!GeneratingTexture)
        drawWallpaper();
    
    if (TitleScreen){
        pics.draw(7, 200,200,0);
        WriteText(260, 370, pics, 0, "tap to play...", 0.8f, 0.8f);
        Scores.display(pics, 0, 10, 0, 0);
        
        if (NameBox.active())
            NameBox.draw(pics, 0, 0, 0, COLOR(1,1,1));
    }
    else{ //in game

            Map.draw(pics,3,0,0);
            for (unsigned int i = 0;i < Prizai.count();i++){
                pics.draw(4, 
                                      _round(Prizai[i].pos.x()),
                                      _round(Prizai[i].pos.y()), Prizai[i].type, true);
            }
            padd.draw(pics, 1, PaddAlpha);
                        for (unsigned i=0;i<balls.count();i++)
                            balls[i].draw(pics, 2);
                        for (unsigned i=0;i<PSystems.count();i++)
                            PSystems[i].drawParticles(pics, 6);

                        Bullets.draw(pics, 6);

                        DisplayLives(300, 4);
                        DrawNumber(3, 3, Score, pics, 5);
                        //TODO:fix this
                        //if (GamePaused)
                        //    WriteText(150,220,pics,0,"GAME PAUSED (press P to unpause)",0.8f,0.8f,1.0f);

    }
    //TODO: fix it
    /*if (ShowDebugText)
        DrawSomeText();  */


}
//----------------------------

