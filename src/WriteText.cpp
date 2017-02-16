#include "WriteText.h"

void WriteText(unsigned int x,unsigned int y, 
			   LPD3DXSPRITE& spraitas, Picture* font,
			   const char* s,
			   float alpha,
			   float scalex, float scaley,
			   float r, float g, float b){

 for (unsigned int i=0;i<strlen(s);i++){
  font->Blt(spraitas,x+(int)(11*scaley)*i,y,s[i]-32,alpha,scalex,scaley,0,r,g,b);
 }
}

 //--------------------------------------
 void DrawNumber(int x, int y,int num,LPD3DXSPRITE& spraitas, Picture* font){
	int arr[7]={0};

	int i=6;
	while (num!=0){
		arr[i]=num%10;
		num=num/10;
		i--;
	}

	for (int a=0; a<7; a++)
		font->Blt(spraitas,x+a*16,y,arr[a],0.6f,0.98f,0.98f); 




 
}