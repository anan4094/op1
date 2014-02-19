#include "OP3.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
GLfloat g_cubeVertices[20]={
	0,0,-0.5f,-0.5f,.5f,
	1,0,0.5f,-0.5f,.5f,
	1,1,.5f,.5f,.5f,
	0,1,-0.5f,.5f,.5f,
};
GLfloat mat_sp[]={1.0,1.0,1.0,1.0};
GLfloat mat_sh[]={50.0};
//���ù�Դ����
GLfloat light_ambient[] = { 1.0 , 1.0 , 1.0 , 0.0 };
GLfloat light_diffuse[] = { 1.0 , 1.0 , 1.0 , 1.0 };
//ָ����Դ��λ��
GLfloat light_position[] = { 0.0 , 0.0 , 1.0 , 1.0 };
GLfloat lmodel_a[]={0.1,0.1,0.1,1.0};

void OP3::Initialize(const char * title){
	base::Initialize();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, .3, 40);
	xrot = 0;
	InitFBO();
	glLightfv(GL_LIGHT0 , GL_AMBIENT , light_ambient);
	glLightfv(GL_LIGHT0 , GL_DIFFUSE , light_diffuse);
	glLightfv(GL_LIGHT0 , GL_POSITION , light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_a); //�趨ȫ�ֻ�����
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE , GL_FALSE);
	glEnable(GL_DEPTH_TEST);
}

void OP3::InitFBO(){
      //��һ��framebuffer object 
      glGenFramebuffersEXT(1,&g_framebuffer);
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,g_framebuffer);
 
      //����һ��RGBA��ͼ�ռ��FBO��ͼʹ��     
      glGenTextures(1,&g_texture);
      glBindTexture(GL_TEXTURE_2D,g_texture);
      //����filter
     glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
     glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     //������ͼ��С����ʽ����ռ�
     glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,256,256,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
     //framebuffer��RGBA��ͼ-��������FBO
     glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,g_texture,0);
     //����zbuffer��FBO ʹ�� 
     glGenRenderbuffersEXT(1,&g_depthbuffer);
     glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,g_depthbuffer);
     glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT24,256,256);
     glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,g_depthbuffer);
     GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
     switch( status ){
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            //MessageBox(NULL,TEXT("GL_FRAMEBUFFER_COMPLETE_EXT!"),TEXT("SUCCESS"),MB_OK|MB_ICONEXCLAMATION);
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            //MessageBox(NULL,TEXT("GL_FRAMEBUFFER_UNSUPPORTED_EXT!"),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
            exit(0);
            break;
     }
}

void OP3::Display(bool auto_redraw){
	//ʹ��g_framebuffer FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,g_framebuffer);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,256,256);
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -8.0f );
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_sp);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_sh);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glRotatef(xrot,0.0,1.0,0.0);
	xrot+=.08f;
	glutSolidTeapot(2);   
	glPopAttrib();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);     
	//  glViewport( 0, 0, width, height);
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    
	glTranslatef(0,0,-5);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,g_texture);
	glScalef(2,2,2);
	glRotated (xrot, 0.0f, 1.0f, 0.0f);    
	glInterleavedArrays( GL_T2F_V3F, 0, g_cubeVertices );
	glDrawArrays( GL_QUADS, 0, 4 );
	base::Display(auto_redraw);
}

void OP3::Finalize(void)
{
}

void OP3::Reshape(int width, int height)
{
    glViewport(0, 0 , width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, ((GLfloat)width)/height, .3, 40);
}