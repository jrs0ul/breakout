#ifndef BREAKOUTMAP_H
#define BREAKOUTMAP_H

#include "TextureLoader.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
class BreakOutMap{
    unsigned width;
    unsigned height;
public:
    unsigned char ** tiles;
    int brickCount;
    
    

    BreakOutMap(){
        tiles = 0;
        brickCount = 0;
    }
    unsigned w(){return width;}
    unsigned h(){return height;}
    void create(unsigned nwidth, unsigned nheight);
    void destroy();
    void generate();
    int  removeTile(unsigned x, unsigned y);
    void draw(PicsContainer& pics, unsigned index, int x, int y);

};


#endif //BREAKOUTMAP_H
