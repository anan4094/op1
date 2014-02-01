#include "glmatrix.h"
#include <math.h>

void loadIdentityMatrix(pmatrix m){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			if(i==j)m->m[i][j]=1;
			else m->m[i][j]=0;
		}
	}
}
void multiplyMatrix(pmatrix m1,pmatrix m2){
	float tmp[] = {0,0,0};
	for (int i = 0; i < 3; i++){
		tmp[0]=m1->m[i][0]*m2->m[0][0]+m1->m[i][1]*m2->m[1][0]+m1->m[i][2]*m2->m[2][0];
		tmp[1]=m1->m[i][0]*m2->m[0][1]+m1->m[i][1]*m2->m[1][1]+m1->m[i][2]*m2->m[2][1];
		tmp[2]=m1->m[i][0]*m2->m[0][2]+m1->m[i][1]*m2->m[1][2]+m1->m[i][2]*m2->m[2][2];
		m1->m[i][0]=tmp[0];
		m1->m[i][1]=tmp[1];
		m1->m[i][2]=tmp[2];
	}
}
void convertByMatrix(pmatrix m1,int *src,int *dst){
	dst[0]=src[0]*m1->m[0][0]+src[1]*m1->m[1][0]+m1->m[2][0];
	dst[1]=src[0]*m1->m[0][1]+src[1]*m1->m[1][1]+m1->m[2][1];
	float tmp =src[0]*m1->m[0][2]+src[1]*m1->m[1][2]+m1->m[2][2];
	dst[0]/=tmp;
	dst[1]/=tmp;
}

void transition(pmatrix m,int x,int y){
	matrix tmp={1,0,0,0,1,0,0,0,1};
	tmp.m[2][0]=x;
	tmp.m[2][1]=y;
	multiplyMatrix(m,&tmp);
}

void rotate(pmatrix m,float deg){
	matrix tmp={1,0,0,0,1,0,0,0,1};
	tmp.m[0][0]=cosf(deg);
	tmp.m[1][0]=-sinf(deg);
	tmp.m[0][1]=sinf(deg);
	tmp.m[1][1]=cosf(deg);
	multiplyMatrix(m,&tmp);
}

void loadIdentityMatrix3(pmatrix3 m){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if(i==j)m->m[i][j]=1;
			else m->m[i][j]=0;
		}
	}
}
void multiplyMatrix3(pmatrix3 m1,pmatrix3 m2){
	float tmp[] = {0,0,0,0};
	for (int i = 0; i < 4; i++){
		tmp[0]=m1->m[i][0]*m2->m[0][0]+m1->m[i][1]*m2->m[1][0]+m1->m[i][2]*m2->m[2][0]+m1->m[i][3]*m2->m[3][0];
		tmp[1]=m1->m[i][0]*m2->m[0][1]+m1->m[i][1]*m2->m[1][1]+m1->m[i][2]*m2->m[2][1]+m1->m[i][3]*m2->m[3][1];
		tmp[2]=m1->m[i][0]*m2->m[0][2]+m1->m[i][1]*m2->m[1][2]+m1->m[i][2]*m2->m[2][2]+m1->m[i][3]*m2->m[3][2];
		tmp[3]=m1->m[i][0]*m2->m[0][3]+m1->m[i][1]*m2->m[1][3]+m1->m[i][2]*m2->m[2][3]+m1->m[i][3]*m2->m[3][3];
		m1->m[i][0]=tmp[0];
		m1->m[i][1]=tmp[1];
		m1->m[i][2]=tmp[2];
		m1->m[i][3]=tmp[3];
	}
}
void convertByMatrix3(pmatrix3 m1,float *_src,float *_dst,int size){
	float *src=_src,*dst=_dst;
	for (int i = 0; i < size; i++){
		dst[0]=src[0]*m1->m[0][0]+src[1]*m1->m[1][0]+src[2]*m1->m[2][0]+m1->m[3][0];
		dst[1]=src[0]*m1->m[0][1]+src[1]*m1->m[1][1]+src[2]*m1->m[2][1]+m1->m[3][1];
		dst[2]=src[0]*m1->m[0][2]+src[1]*m1->m[1][2]+src[2]*m1->m[2][2]+m1->m[3][2];
		float tmp =src[0]*m1->m[0][3]+src[1]*m1->m[1][3]+src[2]*m1->m[2][3]+m1->m[3][3];
		dst[0]/=tmp;
		dst[1]/=tmp;
		dst[2]/=tmp;
		src += 3;
		dst += 3;
	}
}

void transition3(pmatrix3 m,int x,int y,int z){
	matrix3 tmp={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	tmp.m[3][0]=x;
	tmp.m[3][1]=y;
	tmp.m[3][2]=z;
	multiplyMatrix3(m,&tmp);
}

void rotate3(pmatrix3 m,float x,float y,float z,float deg){
	float sf=sinf(deg),cf=cosf(deg);
	matrix3 tmp={(1-cf)*x*x+cf,(1-cf)*x*y-sf*z,(1-cf)*x*z+sf*y,0
		,(1-cf)*x*y+sf*z,(1-cf)*y*y+cf,(1-cf)*y*z-sf*x,0
		,(1-cf)*x*z-sf*y,(1-cf)*y*z+sf*x,(1-cf)*z*z+cf,0
		,0,0,0,1};
	multiplyMatrix3(m,&tmp);
}

void perspective(pmatrix3 m,float n,float f,float l,float r,float t,float b){
	float _a = f/(f-n),_b = -n*f/(f-n);
	matrix3 tmp = {2.0*n/(r-l),0,0,0
		,0,2.0*n/(t-b),0,0
		,(l+r)/(l-r),(b+t)/(b-t),_a,1
		,0,0,_b,0};
	multiplyMatrix3(m,&tmp);
}

void viewport(float *v,int *p,int w,int h,unsigned int size){
	for (int i = 0; i < size; i++){
		p[2*i] = (int)((v[3*i]+1)*w/2);
		p[2*i+1] = (int)((v[3*i+1]+1)*h/2);
	}
}