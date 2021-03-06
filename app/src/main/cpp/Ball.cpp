#include "Ball.h"
#include "Usefull.h"
#include "Utils.h"
#include <cmath>

void Ball::setangle(float _angle){
    angle = _angle;
}
//---------------------------------------------
void Ball::draw(PicsContainer& pics, unsigned index){
    pics.draw(index, round(x), round(y), 0, true, radius/DEFAULT_RADIUS , radius/DEFAULT_RADIUS);  
}
//------------------------------------------

void Ball::setxy(float dx, float dy){
    x = dx * 1.0f;
    y = dy * 1.0f;
}

//--------------------------------------------------
Vector3D Ball::reflection(float dx, float dy, unsigned char ** map, int type){
    int inc = 0;
    bool colide=false;
    Vector3D nvec;

    switch (type){
        case FRONT: nvec=MakeVector(dx+inc,dy+8.0f,angle); break;
        case RIGHT: nvec=MakeVector(dx+inc,dy+8.0f,angle-3.14f/4.0f); break;
        case LEFT: nvec=MakeVector(dx+inc,dy+8.0f,angle+3.14f/4.0f); break;
    }

    while ((nvec.x() + x > 0)&&(y - nvec.y() > 0)&&(roundDouble2Int(nvec.x() + x) / 16 < 40)&&(!colide)){
        inc++;
        int ty=round(y-nvec.y())/16;
        int tx=round(x+nvec.x())/16;
        if ((ty>-1)&&(ty<30)&&(tx>-1)&&(tx<40)){
            if (map[ty][tx])
                colide=true;
        }
        switch (type){
            case FRONT: nvec=MakeVector(dx+inc,dy+8.0f,angle); break;
            case RIGHT: nvec=MakeVector(dx+inc,dy+8.0f,angle-3.14f/4.0f); break;
            case LEFT: nvec=MakeVector(dx+inc,dy+8.0f,angle+3.14f/4.0f); break;
        }
    }
 //----------------------------
    inc = 0;
    colide = false;
    Vector3D nvec2;
    switch (type){
        case FRONT: nvec2=MakeVector(dx+inc,dy-8.0f,angle); break;
        case RIGHT: nvec2=MakeVector(dx+inc,dy-8.0f,angle-3.14f/4.0f); break;
        case LEFT: nvec2=MakeVector(dx+inc,dy-8.0f,angle+3.14f/4.0f); break;
    }
    while ((nvec2.x() + x>0)&&(y-nvec2.y() > 0)&&(roundDouble2Int(nvec2.x() + x)/16<40)&&(!colide)){
        inc++;
        int ty=round(y-nvec.y())/16;
        int tx=round(x+nvec.x())/16;
        if ((ty>-1)&&(ty<30)&&(tx>-1)&&(tx<40)){
            if (map[ty][tx])
                colide=true;
        }
        switch (type){
            case FRONT: nvec2=MakeVector(dx+inc,dy-8.0f,angle); break;
            case RIGHT: nvec2=MakeVector(dx+inc,dy-8.0f,angle-3.14f/4.0f); break;
            case LEFT: nvec2=MakeVector(dx+inc,dy-8.0f,angle+3.14f/4.0f); break;
        }
    }
//---------------
    Vector3D vec;

    switch (type){
        case FRONT: vec=MakeVector(dx,dy,angle); break;
        case RIGHT: vec=MakeVector(dx,dy,angle-3.14f/4.0f); break;
        case LEFT: vec=MakeVector(dx,dy,angle+3.14f/4.0f); break;
    }
    vec.normalize();

    //let's build a wall's vector
    //float dist=sqrt(pow((nvec.x-nvec2.x),2)+pow((nvec.y-nvec2.y),2));
    
    float nvec_l = nvec.length();
    float nvec2_l = nvec2.length();
    
    if ((fabs(nvec2_l-nvec_l)>5.0f)&&(colide)){//if a corner is hit
        if (nvec2_l>nvec_l)
            nvec=nvec-vec;
        else
            nvec=nvec2-vec;
    }
    else{
        nvec=nvec-nvec2; //if everything's allright
        
    }
    nvec.normalize(); //let's normalize wall's vector

    return Vec2Reflection(vec,nvec);

}

//-----------------------------------------------------------------------------

bool Ball::isInMap(BreakOutMap& map, DArray<ColidedBrick>& bricks){
    float tempX = x;
    float tempY = y;
    Vector3D vec = MakeVector(_speed, 0, angle);
    tempX += vec.x();
    tempY -= vec.y();
    unsigned tiles = 0;

    unsigned upX = (unsigned)(roundDouble2Int(tempX - radius)/16);
    unsigned upY = (unsigned)(roundDouble2Int(tempY - radius)/16);
    unsigned downX = (unsigned)(roundDouble2Int(tempX + radius)/16);
    unsigned downY = (unsigned)(roundDouble2Int(tempY + radius)/16);
    for (unsigned a = upY; a <= downY; a++){
        for (unsigned i = upX; i <= downX; i++){
            if ((a < map.h())&&(i < map.w())){
                if (map.tiles[a][i]){
                    if (CollisionCircleRectangle(tempX, tempY, radius,
                                     i * 16, a * 16, 16, 16)){
                        tiles++;
                        bricks.add(ColidedBrick(i, a, true));
                        //map.tiles[a][i] = 0;
                    }
                }
            }
        }
    }
    if (tiles)
        return true;
    return false;
}
//-----------------------------------------------------------------------------
bool Ball::move(BreakOutMap* map, DArray<ColidedBrick>& kalad, bool reflectbricks){


    Vector3D vec= MakeVector(_speed, 0, angle);
    Vector3D vec2= MakeVector(_speed + radius, 0, angle);

    Vector3D vec3 = MakeVector(_speed, radius, angle - 3.14f / 4.0f);
    Vector3D vec4 = MakeVector(_speed, -radius, angle + 3.14f / 4.0f);

    Vector3D atspindys1(0.0f, 0.0f, 0.0f);
    Vector3D atspindys2(0.0f, 0.0f, 0.0f);
    Vector3D atspindys3(0.0f, 0.0f, 0.0f);

    bool colide = false;
    bool inMap = false;
    inMap = isInMap(*map, kalad);
    if (inMap){
        if ((!colide) && (reflectbricks))
            colide = true;

    }

    if ((x + vec2.x() < SCREEN_WIDTH) && (x+vec2.x() >= radius) && (y-vec2.y() >= radius)){       

        if (inMap){
            if (reflectbricks)
                atspindys1 = reflection(_speed, 0, map->tiles, FRONT);
        }
    }
    else {
        atspindys1 = reflection(_speed, 0, map->tiles, FRONT);
        colide = true;
    }
//--------------------------
    if ((x + vec3.x() < SCREEN_WIDTH) && (x+vec3.x() >= 0) && (y-vec3.y() >= 0)){ 

        if (inMap){
            if (reflectbricks)
                atspindys2 = reflection(_speed, 0, map->tiles, LEFT);
        }
    }
    else {
        atspindys2 = reflection(_speed, 0, map->tiles, LEFT);
        colide = true;
    }
//---------------------------
    if ((x + vec4.x() < SCREEN_WIDTH) && (x + vec4.x() >= 0) && (y - vec4.y() >= 0)){     

        if (inMap){
            if (reflectbricks)
                atspindys3 = reflection(_speed, 0, map->tiles, RIGHT);
        } 
    }
    else {
        atspindys3 = reflection(_speed, 0, map->tiles, RIGHT);
        colide = true;
    }

//--------------------------------
    if (colide){
        //let's add up all reflection vectors and find the reflection angle
        Vector3D atspindys = atspindys1 + atspindys2 + atspindys3;
        Vector3D gravity(0, -0.2f, 0);
        atspindys = atspindys + gravity;
        atspindys.normalize();
        Vector3D newpoint(x + atspindys.x(), y - atspindys.y(), 0);
        angle = -atan2(newpoint.y() - y  , newpoint.x() - x);

    }


    //ball moves
    if (!colide){
        x += vec.x();
        y -= vec.y();
    }

    return colide;

}
//-----------------------------

void Ball::speedUp(){

    if (_speed < MAX_SPEED)
        _speed += SPEEDUP_STEP;
}


//------------------------------------------------------------------------------
void Ball::fall(float dy){
    y += dy;
}
//--------------------------------------------------------------------------------
bool Ball::colidesWithPadle(int padx, int pady, int padlen){

    Vector3D vec = MakeVector(_speed, 0, angle);
    Vector3D vec2 = MakeVector(_speed + radius, 0, angle + 3.14f / 4.0f);
    Vector3D vec3 = MakeVector(_speed + radius, 0, angle - 3.14f / 4.0f);


    int padLeftX = padx - padlen * 16 - 16;
    int padRightX = padx + padlen * 16 + 16;
    int padTopY = pady - 8;
    
    if ((x + vec2.x() > padLeftX) && (x + vec2.x() < padRightX) && 
        (y - vec2.y() > padTopY) && (y - vec2.y() < pady))
        return true;
    if ((x + vec3.x() > padLeftX) && (x + vec3.x() < padRightX) && 
        (y - vec3.y() > padTopY) && (y - vec3.y() < pady))
        return true;
    if ((x + vec.x() > padLeftX) && (x + vec.x() < padRightX) && 
        (y - vec.y() > padTopY) && (y - vec.y() < pady))
        return true;
    

    return false;


    //return CollisionCircleRectangle(x + vec.v[0], y + vec.v[1], 8,
    //                                padx - padlen*16 - 16, pady - 8, (padlen*16+16)*2, 16);
}
