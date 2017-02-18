#ifndef PADLE_H
#define PADLE_H

#include "TextureLoader.h"

class Padle{
public:
    int x;
    int y;
    int length;
    bool isMagnet;
    bool canShoot;
    int tic;
    int reloadtic;

    Padle(){
        length = 1;
        isMagnet = false;
        canShoot = false;
        tic = 0;
        reloadtic = 0;
    }

    void setxy(int dx, int dy);
    void move(int dx, int dy);
    void draw(PicsContainer& img, unsigned index, float alpha=1.0f);
};



#endif //PADLE_H
