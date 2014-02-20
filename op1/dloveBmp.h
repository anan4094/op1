#ifndef DLOVE_BMP_H
#define DLOVE_BMP_H

namespace dlove{

typedef unsigned int uint32; 
typedef unsigned char uint8;

class Bmp{
	uint32 	height;					//高度
	uint32 	width;					//宽度
	uint32 	size;						//总大小
	uint32 	xpelsPerMeter;			//水平分辨率
	uint32 	ypelsPerMeter;			//竖直分辨率
	uint8 	alignBytes;				//每一行为了是4的倍数而补充的字节
	mutable uint8 	rgbs[3];					//getPixelAt()把返回值放在这里,setPixelAt()从这里读取
	uint8 	*rgbArray;
	uint32 	bytesArow;				//每一行的字节数

public:
	enum Attri{HEIGHT,WIDTH,SIZE,XPELS,YPELS};
	
public:
	Bmp(uint32 width,uint32 height,uint32 xpels,uint32 ypels);
	Bmp(const char *filename);		
	
	~Bmp();
	
	uint8 *getPixelAt(int x,int y)const;
	void setPixelAt(uint8 *rgbs,int x,int y);	
	
	uint32 get(Attri attr)const;
	void store(const char *storein)const;

private:
	inline static uint8 getAlignBytes(uint32 _width) {
		return 4-(_width*3 & 0x03) & 0x03;
	}//计算每一行像素为了是4的倍数而补加的字节；

	inline uint32 getIndex(int x,int y)const{
		//返回bmp图像上，坐标在(x,y)处的像素点在 rgbArray 中的下标；
		return (height-(++y))*bytesArow+x*3;
	}
};

}


#endif
