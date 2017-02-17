#ifndef MAP_H
#define MAP_H

#include "TextureLoader.h"

class BreakOutMap{
    unsigned width;
    unsigned height;
public:
    unsigned char ** tiles;
    int brickCount;
    
    

    BreakOutMap(){
        tiles=0;
        brickCount=0;
    }
    void create(unsigned nwidth, unsigned nheight);
    void destroy();
    void generate();
    int removetile(unsigned x, unsigned y);
    void draw(PicsContainer& pics, unsigned index, int x, int y);

};


#endif //MAP_H
