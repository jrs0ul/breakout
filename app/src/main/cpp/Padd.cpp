#include "Padd.h"


void Padd::setxy(int dx, int dy){
    x=dx; y=dy;
}

void Padd::move(int dx, int dy){
 x+=dx; y+=dy;
}

void Padd::draw(PicsContainer &img, unsigned index, float alpha){

    int frm=6;
    if (tic<20)
        frm=8;

    for (int i=0;i<length;i++){
     img.draw(index, x-8-(i*16), y, 1, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
     img.draw(index, x+8+(i*16), y, 1, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
     if (isMagnet){    
        img.draw(index, x+8+(i*16),y-16,frm, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
        img.draw(index, x-8-(i*16),y-16,frm, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
     }
    }
    int peframe = 2;
    int pbframe = 0;
    if ((isMagnet)||(canShoot)){
        peframe = 4;
        pbframe = 3;
    }
    img.draw(index, x+16+length*16-8,y,peframe, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
    img.draw(index, x-16-length*16+8,y,pbframe, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));

    if ((isMagnet)&&(!canShoot)){
        img.draw(index, x+16+length*16-8,y-16,7, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
        img.draw(index, x-16-length*16+8,y-16,5, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
    }
    if ((canShoot)&&(!isMagnet)){

        img.draw(index, x+16+length*16-8,y-16,10, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
        img.draw(index, x-16-length*16+8,y-16,9, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
    }
    if ((canShoot)&&(isMagnet)){
        img.draw(index, x+16+length*16-8,y-16,11, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
        img.draw(index, x-16-length*16+8,y-16,12, true, 1.0f, 1.0f, 0.0f, COLOR(1,1,1,alpha), COLOR(1,1,1, alpha));
    }
}
