#include "dloveBmp.h"
#include <stdio.h>
#define mTostr(x) #x
#define mLink(x,y) x##y
using namespace dlove; 
#pragma warning(disable:4996)
Bmp::Bmp(uint32 _width,uint32 _height,uint32 _xpels,uint32 _ypels):
	width(_width),
	height(_height),
	xpelsPerMeter(_xpels),
	ypelsPerMeter(_ypels)
{
	this->alignBytes=( getAlignBytes(_width) );
	this->bytesArow=_width*3+this->alignBytes;

	int bmpPixelSize=this->bytesArow*height;
	this->size=bmpPixelSize+54;
	rgbArray= new uint8[bmpPixelSize];
}

#define mReadAttr(attr)	\
	fread((void*)&attr,sizeof attr,1,bmpfile);

#define mGoto(index)	\
	fseek(bmpfile,index,SEEK_SET);

Bmp::Bmp(const char *filename){
	FILE *bmpfile=fopen(filename,"rb");
	short bitCount;		//图像每个像素的位数
	uint32 compression;	//图像的压缩类型
	uint32 pixelSize;		//bmp中像素总大小

	mGoto(0x02);mReadAttr(size);
	mGoto(0x12);mReadAttr(width);mReadAttr(height);
	mGoto(0x1c);mReadAttr(bitCount);mReadAttr(compression);mReadAttr(pixelSize);
	mGoto(0x26);mReadAttr(xpelsPerMeter);mReadAttr(ypelsPerMeter);

	if(compression!=0)
		throw 0x001;
	if(bitCount!=24)
		throw 0x002;

	this->alignBytes=getAlignBytes(this->width);
	this->bytesArow=pixelSize/this->height;

	this->rgbArray=new uint8[pixelSize];
	mGoto(0x36);
	fread((void*)rgbArray,sizeof(uint8),pixelSize,bmpfile);

	fclose(bmpfile);
}

Bmp::~Bmp(){
	delete []this->rgbArray;
}

uint8* Bmp::getPixelAt(int x,int y)const{
	int index=getIndex(x,y);

//	printf("index: %u \n",index);	//debug
	for(register int ci(2);ci>=0;--ci,++index)
		*(rgbs+ci)=*(rgbArray+index);

	return rgbs;
}

void Bmp::setPixelAt(uint8 *_rgbs,int x,int y){
	int index=getIndex(x,y)+2;

	for(register int ci=2;ci>=0;--ci,++_rgbs,--index)
		*(rgbArray+index)=*_rgbs;

	return ;
}

uint32 Bmp::get(Attri attri)const{
	switch(attri){
	case WIDTH:return this->width;
	case HEIGHT:return this->height;
	case XPELS:return this->xpelsPerMeter;
	case YPELS:return this->ypelsPerMeter;
	case SIZE:return this->size;
	}
	return -1;
}

#define mDefine(type,num,value)	\
	type mLink(attri,num)=value;
#define mWriteNum(num)	\
	fwrite((void*)&(mLink(attri,num)),sizeof(mLink(attri,num)),1,bmpfile);
#define mWrite(attr)	\
	fwrite((void*)&attr,sizeof attr,1,bmpfile);

void Bmp::store(const char *filename)const{
	FILE *bmpfile=fopen(filename,"wb");
	mDefine(unsigned short,1,0x4d42);
	mDefine(unsigned short,3,0);
	mDefine(unsigned short,4,0);
	mDefine(uint32,5,0x0036);
	mDefine(uint32,6,0x0028);
	mDefine(unsigned short,9,1);
	mDefine(unsigned short,10,0x0018);
	mDefine(uint32,11,0);
	mDefine(uint32,15,0);
	mDefine(uint32,16,0);

	mWriteNum(1);
	mWrite(this->size);
	mWriteNum(3);
	mWriteNum(4);
	mWriteNum(5);
	mWriteNum(6);
	mWrite(this->width);
	mWrite(this->height);
	mWriteNum(9);
	mWriteNum(10);
	mWriteNum(11);

	int pixelSize=size-54;
	mWrite(pixelSize);

	mWrite(this->xpelsPerMeter);
	mWrite(this->ypelsPerMeter);
	mWriteNum(15);
	mWriteNum(16);

	fwrite((void*)rgbArray,sizeof(uint8),pixelSize,bmpfile);

	fclose(bmpfile);
	return ;
}
