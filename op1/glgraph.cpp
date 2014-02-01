#include "glgraph.h"
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
				*pix = 255;
				pix++;
				*pix = 255;
				pix++;
				*pix = 255;
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
				*pix = 255;
				pix++;
				*pix = 255;
				pix++;
				*pix = 255;
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

void ptriangle(unsigned char*map,int w,int h,int *vs){
	pline(map,w,h,vs[0],vs[1],vs[2],vs[3]);
	pline(map,w,h,vs[2],vs[3],vs[4],vs[5]);
	pline(map,w,h,vs[4],vs[5],vs[0],vs[1]);
}

void ptriangles(unsigned char*map,int w,int h,int *_vs,int size){
	int *vs = _vs;
	for (int i = 0; i < size; i++){
		pline(map,w,h,vs[0],vs[1],vs[2],vs[3]);
		pline(map,w,h,vs[2],vs[3],vs[4],vs[5]);
		pline(map,w,h,vs[4],vs[5],vs[0],vs[1]);
		vs+=6;
	}
}