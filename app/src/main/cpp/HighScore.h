#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <cstring>
#include "DArray.h"
#include "TextureLoader.h"


//------------------------------------
struct Score{
    char name[11];
    long score;

    Score(){
        strcpy(name,"");
        score = 0;
    }
    Score(const char* _name, long _score){
        if (strlen(_name) < 11)
            strcpy(name, _name);
        else{
            strncpy(name, _name, 10);
            name[10] = 0;
        }
        score=_score;
    }
    
};
//----------------------------------------
class HighScores{
    DArray<Score> data;
public:
    Score getScore(int index);
    void load(const char* path, int count);
    void write(const char* path, int count);
    void addScore(const char* name, long score, int maxcount);
    void display(PicsContainer& pics, unsigned fontIndex, int count, int x, int y);
    void destroy();
};




#endif //HIGHSCORE_H
