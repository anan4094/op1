#include "OP2.h"
#include <stdio.h>
#include <math.h>
#define TRA_NUM 12
GLubyte* colorBuffer = 0;

float* depthBuffer = 0;

int vertexs[] = {-50,50,50,50,-50,-50};
int vertexstmp[6] = {0};

vertex cube[] = {
	{-50,50,50,1,1,0,0,1,0,0,1},{50,50,50,1,1,0,0,1,0,0,1},{-50,-50,50,1,1,0,0,1,0,0,1}
	,{-50,-50,50,1,1,0,0,1,0,0,1},{50,50,50,1,1,0,0,1,0,0,1},{50,-50,50,1,1,0,0,1,0,0,1}//back
	,{-50,50,50,1,.5f,0,0,1,0,1,0},{50,50,-50,1,.5f,0,0,1,0,1,0},{50,50,50,1,.5f,0,0,1,0,1,0}
	,{-50,50,50,1,.5f,0,0,1,0,1,0},{50,50,-50,1,.5f,0,0,1,0,1,0},{-50,50,-50,1,.5f,0,0,1,0,1,0}//top
	,{-50,50,-50,1,1,.5f,0,1,0,0,-1},{50,50,-50,1,1,.5f,0,1,0,0,-1},{-50,-50,-50,1,1,.5f,0,1,0,0,-1}
	,{-50,-50,-50,1,1,.5f,0,1,0,0,-1},{50,50,-50,1,1,.5f,0,1,0,0,-1},{50,-50,-50,1,1,.5f,0,1,0,0,-1}//front
	,{-50,-50,50,1,1,1,0,1,0,-1,0},{50,-50,-50,1,1,1,0,1,0,-1,0},{50,-50,50,1,1,1,0,1,0,-1,0}
	,{-50,-50,50,1,1,1,0,1,0,-1,0},{50,-50,-50,1,1,1,0,1,0,-1,0},{-50,-50,-50,1,1,1,0,1,0,-1,0}//bottom
	,{-50,-50,50,1,0,1,1,1,-1,0,0},{-50,50,50,1,0,1,1,1,-1,0,0},{-50,50,-50,1,0,1,1,1,-1,0,0}
	,{-50,-50,-50,1,0,1,1,1,-1,0,0},{-50,-50,50,1,0,1,1,1,-1,0,0},{-50,50,-50,1,0,1,1,1,-1,0,0}//left
	,{50,-50,50,1,0,1,.5f,1,1,0,0},{50,50,50,1,0,1,.5f,1,1,0,0},{50,50,-50,1,0,1,.5f,1,1,0,0}
	,{50,-50,-50,1,0,1,.5f,1,1,0,0},{50,-50,50,1,0,1,.5f,1,1,0,0},{50,50,-50,1,0,1,.5f,1,1,0,0}//right
};

vertex cubetmp[3*TRA_NUM]={0};
viewpoint viewpoints[TRA_NUM*3]={0};
float stamp = 0;
volatile int _status;
const float sqrtthird = .57735f;

void OP2::Initialize(const char * title){
	base::Initialize();
	glutKeyboardFunc(Keyboard);
	_status = 1;
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glTranslatef(0,0,-0.5);
	glDepthFunc(GL_LEQUAL);
	//colorBuffer = new GLubyte[width*height*3];
    //memset(colorBuffer, 0, width*height*3);
	glClearColor(0,0,0,1);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	loadIdentityMatrix(&this->m);
	loadIdentityMatrix3(&this->m3p);
	perspective(&this->m3p,290,410,-300,300,300,-300);
}
void OP2::Keyboard(unsigned char key, int x, int y){
	if(key == 'Q' || key == 'q'|| key == '\27'|| key == '\x1b'){
		_status = 0;
	}
}

void OP2::Display(bool auto_redraw){
	//glReadBuffer(GL_FRONT);
	//glReadPixels(0,0,480,480,GL_RGB,GL_UNSIGNED_BYTE, colorBuffer);
	//glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glRasterPos2i(0, 0);
	register GLubyte*tmp = colorBuffer;
	unsigned char c = 0;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (i==0||i==height-1||j==0||j==width-1){
				c = 255;
			}else{
				c = 0;
			}
			*tmp=c;
			tmp++;
			*tmp=c;
			tmp++;
			*tmp=c;
			tmp++;
		}
	}
	for(int i=0;i<width*height;i++){
		depthBuffer[i]=9999;
	}
	//2d test
	stamp+=.02f;
	/*loadIdentityMatrix(&this->m);
	rotate(&this->m,stamp);
	transition(&this->m,240,240);
	convertByMatrix(&this->m,vertexs,vertexstmp);
	convertByMatrix(&this->m,vertexs+2,vertexstmp+2);
	convertByMatrix(&this->m,vertexs+4,vertexstmp+4);
	ptriangle(colorBuffer,width,height,vertexstmp);*/

    //3d test
	loadIdentityMatrix3(&this->m3);
	rotate3(&this->m3,sqrtthird,sqrtthird,sqrtthird,-stamp);
	transition3(&this->m3,0,0,350);
	
	
	convertByMatrix3(&this->m3,cube,cubetmp,3*TRA_NUM);
	viewport(cubetmp,&this->m3p,viewpoints,width,height,3*TRA_NUM);
	ptriangles(colorBuffer,depthBuffer,width,height,viewpoints,TRA_NUM);

	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer);
	base::Display(auto_redraw);
}

void OP2::MainLoop(void){
	for (;_status;)
        glutMainLoopEvent(); 
}

void OP2::Finalize(void){
	delete[] colorBuffer;
	delete[] depthBuffer;
}

void OP2::Reshape(int width, int height){
	this->width = width-8;
	this->height = height-8;
    glViewport(0, 0 , width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	if (colorBuffer){
		delete[] colorBuffer;
		colorBuffer = 0;
	}
	if (depthBuffer){
		delete[] depthBuffer;
		depthBuffer = 0;
	}
	colorBuffer = new GLubyte[width*height*3];
	depthBuffer = new float[width*height];
}