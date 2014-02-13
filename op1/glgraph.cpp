#include "glgraph.h"
#include <math.h>
#include <stdio.h>

#ifdef WIN32
#define ABS abs
#else
#define ABS fabs
#endif

char g_red = '\255',g_green = '\255',g_blue = '\255';
void psetcolor(char red,char green,char blue){
	g_red = red;
	g_green = green;
	g_blue = blue;
}

inline bool is_zero(float f)
{
	if (ABS(f) < 1e-8)
		return true;
	return false;
}

void pline(unsigned char*map,int w,int h,int x1,int y1,int x2,int y2){
	int dx = x2 - x1
		,dy = y2 - y1
		,d
		,x = x1
		,y = y1
		,xi = 1
		,yi = 1
		;
	if( dy < 0){
		yi = -1;
		dy = -dy;
	}
	if( dx < 0){
		xi = -1;
		dx = -dx;
	}
	if(dy <= dx){
		d = (dy<<1) - dx;
		x=x1;
		while(x!=x2){
			if(!(y>=h||y<0)&&!(x>=w||x<0)){
				unsigned char*pix = &map[(y*w+x)*3];
				*pix = g_red;
				pix++;
				*pix = g_green;
				pix++;
				*pix = g_blue;
			}
			if(d>0){
				y += yi;
				d = d + (dy<<1) - (dx<<1);
			}else{
				d = d + (dy<<1);
			}
			x+=xi;
		}
	}else{
		d = (dx<<1) - dy;
		y=y1;
		while(y!=y2){
			if(!(y>=h||y<0)&&!(x>=w||x<0)){
				unsigned char*pix = &map[(y*w+x)*3];
				*pix = g_red;
				pix++;
				*pix = g_green;
				pix++;
				*pix = g_blue;
			}
			if(d>0){
				x += xi;
				d = d + (dx<<1) - (dy<<1);
			}else{
				d = d + (dx<<1);
			}
			y+=yi;
		}
	}
}

void plineByDepth(unsigned char*map,float *dem,int w,int h,pviewpoint beg,pviewpoint end){
	int x1=beg->x,y1=beg->y,x2=end->x,y2=end->y;
	float z1=beg->z,z2=end->z;
	int dx = x2 - x1
		,dy = y2 - y1
		,d
		,x = x1
		,y = y1
		,xi = 1
		,yi = 1
		;
	if( dy < 0){
		yi = -1;
		dy = -dy;
	}
	if( dx < 0){
		xi = -1;
		dx = -dx;
	}
	if(dy <= dx){
		float dz = (z2-z1)/dx; 
		d = (dy<<1) - dx;
		x=x1;
		while(x!=x2){
			if(!(y>=h||y<0)&&!(x>=w||x<0)){
				if(z1<dem[y*w+x]){
					unsigned char*pix = &map[(y*w+x)*3];
					*pix = g_red;
					pix++;
					*pix = g_green;
					pix++;
					*pix = g_blue;
					dem[y*w+x]=z1;
				}
			}
			if(d>0){
				y += yi;
				d = d + (dy<<1) - (dx<<1);
			}else{
				d = d + (dy<<1);
			}
			x+=xi;
			z1+=dz;
		}
	}else{
		float dz = (z2-z1)/dy; 
		d = (dx<<1) - dy;
		y=y1;
		while(y!=y2){
			if(!(y>=h||y<0)&&!(x>=w||x<0)){
				if(z1<dem[y*w+x]){
					unsigned char*pix = &map[(y*w+x)*3];
					*pix = g_red;
					pix++;
					*pix = g_green;
					pix++;
					*pix = g_blue;
					dem[y*w+x]=z1;
				}
			}
			if(d>0){
				x += xi;
				d = d + (dx<<1) - (dy<<1);
			}else{
				d = d + (dx<<1);
			}
			y+=yi;
			z1+=dz;
		}
	}
}

void ptriangle(unsigned char*map,int w,int h,int *vs){
	pline(map,w,h,vs[0],vs[1],vs[2],vs[3]);
	pline(map,w,h,vs[2],vs[3],vs[4],vs[5]);
	pline(map,w,h,vs[4],vs[5],vs[0],vs[1]);
}

void ptriangles(unsigned char*map,float *dem,int w,int h,pviewpoint vs,int size){
	for (int i = 0; i < size; i++){
		plineByDepth(map,dem,w,h,&vs[0],&vs[1]);
		plineByDepth(map,dem,w,h,&vs[1],&vs[2]);
		plineByDepth(map,dem,w,h,&vs[2],&vs[0]);
		polyFill(map,dem,w,h,vs);
		vs+=3;
	}
}

//∂‡±ﬂ–Œ…®√Ë◊™ªª
void polyFill(unsigned char*map,float *dem,int w,int h, pviewpoint o,int length){
    int newc=0,delc=0;
	float light[3]={.57735f,-.57735f,-.57735f};
	float rate = o[0].nx*light[0]+o[0].ny*light[1]+o[0].nz*light[2];//∆Ωµ»π‚
	if (rate<0){
		rate=0;
	}
	rate +=.4f;//ª∑æ≥π‚
	if(rate>1){
		rate=1;
	}
	unsigned char col[3];
	col[0] = (unsigned char)255*o[0]._color.r*rate;
	col[1] = (unsigned char)255*o[0]._color.g*rate;
	col[2] = (unsigned char)255*o[0]._color.b*rate;

	float a,b,c,d;				// ∂‡±ﬂ–ŒÀ˘‘⁄µƒ∆Ω√Êµƒ∑Ω≥Ãœµ ˝ ax + by + cz + d = 0
	// º∆À„ ax + by + cz + d = 0
	a = (o[1].y-o[0].y)*(o[2].z-o[0].z)-(o[1].z-o[0].z)*(o[2].y-o[0].y);
	b = (o[1].z-o[0].z)*(o[2].x-o[0].x)-(o[1].x-o[0].x)*(o[2].z-o[0].z);
	c = (o[1].x-o[0].x)*(o[2].y-o[0].y)-(o[1].y-o[0].y)*(o[2].x-o[0].x);
	d = - (a * o[0].x + b * o[0].y + c * o[0].z );
	
	typedef struct XET{
		float x,dx,ymax;
		float z;						// ◊ÛΩªµ„¥¶∂‡±ﬂ–ŒÀ˘‘⁄∆Ω√Êµƒ…Ó∂»÷µ
		float dzx;					// —ÿ…®√ËœﬂœÚ”“◊ﬂπ˝“ª∏ˆœÒÀÿ ±, ∂‡±ﬂ–ŒÀ˘‘⁄∆Ω√Êµƒ…Ó∂»‘ˆ¡ø. ∂‡”⁄∆Ω√Ê∑Ω≥Ã, dzx = -a/c (c!= 0)
		float dzy;					// —ÿy∑ΩœÚœÚœ¬“∆π˝“ª∏˘…®√Ëœﬂ ±, ∂‡±ﬂ–ŒÀ˘‘⁄∆Ω√Êµƒ…Ó∂»‘ˆ¡ø. ∂‘”⁄∆Ω√Ê∑Ω≥Ã, dzy = b/c (c!= 0)
		XET * next;
	}AET,NET;
	int maxy = 0
		,miny = 1024
		,ia
		,ib
		;
	NET *pNet[1024],*p,*q,*r;
	AET *pAet = new AET;
    newc++;
	pAet->next = nullptr;
	//º∆À„◊Ó∏ﬂµ„∫Õ◊ÓµÕµ„
	for(int i = 0;i<length;i++){
		if(o[i].y>maxy){
			maxy = o[i].y;
		}
		if(o[i].y<miny){
			miny = o[i].y;
		}
	}
	//≥ı ºªØNET±Ì
	for(int i=miny;i<=maxy;i++){
		pNet[i] = new NET;
        newc++;
		pNet[i]->next = nullptr;
	}
	for(int i = 0;i<length;i++){
		p = pNet[o[i].y];
		ia = (i-1+length)%length;
		ib = (i+1)%length;
		if(o[ia].y > o[i].y){
			q = new NET;
            newc++;
			q->x = (float)o[i].x;
			q->ymax = (float)o[ia].y;
			q->dx = ((float)(o[ia].x - o[i].x))/(o[ia].y - o[i].y);
			q->next = p->next;
			pNet[o[i].y]->next = q;
		}
		if(o[ib].y > o[i].y){
			q = new NET;
            newc++;
			q->x = (float)o[i].x;
			q->ymax = (float)o[ib].y;
			q->dx = ((float)(o[ib].x - o[i].x))/(o[ib].y - o[i].y);
			q->next = p->next;
			pNet[o[i].y]->next = q;
		}
	}
	for(int i = miny;i<maxy;i++){
		p = pAet->next;
		//∏¸–¬Ωªµ„
		while(p){
			p->x = p->x + p->dx;
			p = p->next;
		}
		//∏¸–¬∫Û≈≈–Ú
		q = pAet;
		p = pAet->next;
		q->next = nullptr;
		while(p){
			while(q->next && p->x >= q->next->x){
				q = q->next;
			}
			r = p->next;
			p->next = q->next;
			q->next = p;
			p = r;
			q = pAet;
		}
		//…æ≥˝Ω·µ„
		q = pAet;
		p = q->next;
		while(p){
			if(p->ymax == i){
				q->next = p->next;
                delc++;
				delete p;
				p = q->next;
			}else{
				p = p->next;
				q = q->next;
			}
		}
		//ÃÌº”Ω·µ„
		p = pNet[i]->next;
		q = pAet;
		while (p){
			while (q->next && p->x >= q->next->x){
				q = q->next;
			}
			if (is_zero(c)) {
			//printf("TODO c == 0 !\n");
				p->z = 0;
				p->dzx	= 0;
				p->dzy	= 0;
			} else {
				p->z	= - (a*p->x + b*i + d) / c;  // ax + by + cz + d = 0;
				p->dzx	= - (a/c);
				p->dzy	= -(b/c);
			}
			r = p->next;
			p->next = q->next;
			q->next = p;
			p = r;
			q = pAet;
		}
		//◊≈…´
		p = pAet->next;
		while(p && p->next){
			ia = static_cast<int>(p->x);
			ib = static_cast<int>(p->next->x);
			float zx = p->z;
			for(int j = ia;j<=ib;j++){
				if(!(i>=h||i<0)&&!(j>=w||j<0)){
					if(zx<dem[i*w+j]){
						dem[i*w+j]= zx;
						unsigned char*pix = &map[(i*w+j)*3];
						*pix = col[0];
						pix++;
						*pix = col[1];
						pix++;
						*pix = col[2];
					}
				}
				zx += p->dzx;
			}
			// ∂‡±ﬂ–ŒÀ˘‘⁄µƒ∆Ω√Ê∂‘”¶œ¬“ªÃı…®√Ëœﬂ‘⁄x=xl¥¶µƒ…Ó∂»Œ™ z = z + dzl * dxl + dzy
			// ¥À¥¶øŒº˛÷–dzl∆‰ µ”¶Œ™dzx?
			p->z += p->dzx* p->dx +p->dzy;
			p->next->z += p->dzx* p->next->dx +p->next->dzy;
			p = p->next->next;
		}
	}
    for(int i=miny;i<=maxy;i++){
		delete pNet[i];
        delc++;
	}
    while (pAet->next) {
        NET*p=pAet->next,*q=pAet;
        while (p->next) {
            q = p;
            p = p->next;
        }
        delete p;
        delc++;
        q->next = nullptr;
    }
    delete pAet;
    delc++;
    if (newc!=delc) {
        printf("warn:memory leak\n");
    }
}