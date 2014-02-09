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
void convertByMatrix3(pmatrix3 m1,vertex *vt,vertex *ds,int size,effon eff){
	float tmp[4],f[]={0,0,0};
	float *src=vt->_vertex.f,*dst=ds->_vertex.f;
	if (eff==all){
		tmp[0]=f[0]*m1->m[0][0]+f[1]*m1->m[1][0]+f[2]*m1->m[2][0]+m1->m[3][0];
		tmp[1]=f[0]*m1->m[0][1]+f[1]*m1->m[1][1]+f[2]*m1->m[2][1]+m1->m[3][1];
		tmp[2]=f[0]*m1->m[0][2]+f[1]*m1->m[1][2]+f[2]*m1->m[2][2]+m1->m[3][2];
		tmp[3]=f[0]*m1->m[0][3]+f[1]*m1->m[1][3]+f[2]*m1->m[2][3]+m1->m[3][3];
		f[0]=tmp[0]/tmp[3];
		f[1]=tmp[1]/tmp[3];
		f[2]=tmp[2]/tmp[3];
	}
	for (int i = 0; i < size; i++){
		tmp[0]=src[0]*m1->m[0][0]+src[1]*m1->m[1][0]+src[2]*m1->m[2][0]+m1->m[3][0];
		tmp[1]=src[0]*m1->m[0][1]+src[1]*m1->m[1][1]+src[2]*m1->m[2][1]+m1->m[3][1];
		tmp[2]=src[0]*m1->m[0][2]+src[1]*m1->m[1][2]+src[2]*m1->m[2][2]+m1->m[3][2];
		tmp[3] =src[0]*m1->m[0][3]+src[1]*m1->m[1][3]+src[2]*m1->m[2][3]+m1->m[3][3];
		dst[0]=tmp[0];
		dst[1]=tmp[1];
		dst[2]=tmp[2];
		dst[3]=tmp[3];
		if (eff==all){
			src=vt[i]._normal.f;
			dst=ds[i]._normal.f;
			tmp[0]=src[0]*m1->m[0][0]+src[1]*m1->m[1][0]+src[2]*m1->m[2][0]+m1->m[3][0];
			tmp[1]=src[0]*m1->m[0][1]+src[1]*m1->m[1][1]+src[2]*m1->m[2][1]+m1->m[3][1];
			tmp[2]=src[0]*m1->m[0][2]+src[1]*m1->m[1][2]+src[2]*m1->m[2][2]+m1->m[3][2];
			tmp[3]=src[0]*m1->m[0][3]+src[1]*m1->m[1][3]+src[2]*m1->m[2][3]+m1->m[3][3];
			dst[0]=tmp[0]/tmp[3]-f[0];
			dst[1]=tmp[1]/tmp[3]-f[1];
			dst[2]=tmp[2]/tmp[3]-f[2];
			ds[i]._color.r = vt[i]._color.r;
			ds[i]._color.g = vt[i]._color.g;
			ds[i]._color.b = vt[i]._color.b;
			ds[i]._color.a = vt[i]._color.a;
		}
		src = vt[i+1]._vertex.f;
		dst = ds[i+1]._vertex.f;
	}
}

void transition3(pmatrix3 m,float x,float y,float z){
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
	matrix3 tmp = {2.0f*n/(r-l),0,0,0
		,0,2.0f*n/(t-b),0,0
		,(l+r)/(l-r),(b+t)/(b-t),_a,1
		,0,0,_b,0};
	multiplyMatrix3(m,&tmp);
}

void viewport(vertex *v,pmatrix3 m,pviewpoint p,int w,int h,unsigned int size){
	for (unsigned int i = 0; i < size; i++){
		p[i]._original.x = v[i]._vertex.x;
		p[i]._original.y = v[i]._vertex.y;
		p[i]._original.z = v[i]._vertex.z;
	}
	convertByMatrix3(m,v,v,size,position);
	for (unsigned int i = 0; i < size; i++){
		p[i].x = (int)((v[i]._vertex.x/v[i]._vertex.w+1)*w/2);
		p[i].y = (int)((v[i]._vertex.y/v[i]._vertex.w+1)*h/2);
		p[i].z = v[i]._vertex.w;
		p[i].nx = v[i]._normal.x;
		p[i].ny = v[i]._normal.y;
		p[i].nz = v[i]._normal.z;
		p[i].d = v[i]._vertex.z/v[i]._vertex.w;
		p[i]._color.r = v[i]._color.r;
		p[i]._color.g = v[i]._color.g;
		p[i]._color.b = v[i]._color.b;
		p[i]._color.a = v[i]._color.a;
	}
}