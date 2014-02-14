#ifndef __OP1_H__
#define __OP1_H__
#include "vapp.h"

BEGIN_APP_DECLARATION(OP1,"opengl demo 1")
	virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);
END_APP_DECLARATION()
#endif