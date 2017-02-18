#include "BreakOutMap.h"
#include <string.h>
#include <stdlib.h>

void BreakOutMap::create(unsigned int nwidth, unsigned int nheight){
    width = nwidth;
    height = nheight;
    tiles = new unsigned char* [height];
    for (unsigned i = 0; i < height; i++){
        tiles[i] = new unsigned char[width];
        memset(tiles[i], 0, width);
    }

}

//------------------------
void BreakOutMap::generate(){

    
    for(int i = 8; i < 14; i++){
        for (int a = 5; a < 20; a += 2){
            int nmb = 0;
            if (rand()%100 > 50)
                nmb = rand()%7;


            if (nmb==0){
                tiles[i][a]=0;
                tiles[i][a+1]=0;
            }
            else{
                brickCount++;
                tiles[i][a]=nmb*2-1;
                tiles[i][a+1]=tiles[i][a]+1;
            }
        }
    }

    for (int i = 0; i < 6; i++){
        for (int a = 2; a < 15; a += 2){
            if (tiles[8 + i][19 - a]){
                    brickCount++;
                    tiles[i+8][a+19]=tiles[8+i][19-a];
                    tiles[i+8][a+20]=tiles[8+i][19-a]+1;
                    
                }
            }

    }


        for (int i=0;i<6;i++){
            for (int a=0;a<30;a++){
                
                    tiles[i+14][a+5]=tiles[13-i][5+a];
                    
                
            }

        }

        brickCount*=2;
        

}
//-------------------------------
void BreakOutMap::draw(PicsContainer& tileset, unsigned index, int x, int y){
    for (unsigned i = 0; i < height; i++){
        for(unsigned a = 0; a < width; a++){
            if (tiles[i][a])
                tileset.draw(index, x+a*16,y+i*16,tiles[i][a]-1);
        }
    }
}

//-----------------------------------------------------
void BreakOutMap::destroy(){
    for (unsigned i=0; i < height; i++)
        delete []tiles[i];
    delete []tiles;
    tiles = 0;
    height = width = 0;
}
//------------------------------------------------
int BreakOutMap::removetile(unsigned int x, unsigned int y){

    if ((y < height) && (y>=0) && (x < width) && (x>=0)){

        int tile=tiles[y][x];
        if (tile > 0){//tile not empty
            bool hard = false;
            if (((tile + 1) / 2 == 6) || ((tile + 1) / 2 == 5)){
                tiles[y][x] -= 2;
                hard = true;
            }
            else
                tiles[y][x]=0;
        
            if (tile%2==0){
                if (!hard)
                    tiles[y][x-1] = 0;
                else
                    tiles[y][x-1] -= 2;
            }
            else{
                if (!hard)
                    tiles[y][x+1] = 0;
                else
                    tiles[y][x+1] -= 2;
            }
            if (!hard)
                brickCount -= 1;

            if ((tile > 0) && (tile < 2))
                return tile;
            else
                return (tile + 1) / 2;
        }else{
            printf("tile x:%u y:%u is already empty\n", x, y);
            return 0;
        }
    }
    else {
        printf("x:%u y:%u are out of limits\n", x, y);
        return 0;
    }
}
