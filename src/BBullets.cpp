#include "BBullets.h"

void BulletContainer::draw(PicsContainer& pics, unsigned index){
    DLinkedListIterator<Bullet> iter;
    iter.reset(&bullets);
    Bullet* b=0;
    b=iter.getvalue();
    bool isNext=false;
    if (b)
        isNext=true;
    while (isNext){
        pics.draw(index, b->x, b->y, 0, true, 1.0f, 1.0f, 0.0f, COLOR(1.0,0.0,0.0), COLOR(1.0,0.0,0.0));
        isNext=iter.nextStep();
        b=iter.getvalue();
    }

}
//-----------------------------------
void BulletContainer::add(int x, int y){
    Bullet b;
    b.x=x; b.y=y;
    bullets.add(b);
}
//----------------------------------

void BulletContainer::iteratorReset(){
    iterator.reset(&bullets);
}
//--------------------------------------
bool BulletContainer::iteratorGoNext(){
    return iterator.nextStep();
}
//---------------------------------------
bool BulletContainer::iteratorGoPrev(){
    return iterator.prevStep();
}
//--------------------------------------
Bullet* BulletContainer::getCurrent(){
    return iterator.getvalue();
}
//------------------------------------
void BulletContainer::removeCurrent(){

    iterator.removeThis();
}
//-----------------------------
void BulletContainer::destroy(){
    bullets.destroy();
}
