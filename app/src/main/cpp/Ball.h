#ifndef BALL_H
#define BALL_H

#include "TextureLoader.h"
#include "BreakOutMap.h"
#include "Vectors.h"

enum types{FRONT,RIGHT,LEFT};


const float DEFAULT_SPEED = 4.0f;
const float SPEEDUP_STEP = 0.0015f;
const float MAX_SPEED = 10.0f;
const float DEFAULT_RADIUS = 8.0f;

struct ColidedBrick{
    int x;
    int y;
    bool colided;

    ColidedBrick(){
        x = 0; y = 0;
        colided = false;
    }
    ColidedBrick(int dx, int dy, bool c){
        x = dx; y = dy; colided = c;
    }

};
//-----------------------------
class Ball{

    float _speed;
    bool active;
    float radius;
public:
    float x;
    float y;
    float angle;

    bool     isActive(){return active;}
    void     deactivate(){active = false;}

    float    speed(){return _speed;}
    void     setSpeed(float newSpeed){_speed = newSpeed;}

    void     speedUp();

    void     launch(float sp = DEFAULT_SPEED, float a = 3.14f/2.0f){active = true; _speed = sp; angle = a;}

    void     setxy(float dx, float dy);


    bool     move(BreakOutMap* map, DArray<ColidedBrick>& kalad, bool reflectbricks);


    void     setangle(float _angle);
    void     draw(PicsContainer& spraitas, unsigned index);
    void     fall(float dy);
    bool     colidesWithPadle(int padx, int pady, int padlen);
    bool     isInMap(BreakOutMap& map, DArray<ColidedBrick>& bricks);
    float    rad(){return radius;}

             Ball(){
                    angle = 0;
                    active = false;
                    radius = DEFAULT_RADIUS;
             }
private:
    Vector3D reflection(float dx, float dy, unsigned char ** map, int type);

};


#endif //BALL_H
