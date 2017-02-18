#include "HighScore.h"

#include <cstdio>
#include "gui/OnScreenBoard.h"
#include "gui/Text.h"

//-------------------------------
void HighScores::load(const char* path, int count){
    FILE* failas;

    if ((failas=fopen(path,"rb"))==0){
      failas=fopen(path,"wb");
      fclose(failas);
    }
    else{
      
      for (int i=0;i<count;i++){
          Score scoras;
          if (!feof(failas)){
            fread(&scoras,sizeof(Score),1,failas);
            if ((strcmp(scoras.name,"")!=0)&&(scoras.score>0))
                data.add(scoras);
          }
          else break;
      }
      fclose(failas);
    }

}
//------------------
void HighScores::write(const char* path, int count){
    FILE* failas;

    failas = fopen(path,"wb+");
    if (count > (int)data.count())
        count=data.count();
    for (int i=0;i<count;i++){  

        fwrite(&data[i],sizeof(Score),1,failas);
    }
    fclose(failas);

}

//--------------------
void HighScores::addScore(const char* name, long score, int maxcount){
    if (score){
     Score tmp(name,score);
    
     int i=0;

     if (data.count()){
        while ((score<data[i].score)&&(i <= (int)data.count()))
            i++;
     }
    
    
      if ((int)data.count() < maxcount)
         data.add(tmp);
     
     if (i < (int)data.count()){
         for (int a = (int)data.count()-1; a>i;a--){
             strcpy(data[a].name,data[a-1].name);
             data[a].score=data[a-1].score;
         }
         strcpy(data[i].name,tmp.name);
         data[i].score=tmp.score;
     } 
         
     
    }

}

//-------------------
void HighScores::display(PicsContainer& pics, unsigned fontIndex, int count, int x, int y){
 
  char buf[150];
  
  if (count > (int)data.count())
      count=data.count();
  for (int i=0; i<count;i++){
      sprintf(buf, "%2d. %10s : %ld",i+1,data[i].name, data[i].score);
   WriteText(x, y+i*18, pics, fontIndex, buf, 0.8f,0.8f);
  }
   
}
//-----------------------------
void HighScores::destroy(){
    data.destroy();
}

//-----------------------------
Score HighScores::getScore(int index){
    Score sc;
    if ((index>=0)&&(index < (int)data.count())){
        strcpy(sc.name,data[index].name);
        sc.score=data[index].score;
    }

    return sc;
}

