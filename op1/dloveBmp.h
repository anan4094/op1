#ifndef DLOVE_BMP_H
#define DLOVE_BMP_H

namespace dlove{

typedef unsigned int uint32; 
typedef unsigned char uint8;

class Bmp{
	uint32 	height;					//�߶�
	uint32 	width;					//���
	uint32 	size;						//�ܴ�С
	uint32 	xpelsPerMeter;			//ˮƽ�ֱ���
	uint32 	ypelsPerMeter;			//��ֱ�ֱ���
	uint8 	alignBytes;				//ÿһ��Ϊ����4�ı�����������ֽ�
	mutable uint8 	rgbs[3];					//getPixelAt()�ѷ���ֵ��������,setPixelAt()�������ȡ
	uint8 	*rgbArray;
	uint32 	bytesArow;				//ÿһ�е��ֽ���

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
	}//����ÿһ������Ϊ����4�ı��������ӵ��ֽڣ�

	inline uint32 getIndex(int x,int y)const{
		//����bmpͼ���ϣ�������(x,y)�������ص��� rgbArray �е��±ꣻ
		return (height-(++y))*bytesArow+x*3;
	}
};

}


#endif
