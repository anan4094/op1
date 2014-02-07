#ifndef __OP_GRAPH__
#define __OP_GRAPH__
#include "glmatrix.h"
void psetcolor(char red,char green,char blue);
void pline(unsigned char*map,int w,int h,int x1,int y1,int x2,int y2);
void ptriangle(unsigned char*map,int w,int h,int *vs);
void ptriangles(unsigned char*map,float *dem,int w,int h,pviewpoint _vs,int size);
void polyFill(unsigned char*map,float *dem,int w,int h,pviewpoint o,int length = 3);
#endif
