#ifndef BBULLETS_H
#define BBULLETS_H

#include "DLinkedList.h"
#include "TextureLoader.h"


struct Bullet{
	int x;
	int y;
};

class BulletContainer{

 DLinkedList<Bullet> bullets;
 DLinkedListIterator<Bullet> iterator;
public:
	BulletContainer(){
	}
	void add(int x, int y);
	void draw(PicsContainer& pics, unsigned index);
	void iteratorReset();
	Bullet* getCurrent();
	bool iteratorGoNext();
	bool iteratorGoPrev();
	void removeCurrent();
	void destroy();
};




#endif //BBULETS_H
