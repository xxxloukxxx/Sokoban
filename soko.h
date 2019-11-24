#ifndef SOKO_H_
#define SOKO_H_

#define uint unsigned int

#define VIDE                0x5F
#define WALL                0x23
#define PLAYER              0x40
#define PLAYER_ON_GOAL      0x2b
#define BOX                 0x24
#define BOX_ON_GOAL         0x2a
#define GOAL                0x2e
#define FLOOR               0x2d

typedef struct{
	uint l,c;
	char *t;
} Tlevel;

typedef struct{
	uint x,y;
} Tpos;

Tpos get_hero_pos(Tlevel lvl);

uint move_possible(Tlevel lvl,Tpos src,Tpos dst);
uint move_src_dst(Tlevel* lvl,Tpos src,Tpos dst);
uint move(Tlevel* lvl,uint direction);

uint is_solve(Tlevel lvl);
void print_lvl(Tlevel lvl);
void copy_lvl(Tlevel src,Tlevel* dst);

#endif