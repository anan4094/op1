#ifndef __OP3_H__
#define __OP3_H__
#include "vapp.h"

BEGIN_APP_DECLARATION(OP3,"opengl demo 3")
	virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);
	void InitFBO();
private:
	GLuint  g_framebuffer;
	GLuint g_texture;
	GLuint g_depthbuffer;
	GLfloat xrot;
END_APP_DECLARATION()
#endif