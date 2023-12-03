#include "./BSP/TOUCH/touch.h"
#include "./BSP/LCD/lcd.h"
#include "lcd.h"
#include <math.h>

int state=0;

void ctp_init(){
	tp_dev.init(); 
}

double dist(int x,int y,int gx,int gy){
	return sqrt((x-gx)*(x-gx)+(y-gy)*(y-gy));
}

void ctp_scan(){
	tp_dev.scan(0);
	for(int i=0;i<5;i++){
		if ((tp_dev.sta) & (1 << i)){
			int x=tp_dev.x[i];
			int y=tp_dev.y[i];
			if(x<lcddev.width && y<lcddev.height){
				if(dist(x,y,70,185)<=40){
					state=1;
					start();
				}
				if(dist(x,y,70,90)<=40){
					state=0;
					stop();
				}
				if(dist(x,y,175,185)<=40){
					state=0;
					clear();
					stop();
				}
			}
		}
	}
}