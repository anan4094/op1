#include "OP1.h"
#include <stdio.h>
#include <math.h>

#define X .525731112119133606
#define Z .850650808352039932

void OP1::Initialize(const char * title){
	base::Initialize();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, .3, 40);
}

void normalize(float v[3]){
	GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if(d == 0.0){
		return;
	}
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}

void normcrossprod(float v1[3],float v2[3],float out[3]){
	out[0] = v1[1]*v2[2] - v1[2]*v2[1];
	out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0];
	normalize(out);
}

void drawtrianle(float *v1,float *v2,float *v3){
	GLfloat d1[3],d2[3],norm[3];
	for (int j = 0;j < 3; j++){
		d1[j] = v1[j] - v2[j];
		d2[j] = v2[j] - v3[j];
	}
	normcrossprod(d1,d2,norm);
	glBegin(GL_TRIANGLES);
	glNormal3fv(norm);
	glVertex3fv(v1);
	glNormal3fv(norm);
	glVertex3fv(v2);
	glNormal3fv(norm);
	glVertex3fv(v3);
	glEnd();
}

void subdivide(float *v1,float *v2,float *v3,int depth){
	if (depth == 0){
		drawtrianle(v1,v2,v3);
		return;
	}
	GLfloat v12[3],v23[3],v31[3];
	GLint i;
	for (i = 0; i < 3; i++){
		v12[i] = (v1[i]+v2[i])/2.0;
		v23[i] = (v2[i]+v3[i])/2.0;
		v31[i] = (v3[i]+v1[i])/2.0;
	}
	normalize(v12);
	normalize(v23);
	normalize(v31);
	subdivide(v1,v12,v31,depth-1);
	subdivide(v2,v23,v12,depth-1);
	subdivide(v3,v31,v23,depth-1);
	subdivide(v12,v23,v31,depth-1);
}

void OP1::Display(bool auto_redraw){
	static GLfloat angle = 0;
	angle += 0.06f;
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//清空屏幕上的颜色
    glColor3f (1.0, 1.0, 0.0);//设置当前画笔颜色
	
	static const GLfloat light_position[] = {1.0f, -1.0f, -1.0f, 1.0f};
    static const GLfloat light_ambient[]   = {0.1f, 0.1f, 0.1f, 1.0f};
    static const GLfloat light_diffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f};
    static const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION,   light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,   light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,   light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
	static GLfloat vdata[12][3] = {
		{-X,0.0,Z},{ X,0.0,Z},{-X,0.0,-Z},{X,0.0,-Z},
		{0.0,Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
		{Z,X,0.0},{-Z,X,0.0},{Z,-X,0.0},{-Z,-X,0.0}
	};

	static GLuint tindices[20][3] = {
		{1,4,0},{4,9,0},{4,5,9},{8,5,4},{1,8,4},
		{1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
		{3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0},
		{10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}
	};
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 4, 0, 0, 0, 0, 1, 0);
	glPushMatrix();
	glTranslatef(-1.2,0,0);
	glScalef(.6f,.6f,.6f);
	glRotatef(angle,1,1,1);
    /*glBegin(GL_TRIANGLES);
	for (int i = 0; i < 20; i++){
		GLfloat d1[3],d2[3],norm[3];
		for (int j = 0;j < 3; j++){
			d1[j] = vdata[tindices[i][0]][j] - vdata[tindices[i][1]][j];
			d2[j] = vdata[tindices[i][1]][j] - vdata[tindices[i][2]][j];
		}
		normcrossprod(d1,d2,norm);
		glNormal3fv(norm);
		glVertex3fv(&vdata[tindices[i][0]][0]);
		glNormal3fv(norm);
		glVertex3fv(&vdata[tindices[i][1]][0]);
		glNormal3fv(norm);
		glVertex3fv(&vdata[tindices[i][2]][0]);
	}
	glEnd();*/
	for (int i = 0; i < 20; i++){
		subdivide(&vdata[tindices[i][0]][0],&vdata[tindices[i][1]][0],&vdata[tindices[i][2]][0],3);
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.2,0,0);
	glScalef(.6f,.6f,.6f);
	glRotatef(-angle,1,1,1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,vdata);
	glNormalPointer(GL_FLOAT,0,vdata);
	glDrawElements(GL_TRIANGLES,60,GL_UNSIGNED_INT,tindices);
	glPopMatrix();
	glRotatef(2*angle,1,2,-1);
	base::Display(auto_redraw);
}

void OP1::Finalize(void)
{
}

void OP1::Reshape(int width, int height)
{
    glViewport(0, 0 , width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, ((GLfloat)width)/height, .3, 40);
}