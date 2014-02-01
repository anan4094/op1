#ifndef __OP_MATRIX__
#define __OP_MATRIX__
typedef struct _matrix{
	float m[3][3];
}matrix,*pmatrix;

typedef struct _matrix3{
	float m[4][4];
}matrix3,*pmatrix3;

void loadIdentityMatrix(pmatrix m);
void multiplyMatrix(pmatrix m1,pmatrix m2);
void convertByMatrix(pmatrix m1,int *src,int *dst);
void transition(pmatrix m,int x,int y);
void rotate(pmatrix m,float deg);

void loadIdentityMatrix3(pmatrix3 m);
void multiplyMatrix3(pmatrix3 m1,pmatrix3 m2);
void convertByMatrix3(pmatrix3 m1,float *src,float *dst,int size);
void transition3(pmatrix3 m,int x,int y,int z);
void rotate3(pmatrix3 m,float x,float y,float z,float deg);
void perspective(pmatrix3 m,float n,float f,float l,float r,float t,float b);
void viewport(float *v,int *p,int w,int h,unsigned int size);
#endif