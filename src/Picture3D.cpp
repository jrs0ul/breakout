#include "picture3D.h"
#include "stdio.h"

/*
jrs0ul
2007 04 17
*/

	

    int AvailSysMem(void) 
	{ 	
	MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	return (int)stat.dwAvailPhys;
	}

//=========================================================================


Picture::Picture(){
	
	tilewidth=0;
	tileheight=0;
	image=0;
	vframes=0;
	hframes=0;
}

void Picture::SetTileSize(int dx,int dy){
 tilewidth=dx;
 tileheight=dy;
 hframes=info.Width/tilewidth;
 vframes=info.Height/tileheight;
 SetRect(&prect,0,0,tilewidth,tileheight);
}


//---------------
bool Picture::Load(LPDIRECT3DDEVICE9& D3DDevice, const char* pic){
 
 if (FAILED(D3DXCreateTextureFromFileEx(D3DDevice,pic,
		                                      D3DX_DEFAULT,D3DX_DEFAULT,4,0,
											  D3DFMT_A8R8G8B8,
											  D3DPOOL_MANAGED,
											  D3DX_FILTER_LINEAR,
											  D3DX_DEFAULT,0,&info,NULL,&image)))
		return false;
  
  return true;
}

//--------------------------
void Picture::Blt(LPD3DXSPRITE& spraitas, int x, int y, int frame,float alpha,
		   float scalex, float scaley,float angle,float r,float g,float b, bool center ){


        D3DXVECTOR3 vec;
		vec.x=x*1.0f;
		vec.y=y*1.0f;
		vec.z=0.0f;
		RECT rec;

	

 
	 
		int hf=frame/(info.Width/tilewidth);
        int numx=frame-(info.Width/tilewidth)*hf;
        int numy=hf;
		D3DXVECTOR2 vscale=D3DXVECTOR2(scalex,scaley);
		
		SetRect(&rec,tilewidth*numx,tileheight*numy,tilewidth*(numx+1),tileheight*(numy+1));
		D3DXCOLOR col=D3DXCOLOR(r,g,b,alpha);

		D3DXVECTOR3 rcenter=D3DXVECTOR3(tilewidth/2.0f,tileheight/2.0f,0.0f);

		D3DXMATRIX rot;
		D3DXMATRIX scale;
        D3DXMATRIX trans; 
		

		D3DXMatrixRotationZ(&rot,-angle);
	    if (!center)
		 D3DXMatrixTranslation(&trans,(float)x+(tilewidth*scalex)/2.0f,
			                         (float)y+(tileheight*scaley)/2.0f,1.0f);
		else
		 D3DXMatrixTranslation(&trans,(float)x,
			                         (float)y,1.0f);
		D3DXMatrixScaling(&scale,scalex,scaley,1.0f);
		
       
		
        
		spraitas->SetTransform(&(scale*rot*trans));
 
 
		spraitas->Draw(image,&rec,&rcenter,0,col);
		spraitas->SetTransform(&rot);  


}
//------------

void Picture::Clear(){

	if (image!=NULL){
		
		image->Release();
        image=0;
	} 

}

int Picture::TotalFrames(){
	if ((tilewidth!=0)&&(tileheight!=0))
      if ((tilewidth<=info.Width)&&(tileheight<=info.Height))
		return (info.Width/tilewidth)*(info.Height/tileheight);
	return 1;
}
//=======================================

bool PictureContainer::initContainer(const char* path){

  FILE* failas=fopen(path,"rt");
   
  if (failas!=0) {
     

	 while (!feof(failas)){ 
	  PicData data;
	  data.name[0]=0;
	  
	  fscanf(failas,"%s\n",data.name);
      fscanf(failas,"%d %d\n",&data.theight,&data.twidth);
	  list.add(data);
	 }
     
   
    fclose(failas);
	return true;
  }
  return false;
}
//--------------
bool PictureContainer::loadpicture(LPDIRECT3DDEVICE9& D3DDevice,int index){
	char buf[255];
	strcpy(buf,"./pics/");
	strcat(buf,list[index].name);
	
	Picture pic;
	images.add(pic);

	if (!images[images.count()-1].Load(D3DDevice,buf))
	 return false;
	

	if(list[images.count()-1].theight)
	 images[images.count()-1].SetTileSize(list[images.count()-1].twidth,list[images.count()-1].theight);
	return true;
}


Picture* PictureContainer::findpic(const char* name){
 int index=0;
 while ((strcmp(name,list[index].name)!=0)&&(index<images.count()))
  index++;
 if (index<images.count())
  return &images[index];
 return 0;
}