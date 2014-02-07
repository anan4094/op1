#ifndef __OP_MATRIX__
#define __OP_MATRIX__
typedef union{
    struct
    {
        float  r,g,b,a;
    };
    float  f[4];
}color;
typedef struct vertex_
{
    union
    {
        struct
        {
            float  x,y,z,w;
        };
        float  f[4];
    }_vertex;
	color _color;
}vertex,*pvertex;

typedef struct matrix_{
	float m[3][3];
}matrix,*pmatrix;

typedef struct matrix3_{
	float m[4][4];
}matrix3,*pmatrix3;

typedef struct viewpoint_{
	int x;
	int y;
	float z;
	float d;
	color _color;
}viewpoint,*pviewpoint;

void loadIdentityMatrix(pmatrix m);
void multiplyMatrix(pmatrix m1,pmatrix m2);
void convertByMatrix(pmatrix m1,int *src,int *dst);
void transition(pmatrix m,int x,int y);
void rotate(pmatrix m,float deg);

void loadIdentityMatrix3(pmatrix3 m);
void multiplyMatrix3(pmatrix3 m1,pmatrix3 m2);
void convertByMatrix3(pmatrix3 m1,vertex *src,vertex *dst,int size);
void transition3(pmatrix3 m,float x,float y,float z);
void rotate3(pmatrix3 m,float x,float y,float z,float deg);
void perspective(pmatrix3 m,float n,float f,float l,float r,float t,float b);
void viewport(vertex *v,pviewpoint p,int w,int h,unsigned int size);
#endif