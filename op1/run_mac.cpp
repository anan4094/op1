#include <GLUT/GLUT.h>

#include "glgraph.h"
#include "glmatrix.h"
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
volatile int _status = 1;
const float sqrtthird = .57735f;


int width  = 480;
int height = 480;
matrix m;
matrix3 m3;
matrix3 m3p;

void keyboard(unsigned char key, int x, int y){
	if(key == 'Q' || key == 'q'|| key == '\27'|| key == '\x1b'){
		_status = 0;
	}
}

void reshape(int width_, int height_){
	width = width_;
	height = height_;
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

void display(){
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
	stamp+=.02f;
//	loadIdentityMatrix(&m);
//    rotate(&m,stamp);
//    transition(&m,240,240);
//    convertByMatrix(&m,vertexs,vertexstmp);
//    convertByMatrix(&m,vertexs+2,vertexstmp+2);
//    convertByMatrix(&m,vertexs+4,vertexstmp+4);
//    ptriangle(colorBuffer,width,height,vertexstmp);
    
	loadIdentityMatrix3(&m3);
	rotate3(&m3,sqrtthird,sqrtthird,sqrtthird,-stamp);
	transition3(&m3,0,0,350);
	
	
	convertByMatrix3(&m3,cube,cubetmp,3*TRA_NUM);
	viewport(cubetmp,&m3p,viewpoints,width,height,3*TRA_NUM);
	ptriangles(colorBuffer,depthBuffer,width,height,viewpoints,TRA_NUM);
    
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, colorBuffer);
	glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char ** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(480, 400);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("OpenGL Application");
    
    glClearColor(0,0,0,1);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	loadIdentityMatrix(&m);
	loadIdentityMatrix3(&m3p);
	perspective(&m3p,290,410,-300,300,300,-300);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    for (;_status;)
        glutCheckLoop();
    delete []colorBuffer;
    delete []depthBuffer;
}