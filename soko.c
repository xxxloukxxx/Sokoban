#include <stdlib.h>
#include <stdio.h>
#include "soko.h"

/// valeur absolue en version "rapide"
#define vabs(a) ((a<0)?-a:a)

Tpos get_hero_pos(Tlevel lvl) {
	Tpos result={-1,-1};
	int i=0;
	while(lvl.t[i]!='\0') {
		if(lvl.t[i]==PLAYER || lvl.t[i]==PLAYER_ON_GOAL) {
			result.y = i / lvl.c;
			result.x = i % lvl.c;
		}
		i++;
	}
	return result;
}

uint move_possible(Tlevel lvl,Tpos src,Tpos dst) {
	int vx=dst.x-src.x;
	int vy=dst.y-src.y;
	char c_dst=lvl.t[dst.y*lvl.c+dst.x];
	char c_box_dst;

	if (dst.x>lvl.c || dst.y> lvl.l) return 0;
	if (vabs(vx)>1 || vabs(vy)>1) return 0;
	if (vabs(vx)==1 && vabs(vy)==1) return 0;
	if(c_dst==FLOOR || c_dst==GOAL){ return 1; }
	if(c_dst==BOX || c_dst==BOX_ON_GOAL) {
		c_box_dst=lvl.t[(dst.y+vy)*lvl.c+(dst.x+vx)];
		if(c_box_dst==FLOOR || c_box_dst==GOAL) return 1;
	}
	return 0;
}

uint move_src_dst(Tlevel* lvl, Tpos src,Tpos dst){
	if( !move_possible( (*lvl), src, dst) ) return 0;
	int   cols=(*lvl).c;
	int   i_src=src.y*cols+src.x;
	char  c_src=(*lvl).t[i_src];
	int   i_dst=dst.y*cols+dst.x;
	char  c_dst=(*lvl).t[i_dst];
	int   vx=dst.x-src.x;
	int   vy=dst.y-src.y;
	int   i_box_dst=(dst.y+vy)*cols+(dst.x+vx);
	char  c_box_dst=(*lvl).t[(dst.y+vy)*cols+(dst.x+vx)];

	(*lvl).t[i_src]=(c_src==PLAYER)?FLOOR:GOAL;

	if (c_dst==FLOOR) (*lvl).t[i_dst]=PLAYER;
	if (c_dst==GOAL) (*lvl).t[i_dst]=PLAYER_ON_GOAL;
	// on pousse une boite
	if (c_dst==BOX) {
		(*lvl).t[i_dst]=PLAYER;
		(*lvl).t[i_box_dst]=(c_box_dst==FLOOR)?BOX:BOX_ON_GOAL;
	}
	if (c_dst==BOX_ON_GOAL) {
		(*lvl).t[i_dst]=PLAYER_ON_GOAL;
		(*lvl).t[i_box_dst]=(c_box_dst==FLOOR)?BOX:BOX_ON_GOAL;
	}
	return 1;
}


uint move(Tlevel* lvl,uint direction){
	Tpos hero,hero_dst;
	hero=get_hero_pos( (*lvl) );
	hero_dst=hero;
	switch(direction) {
		case 1 : hero_dst.y--; break;
		case 2 : hero_dst.y++; break;
		case 3 : hero_dst.x--; break;
		case 4 : hero_dst.x++; break;
		case 'u' : hero_dst.y--; break;
		case 'd' : hero_dst.y++; break;
		case 'l' : hero_dst.x--; break;
		case 'r' : hero_dst.x++; break;
	}
	return move_src_dst(lvl,hero,hero_dst);
}


void print_lvl(Tlevel lvl) {
	int i=0;
	while(lvl.t[i]!='\0') {
		printf("%c",lvl.t[i]);
		i++;
		if((i%lvl.c)==0) printf("\n");
	}
	//printf("\n");
}

void copy_lvl(Tlevel src,Tlevel* dst){
	(*dst).l=src.l;
	(*dst).c=src.c;
	int i=0;
	while( src.t[i]!='\0' ) {
		(*dst).t[i]=src.t[i];
		i++;
	}
	(*dst).t[i]='\0';
}

uint is_solve(Tlevel lvl){
	int i=0;
	while(lvl.t[i]!='\0') {
		if (lvl.t[i]==BOX) return 0;
		i++;
	}
	return 1;
}