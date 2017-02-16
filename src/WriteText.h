#ifndef _WRITE_TEXT_H
#define _WRITE_TEXT_H

#include "Picture3d.h"


void WriteText(unsigned int x,unsigned int y,LPD3DXSPRITE& spraitas, Picture* font, const char* s,float alpha=1.0f,
			   float scalex=1.0f, float scaley=1.0f, float r=1.0f,float g=1.0f,float b=1.0f);

void DrawNumber(int x, int y,int num,LPD3DXSPRITE& spraitas, Picture* font);



#endif //_WRITE_TEXT_H
