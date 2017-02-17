#include "Ball.h"
#include "Usefull.h"
#include "Utils.h"
#include <cmath>

void Ball::setangle(float _angle){
    angle=_angle;
}
//---------------------------------------------
void Ball::draw(PicsContainer& pics, unsigned index){
    pics.draw(index, round(x), round(y), 0, true);  
}
//------------------------------------------

void Ball::setxy(float dx, float dy){
    x=dx*1.0f;
    y=dy*1.0f;
}
//--------------------------------------------------
Vector3D Ball::reflection(float dx, float dy, unsigned char ** map, int type){
    int inc=0;
    bool colide=false;
    Vector3D nvec;

    switch (type){
        case 0: nvec=MakeVector(dx+inc,dy+8.0f,angle); break;
        case 1: nvec=MakeVector(dx+inc,dy+8.0f,angle-3.14f/4.0f); break;
        case 2: nvec=MakeVector(dx+inc,dy+8.0f,angle+3.14f/4.0f); break;
    }

    while ((nvec.x() + x > 0)&&(y - nvec.y() > 0)&&(_round(nvec.x() + x) / 16 < 40)&&(!colide)){
        inc++;
        int ty=round(y-nvec.y())/16;
        int tx=round(x+nvec.x())/16;
        if ((ty>-1)&&(ty<30)&&(tx>-1)&&(tx<40)){
            if (map[ty][tx])
                colide=true;
        }
        switch (type){
            case 0: nvec=MakeVector(dx+inc,dy+8.0f,angle); break;
            case 1: nvec=MakeVector(dx+inc,dy+8.0f,angle-3.14f/4.0f); break;
            case 2: nvec=MakeVector(dx+inc,dy+8.0f,angle+3.14f/4.0f); break;
        }
    }
 //----------------------------
    inc=0;
    colide=false;
    Vector3D nvec2;
    switch (type){
        case 0: nvec2=MakeVector(dx+inc,dy-8.0f,angle); break;
        case 1: nvec2=MakeVector(dx+inc,dy-8.0f,angle-3.14f/4.0f); break;
        case 2: nvec2=MakeVector(dx+inc,dy-8.0f,angle+3.14f/4.0f); break;
    }
    while ((nvec2.x() + x>0)&&(y-nvec2.y() > 0)&&(_round(nvec2.x() + x)/16<40)&&(!colide)){
        inc++;
        int ty=round(y-nvec.y())/16;
        int tx=round(x+nvec.x())/16;
        if ((ty>-1)&&(ty<30)&&(tx>-1)&&(tx<40)){
            if (map[ty][tx])
                colide=true;
        }
        switch (type){
            case 0: nvec2=MakeVector(dx+inc,dy-8.0f,angle); break;
            case 1: nvec2=MakeVector(dx+inc,dy-8.0f,angle-3.14f/4.0f); break;
            case 2: nvec2=MakeVector(dx+inc,dy-8.0f,angle+3.14f/4.0f); break;
        }
    }
//---------------
    Vector3D vec;

    switch (type){
        case 0: vec=MakeVector(dx,dy,angle); break;
        case 1: vec=MakeVector(dx,dy,angle-3.14f/4.0f); break;
        case 2: vec=MakeVector(dx,dy,angle+3.14f/4.0f); break;
    }
    vec.normalize();

    //pagaminame sienos vektoriu
    //float dist=sqrt(pow((nvec.x-nvec2.x),2)+pow((nvec.y-nvec2.y),2));
    
    float nvec_l = nvec.length();
    float nvec2_l = nvec2.length();
    
    if ((fabs(nvec2_l-nvec_l)>5.0f)&&(colide)){//jei atsimuse i kladeles kampa
        if (nvec2_l>nvec_l)
            nvec=nvec-vec;
        else
            nvec=nvec2-vec;
    }
    else{
        nvec=nvec-nvec2; //jei viskas ok
        
    }
    nvec.normalize(); //let's normalise wall's vector

    return Vec2Reflection(vec,nvec);

}

//-----------------------------------------------------------------------------
bool Ball::move(float dx, float dy, BreakOutMap* map, ColidedBrick* kalad, bool reflectbricks){

    Vector3D vec= MakeVector(dx,dy, angle);
    Vector3D vec2= MakeVector(dx+8.0f,dy, angle);

    Vector3D vec3= MakeVector(dx,dy+8.0f, angle-3.14f/4.0f);
    Vector3D vec4= MakeVector(dx,dy-8.0f, angle+3.14f/4.0f);

    Vector3D atspindys1(0, 0, 0);
    Vector3D atspindys2(0, 0, 0);
    Vector3D atspindys3(0, 0, 0);

    bool colide = false;
    

    if ((_round(x + vec2.x())/16 < 40) && (x+vec2.x() >= 8)&&(y-vec2.y() >= 8)){       
        int ty = _round(y - vec2.y()) / 16;
        int tx = _round(x + vec2.x()) / 16;
        if ((ty<30)&&(ty>-1)&&(tx<40)&&(tx>-1)){
            if (map->tiles[ty][tx]){
                if (reflectbricks)
                    atspindys1=reflection(dx,dy,map->tiles,FRONT);
                if (kalad)
                    kalad[0]=ColidedBrick(tx,ty,true);
                if ((!colide)&&(reflectbricks))
                    colide=true;
            }
        }

    }
    else {
        
        atspindys1 = reflection(dx,dy,map->tiles,FRONT);
        if (!colide)
                colide=true;
        
    }
//-----------------------------------------
    
    if ((round(x + vec3.x()) / 16 < 40) && (x+vec3.x() >= 0) && (y-vec3.y() >= 0)){     
        int ty = _round(y - vec3.y()) / 16;
        int tx = _round(x+vec3.x()) / 16;
        if ((ty < 30)&&(ty > -1)&&(tx < 40)&&(tx>-1)){
            if (map->tiles[ty][tx]){
                if (reflectbricks)
                    atspindys2 = reflection(dx,dy,map->tiles,LEFT);
                if (kalad)
                    kalad[1]=ColidedBrick(tx,ty,true);
                if ((!colide)&&(reflectbricks))
                    colide=true;            
            }
        }
    }
    else {
        
        atspindys2=reflection(dx,dy,map->tiles,LEFT);
        if (!colide)
                colide=true;
        
        
    }
//-----------------------------------------
    
    if ((round(x+vec4.x()) / 16 < 40) && (x + vec4.x() >= 0) && (y-vec4.y() >= 0)){     
        int ty = _round(y-vec4.y()) / 16;
        int tx = _round(x+vec4.x()) / 16;
        if ((ty<30)&&(ty>-1)&&(tx<40)&&(tx>-1)){
            if (map->tiles[ty][tx]){
                if (reflectbricks)
                    atspindys3 = reflection(dx,dy,map->tiles,RIGHT);
                if (kalad)
                    kalad[2]=ColidedBrick(tx,ty,true);
                if ((!colide)&&(reflectbricks))
                    colide=true;
            }
        }

    }
    else {
        
        atspindys3=reflection(dx, dy, map->tiles, RIGHT);
        if (!colide)
                colide=true;
        
        
    }

//--------------------------------
    if (colide){
        //sudedam visus atspindzio vektorius ir randam atspindzio kampa
        Vector3D atspindys = atspindys1 + atspindys2 + atspindys3;
        Vector3D gravity;
        gravity.set(0, -0.2f, 0);
        atspindys = atspindys + gravity;
        atspindys.normalize();
        Vector3D newpoint(x + atspindys.x(), y - atspindys.y(), 0);
        angle = -atan2(newpoint.y() - y  , newpoint.x() - x);

    }



    if (!colide){
        x += vec.x();
        y -= vec.y();
    }

    return colide;

}
//------------------------------------------------------------------------------
void Ball::fall(float dy){
    y+=dy;
}
//--------------------------------------------------------------------------------
bool Ball::colidepadd(float dx, float dy, int padx, int pady, int padlen){

    Vector3D vec = MakeVector(dx+8.0f,dy, angle);
    Vector3D vec2 = MakeVector(dx+8.0f,dy, angle+3.14f/4.0f);
    Vector3D vec3 = MakeVector(dx+8.0f,dy, angle-3.14f/4.0f);

    
    if ((x + vec2.x() > padx-padlen * 16 - 16) && (x+vec2.x() < padx + padlen * 16 + 16) && 
        (y-vec2.y() > pady-8) && (y-vec2.y() < pady))
        return true;
    if ((x+vec3.x() > padx-padlen * 16 - 16) && (x + vec3.x() < padx+padlen*16+16) && 
        (y-vec3.y() > pady-8) && (y-vec3.y() < pady))
        return true;
    if ((x+vec.x() > padx-padlen * 16 - 16) && (x+vec.x()<padx+padlen*16+16) && 
        (y-vec.y()>pady-8) && (y-vec.y()<pady))
        return true;
    

    return false;
}
