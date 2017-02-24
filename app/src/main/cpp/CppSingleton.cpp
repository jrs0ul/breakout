
#include "CppSingleton.h"
#include "Usefull.h"
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
#else
    if (!ss.init())
        ss.exit();
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
    pics.loadFile("pics/rod.tga",     8, 16,16,0, AssetManager);
    pics.loadFile("pics/guistuff.tga", 9, 64, 64, 1, AssetManager);
#endif

    useAccel = false;

    srand((unsigned int)time(0));




    Map.create(40,30);
    Map.generate();

    //TODO: let's fix this
    Scores.load("scores.dat",10);
    
    char letters[] = "1234567890QWERTYUIOPASDFGHJKL:ZXCVBNM@$&*-/\\!#?_=() .,";
    NameBox.setChars((const unsigned char*)letters);
    NameBox.setCharWidth(50);
    NameBox.setRowLen(10);
    NameBox.setpos(50, 65);


    padle.setxy(SCREEN_WIDTH/2, SCREEN_HEIGHT - 32);

    Ball newball;
    newball.setxy(SCREEN_WIDTH/2, padle.y - 8 - newball.rad());
    balls.add(newball);


    GenerateTexture(64, 64);
    GeneratingTexture = false;


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
        PadX = touches.down[0].v[0] - padle.x;
    }
    if (touches.move.count()){
        PadX = touches.move[0].v[0] - padle.x;
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
#else
    ss.exit();
#endif
    pics.destroy();
}

//==================================================
int Singleton::FPS (void){
    static int ctime = 0, FPS = 0, frames = 0, frames0 = 0;
    if ((int)TimeTicks >= ctime) {
        FPS = frames - frames0;
        ctime = (int)TimeTicks + 1000;
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

    if ((padle.length == 1) && (newprize.type == SHRINK))
        newprize.type = GROW;
    if ((padle.length > padle.maxLength) && (newprize.type == GROW))
        newprize.type = SHRINK;
    if ((padle.isMagnet) && (newprize.type == MAGNET))
        newprize.type = MULTIPLY;
    if ((padle.canShoot) && (newprize.type == GUNS))
        newprize.type = DEATH;
    if ((!ReflectBricks) && (newprize.type == NOCLIP))
        newprize.type = DEATH;

    Prizai.add(newprize);
}


//-------------------------
void Singleton::EliminateBrick(int tx, int ty, float speed){

    int brickid = 0;
    int x,y;

    if (Map.tiles[ty][tx]%2 != 0)
        x = tx * 16 + 16;
    else
        x = tx * 16;

    y = ty * 16 + 8;

    brickid = Map.removeTile(tx, ty);
    if (brickid){
        if (brickid == 3)
            CreatePrize(tx,ty);
        Score += 5 * roundDouble2Int(speed / DEFAULT_SPEED);
        if (brickid < 5){
#ifndef __ANDROID__
            ss.playsound(2);
#endif
            Particle2DSystem ps;
            ps.setPos(x, 0, y);
            ps.setDirIntervals(Vector3D(1,1,1) , 360);
            ps.setParticleLifetime(10);
            ps.setSystemLifetime(30);
            ps.setColors(COLOR(1.0f, 0.0f, 0.0f, 1.0f), COLOR(1.0f, 1.0f, 0.0f, 0.2f));
            ps.setSizes(0.3f, 0.9f);
            ps.revive();
            PSystems.add(ps);
        }
    }
}
//--------------------------
void Singleton::TakeNewLife(){

    Ball newball;
    newball.deactivate();
    newball.angle = 0;
    newball.setxy(padle.x, padle.y - 8 - newball.rad());
    balls.add(newball);
    padle.canShoot = false;
    padle.isMagnet = false;
    ReflectBricks = true;
    lives--;

}
//---------------------
void Singleton::KillPadd(){
    padleKilled = true;
#ifndef __ANDROID__
    ss.playsound(1);
#endif

    Particle2DSystem ps;

    ps.setPos(padle.x, 0, padle.y);
    ps.setDirIntervals(Vector3D(1,1,1), 360);
    ps.setParticleLifetime(50);
    ps.setSystemLifetime(50);
    ps.setColors(COLOR(1,1,1,1), COLOR(1,1,1,0));
    ps.setSizes(0.2f, 0.7f);
    ps.revive();

    PSystems.add(ps);

}
//---------------------------------
void Singleton::ResetGame(){
    Bullets.destroy();
    Prizai.destroy();
    //TODO fix it
    //Scores.load("scores.dat",10);
    lives = NUM_LIVES;
    padle.length = 1;
    balls.destroy();
    Ball newball;
    newball.setxy(padle.x, padle.y - 8 - newball.rad());
    balls.add(newball);

    Map.destroy();
    Map.brickCount = 0;
    Map.create(40, 30);
    Map.generate();
    padle.canShoot = false;
    padle.isMagnet = false;
    ReflectBricks = true;
    //---------
    GeneratingTexture = true;
    pics.remove(pics.count()-1);
    GenerateTexture(64, 64);
    GeneratingTexture = false;

    printf ("%ld > %ld ?\n", Score, Scores.getScore(9).score);
    if (Score > Scores.getScore(9).score)
        NameBox.activate();
    gameState = TITLE;
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
    newball.setxy(padle.x, padle.y - 16);
    newball.deactivate();
    balls.add(newball);
    GeneratingTexture = true;
    pics.remove(pics.count() - 1);
    GenerateTexture(64, 64);
    GeneratingTexture = false;
    Map.generate();
    padle.canShoot = false;
    padle.isMagnet = false;
    ReflectBricks = true;
    Bullets.destroy();
    Prizai.destroy();
    padle.length = 1;


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

    if (!padleRevive){
        padleAlpha -= 0.01f;
        if ((padleAlpha<0) && (lives))
            padleRevive = true;
        if ((!lives) && (padleAlpha < 0)){
            padleKilled = false;
            lives--;
            padleAlpha = 1.0f;
        }
    }
    else{
        padleAlpha += 0.01f;
                    
        if (padleAlpha > 1){
            TakeNewLife();
            padleKilled = false;
            padleRevive = false;
            padleAlpha = 1;
        }
    }
}
//-----------------------------
void Singleton::onPrizePickup(unsigned i){
#ifndef __ANDROID__
    ss.playsound(4);
#endif
    switch(Prizai[i].type){
        case GROW:    padle.length++; break; 
        case SHRINK:  if (padle.length > 1) padle.length--; break;
        case MAGNET:  padle.isMagnet = true; break;
        case NOCLIP:  ReflectBricks = false; break;
        case MULTIPLY:{ //multiply balls x2
                        if (balls.count()){
                            unsigned cnt=balls.count();
                            for (unsigned int i=0; i < cnt; i++){
                                if (!balls[i].isActive()){
                                    balls[i].launch();
                                }
                                Ball newball;
                                newball.setxy(balls[i].x,balls[i].y);
                                newball.launch(balls[i].speed(), -balls[i].angle);
                                balls.add(newball);
                            }
                        }
                }break;

        case SLOWDOWN:{ //slow balls :)
            for (unsigned int i=0; i<balls.count(); i++)
                            balls[i].setSpeed(DEFAULT_SPEED/2.0f);
                } break;

        case GUNS: padle.canShoot = true; break;
        case DEATH:{ //death
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

    if ((scrooltim >= 0.0f) && (scrooltim < 5.0f)){
            bgShiftY += (1.0f * DT * 60.0f);
            if (bgShiftY >= 64.0f){
                bgShiftY = 0.0f;
                scrooltim += (1.0f * DT * 60.0f); 
            }
        }
        else{
            bgShiftX += (1.0f * DT * 60.0f);
            if (bgShiftX >= 64.0f){
                bgShiftX = 0.0f;
                scrooltim += (1.0f * DT * 60.0f);
                if (scrooltim >= 10.0f)
                    scrooltim = 0.0f;
            }
        }

}
//-----------------------------
void Singleton::onTitleScreen(){

    scrollWallpaper();
    if (NameBox.active()){


            float mx = -1;
            float my = -1;
            if (touches.up.count()){
                mx = touches.up[0].v[0];
                my = touches.up[0].v[1];
            }


            unsigned result = NameBox.getInput(globalKEY, mx, my);
            if (result){
                printf("%c\n", globalKEY);
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

                printf("adding score...\n");
                Scores.addScore(nameToEnter, Score, 10);
                printf("done!\n");
                //Scores.write("scores.dat",10);
                NameBox.deactivate();
                NameBox.reset();
                Score=0;
            }
        }
        else{

        //start game
            if ((((OldKeys[4])&&(!Keys[4]))
                ||(touches.up.count()))){
            
                gameState = GAME;
#ifndef __ANDROID__
                music.stop();
                music.open("music/musicingame.ogg");
                music.playback();
#endif
                bgShiftY = 0;
                bgShiftX = 0;
            }
        }

}
//----------------------------
void Singleton::clearBricks(){
    if (cbricks.count() > 1){
        for (unsigned a = 0; a < cbricks.count() - 1; a++){
            unsigned x0 = cbricks[a].x;
            unsigned y0 = cbricks[a].y;

            for (unsigned i = 0; i < cbricks.count(); i++){

                if (i > a){
                    unsigned x = cbricks[i].x;
                    unsigned y = cbricks[i].y;
                    if ((x == x0)&&(y == y0)){
                    
                        cbricks[i].colided = false;
                    }
                }

            }
        }
    }

    for (unsigned i = 0; i < cbricks.count(); i++){
        //printf("value:%u\n", Map.tiles[cbricks[i].y][cbricks[i].x]);
        if (cbricks[i].colided){
            unsigned x = cbricks[i].x;
            unsigned y = cbricks[i].y;
            unsigned val = Map.tiles[y][x];
            if (val)
                EliminateBrick(x , y, balls[0].speed());
        }
    }

    cbricks.destroy();


}

//-----------------------------

void Singleton::GameLoop(){


    switch(gameState){
        case TITLE:{
            onTitleScreen();
        } break;
        case GAME:{ 

            if (!GamePaused){
                padle.tic++;
                if (padle.tic > 50)
                    padle.tic = 0;

                updateParticleSystems();

                //move padle
                if ((padle.x + padle.length * 16 + 16 + PadX < SCREEN_WIDTH)&&
                    (padle.x-16-padle.length * 16 + PadX > 0)&&
                    ((!padleKilled) || (padleRevive)) && (!NextLevelTimer)){

                    unsigned i = 0;
                    if (balls.count()){
                        while ((!balls[i].colidesWithPadle(padle.x, padle.y, padle.length) &&
                                (i < balls.count())))
                            i++;
                        if ((i >= 0) && (i < balls.count())){
                            if ((balls[i].x > 8) && (balls[i].x < SCREEN_WIDTH - 8))
                                padle.move(PadX, 0);
                        }
                        else
                            padle.move(PadX, 0);

                        for (unsigned int i=0; i < balls.count(); i++){
                            if (!balls[i].isActive())
                                balls[i].x += PadX;
                        }   
                    }
                }
                //--------
                if (padleKilled)
                    onPadDeath();

                bulletLogic();

                //--------------
                if ((Keys[4] || touches.up.count()) && (!NextLevelTimer)){ //launches  ball(s)
                    
                    for (unsigned i = 0; i < balls.count(); i++){
                        if (!balls[i].isActive()){
                            balls[i].launch();
                        }
                    }
                }

                //let's spread some bullets
                if ((padle.canShoot) && (!padleKilled)){

                    if (padle.reloadtic == 0){
#ifndef __ANDROID__
                        ss.playsound(3);
#endif
                        Bullets.add(padle.x + padle.length * 16 + 12, padle.y - 20);
                        Bullets.add(padle.x - padle.length * 16 - 12, padle.y - 20);
                        padle.reloadtic = 1;
                    }
                }
                if (padle.reloadtic >= 1){
                    padle.reloadtic += (1.0f * DT * 60.0f);
                    if (padle.reloadtic > 20)
                        padle.reloadtic = 0;

                }
                //---
                if (!NextLevelTimer){
                    for (unsigned int i = 0; i < balls.count(); i++){

                        if (balls[i].isActive()){

                            balls[i].speedUp();
                            
                            //doesn't this ball colide with padle ?    
                            if (!balls[i].colidesWithPadle(padle.x, padle.y, padle.length)){

                                //moves and gets collision data
                                if (balls[i].move(&Map, cbricks, ReflectBricks)){

#ifndef __ANDROID__
                                    ss.playsound(5);
#endif                              
                                }
                                clearBricks();

                            } 
                            else{ //so the ball does colide with the padle


                                if ((padle.isMagnet)&&
                                    (balls[i].x+8.0f <= padle.x + ((padle.length + 1)*16 - 8))&&
                                    (balls[i].x-8.0f >= padle.x - ((padle.length + 1)*16 - 8))){

                                        balls[i].angle = 0;
                                        balls[i].deactivate();
                                }
                                else{
#ifndef __ANDROID__
                                    ss.playsound(0);
#endif
                                    float kprc = ((3.14f/8.0f)*2.0f)/100.0f;
                                    float iprc = ((padle.length + 1)*16-8)/100.0f;
                                    balls[i].angle = (3.14f/2.0f)-((balls[i].x - padle.x)/iprc)*kprc;
                                    if ((balls[i].x + PadX > 8) && (balls[i].x + PadX < SCREEN_WIDTH - 8))
                                        balls[i].x += PadX;
                                }

                            }


                            if (balls[i].y > padle.y + 8){
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

                if (!padleKilled){
                    for (unsigned i = 0; i < Prizai.count(); i++){

                    Prizai[i].pos = Prizai[i].pos + Prizai[i].vel;

                    if (CollisionCircleRectangle(Prizai[i].pos.x(), Prizai[i].pos.y(), 16,
                                                 padle.x - padle.length * 16 - 16, padle.y - 8,
                                                 (padle.length * 16 + 16) * 2, 16)){

                        onPrizePickup(i);

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
    }

//------------------------------------
void Singleton::DisplayLives(int x, int y){

    Padle padas;
    padas.length = 0;
    padas.y = y + 8;
    for (int i = 0; i<lives; i++){
        padas.x = x + i * 32;
        padas.draw(pics, 1);
    }
}
//---------------------------
void Singleton::drawWallpaper(){

    for (int i = 0; i < 10; i++){
        for (int a = 0; a < 14; a++)
            pics.draw(pics.count() - 1,
                      (a-2)*64 + bgShiftX,
                      (i-2)*64 + bgShiftY,
                       0, (100 - NextLevelTimer)/100.0f);
    }
    
}

//---------------------------

void Singleton::RenderScreen(){

    if (!GeneratingTexture)
        drawWallpaper();
    
    if (gameState == TITLE){
                
        if (NameBox.active()){
            WriteShadedText(60, 10, pics, 0, "Enter your name:");

            WriteShadedText(90, 30, pics, 0, nameToEnter);
            WriteShadedText(90 + strlen(nameToEnter) * 11, 40, pics, 0, (rand()%2 == 0) ? "_" : " ");

            //WriteText(SCREEN_WIDTH/2 - strlen(nameToEnter)*16*0.8f/2, 30, pics, 0, nameToEnter, 0.8f, 0.8f);
            NameBox.draw(pics, 9, 8, 0, COLOR(1,1,1, 0.5));
        }
        else{
            pics.draw(7, 200, 200,0);
            WriteText(260, 370, pics, 0, "tap to play...", 0.8f, 0.8f);
            Scores.display(pics, 0, 10, 5, 5);
        }
    }
    else{ //in game

        Map.draw(pics,3,0,0);
        for (unsigned int i = 0;i < Prizai.count(); i++){
            pics.draw(4, roundDouble2Int(Prizai[i].pos.x()),
                         roundDouble2Int(Prizai[i].pos.y()), Prizai[i].type, true);
        }

        padle.draw(pics, 1, padleAlpha);
        for (unsigned i = 0; i < balls.count(); i++)
            balls[i].draw(pics, 2);

        for (unsigned i = 0; i < PSystems.count(); i++)
            PSystems[i].drawParticles(pics, 6);

        Bullets.draw(pics, 6);

        DisplayLives(300, 4);
        DrawNumber(3, 3, Score, pics, 5);
        //TODO:fix this
        //if (GamePaused)
        //    WriteText(150,220,pics,0,"GAME PAUSED (press P to unpause)",0.8f,0.8f,1.0f);

    }
    if (ShowDebugText)
        DrawDebugText();  


}
//----------------------------
void Singleton::DrawDebugText(){

    char buf[256];
    sprintf(buf, "FPS:%d", FPS());
    WriteText(2, 60, pics, 0, buf, 0.8f, 0.8f);
    if (balls.count()){
        sprintf(buf, "speed:%.3f", balls[0].speed());
        WriteText(2, 85, pics, 0, buf, 0.8f, 0.8f);
    }
    sprintf(buf, "Accumulator:%.2f", Accumulator);
    WriteText(2, 105, pics, 0, buf, 0.8f, 0.8f);

}
