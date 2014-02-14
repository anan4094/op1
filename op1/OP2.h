#ifndef __OP2_H__
#define __OP2_H__
#include "vapp.h"
#include "glgraph.h"
#include "glmatrix.h"

BEGIN_APP_DECLARATION(OP2,"opengl demo 2")
	virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);
	virtual void MainLoop(void);
	private:
		static void Keyboard(unsigned char key, int x, int y);
		int width;
		int height;
		matrix m;
		matrix3 m3;
		matrix3 m3p;
END_APP_DECLARATION()
#endif