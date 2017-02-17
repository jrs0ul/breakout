#ifndef BALL_H
#define BALL_H

#include "TextureLoader.h"
#include "map.h"
#include "Vectors.h"

enum types{FRONT,RIGHT,LEFT};

struct ColidedBrick{
	int x;
	int y;
	bool colided;

	ColidedBrick(){
		x=0; y=0;
		colided=false;
	}
	ColidedBrick(int dx, int dy, bool c){
		x=dx; y=dy; colided=c;
	}

};
//-----------------------------
class Ball{
	public:
	float x;
	float y;
	float angle;
	bool moving;
	float speed;


	void setxy(float dx, float dy);
	bool move(float dx, float dy, BreakOutMap* map, ColidedBrick* kalad,bool reflectbricks);
	void setangle(float _angle);
	void draw(PicsContainer& spraitas, unsigned index);
	void fall(float dy);
	bool colidepadd(float dx, float dy, int padx, int pady, int padlen);
private:
	Vector3D reflection(float dx, float dy, unsigned char ** map, int type);

};


#endif //BALL_H
