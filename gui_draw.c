#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "init.h"
#include "init_video.h"
#include "sprites.h"
#include "levels.h"
#include "soko.h"
#include "gui.h"
#include "gui_draw.h"

double f_creneau(double x);
double f_rebond(int t, int period);

//////////////////////////////////////////////////////////////////////////
// affiche une "string"
//////////////////////////////////////////////////////////////////////////
void draw_ttf_string(SDL_Renderer *r, TTF_Font *f, char* str, int x, int y) {
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Color textColor = {255, 255, 255, 0};
	surface = TTF_RenderText_Blended(f, str, textColor);
	texture = SDL_CreateTextureFromSurface(r, surface);
	SDL_Rect dst = {x, y, surface->w, surface->h};
	SDL_RenderCopy(r, texture, NULL, &dst);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}


//////////////////////////////////////////////////////////////////////////
// Clear_screen
//////////////////////////////////////////////////////////////////////////
void clear_screen(int r, int g, int b) {
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

//////////////////////////////////////////////////////////////////////////
// Dessine un rect avec spr_ui
//////////////////////////////////////////////////////////////////////////
void draw_ui_rect(int x, int y, int w, int h) {
	int xx = x + spr_ui.spr_w;
	int yy = y + h - spr_ui.spr_w;
	draw_sprite(renderer, spr_ui, 32, x, y);
	while (xx < (x + w - spr_ui.spr_w) ) {
		draw_sprite(renderer, spr_ui, 33, xx, y);
		xx = xx + spr_ui.spr_w;
	}
	draw_sprite(renderer, spr_ui, 35, xx, y);
	xx = x + spr_ui.spr_w;
	draw_sprite(renderer, spr_ui, 40, x, yy);
	while (xx < (x + w - spr_ui.spr_w) ) {
		draw_sprite(renderer, spr_ui, 41, xx, yy);
		xx = xx + spr_ui.spr_w;
	}
	draw_sprite(renderer, spr_ui, 43, xx, yy);
	xx = x + spr_ui.spr_w;
	yy = y + spr_ui.spr_h;
	while ( yy < y + h - spr_ui.spr_w) {
		draw_sprite(renderer, spr_ui, 36, x, yy);
		while (xx < (x + w - spr_ui.spr_w) ) {
			draw_sprite(renderer, spr_ui, 37, xx, yy);
			xx = xx + spr_ui.spr_w;
		}
		draw_sprite(renderer, spr_ui, 39, xx, yy);
		xx = x + spr_ui.spr_w;
		yy = yy + spr_ui.spr_h;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void draw_playground() {
	int i, spr_n = 0, c = 1, x, y, spr_player;
	int w = spr_decor.spr_w;
	int h = spr_decor.spr_h;
	int xd = ( ( SCREEN_WIDTH / w ) - (lvl.c) ) / 2 * w;
	int yd = ( ( SCREEN_HEIGHT / h ) - (lvl.l) ) / 2 * h;

	draw_background(1);
	switch (direction) {
	case 'u' : spr_player = 8; break;
	case 'd' : spr_player = 10; break;
	case 'l' : spr_player = 12; break;
	case 'r' : spr_player = 14; break;
	default : spr_player = 10;
	}
	for (i = 0; lvl.t[i] != '\0'; i++) {
		c = lvl.t[i];
		x = xd + (i % lvl.c) * w;
		y = yd + (i / lvl.c) * w;
		switch (c) {
		case VIDE : spr_n = 0; break;
		case WALL : spr_n = 1; break;
		case BOX : spr_n = 5; break;
		case GOAL : spr_n = 3; break;
		case PLAYER : spr_n = spr_player;
			draw_sprite(renderer, spr_decor, 2, x, y);
			break;
		case PLAYER_ON_GOAL : spr_n = spr_player;
			draw_sprite(renderer, spr_decor, 3, x, y);
			break;
		case BOX_ON_GOAL : spr_n = 4;
			draw_sprite(renderer, spr_decor, 3, x, y);
			break;
		case FLOOR : spr_n = 2; break;
		}
		draw_sprite(renderer, spr_decor, spr_n, x, y);
	}
}


////////////////////////////////////////////////////////////////////////
// Affiche le Hud (l'heure, le level et les nombre de moves)
////////////////////////////////////////////////////////////////////////
void draw_hud() {
	int i, x = 0;
	int spr_n_left[5] = {1, 2, 1, 2, 3};
	int spr_n_right[5] = {1, 2, 1, 2, 0};

	// Affiche les cadres en haut
	for (i = 0; i < 5; i++) {
		draw_sprite(renderer, spr_ui, 16 + spr_n_left[i], i * 50, 2);
		draw_sprite(renderer, spr_ui, 16 + spr_n_left[i], i * 50, SCREEN_HEIGHT - 50);
	}

	// Affiche les cadres en bas
	for (i = 0; i < 5; i++) {
		draw_sprite(renderer, spr_ui, 16 + spr_n_right[i], SCREEN_WIDTH - (i + 1) * 50	, 2);
		draw_sprite(renderer, spr_ui, 16 + spr_n_right[i], SCREEN_WIDTH - (i + 1) * 27	, SCREEN_HEIGHT - 50);
	}

	// Level et Moves
	char buffer[50];
	sprintf(buffer, "LEVEL : %i", current_lvl);
	draw_ttf_string(renderer, font, buffer, 20 , 10);
	sprintf(buffer, "Moves : %.5i", number_of_moves);
	draw_ttf_string(renderer, font, buffer, SCREEN_WIDTH - 235 , 10);

	// affiche l'heure
	time_t timestamp = time(NULL);
	strftime(buffer, sizeof(buffer), "%H : %M", localtime(&timestamp));
	draw_ttf_string(renderer, font, buffer, SCREEN_WIDTH - 110 , SCREEN_HEIGHT - 42);

	// Affiche "l'aide"
	int nbchoix = 3, duration = 5000 ;
	int t = SDL_GetTicks() % (nbchoix * duration);
	i = t / duration;
	int delta = 0;
	switch (i) {
	case 0 :
		sprintf(buffer, "Return : Reset");
		break;
	case 1 :
		sprintf(buffer, "F1 : Undo");
		break;
	case 2 :
		sprintf(buffer, "Esc : Menu");
		break;
	}
	TTF_SizeText(font, buffer, &delta, NULL);
	delta = delta + 15;
	x = 10 + (int)(delta * f_creneau( (t - i * (duration) ) / 1000.0)) - delta;
	draw_ttf_string(renderer, font, buffer, x , SCREEN_HEIGHT - 42);
}

////////////////////////////////////////////////////////////////////////
// affiche l'encart level complete
////////////////////////////////////////////////////////////////////////
void draw_level_complete() {
	int w_text;
	char buffer[50];
	// le cadre
	draw_ui_rect((SCREEN_WIDTH - 350) / 2, 10, 350, 80);
	// le texte
	sprintf(buffer, "Level Complete !!!");
	TTF_SizeText(font, buffer, &w_text, NULL);
	draw_ttf_string(renderer, font, buffer, (SCREEN_WIDTH - w_text) / 2 , 20 );
	sprintf(buffer, " Moves : %.5i", number_of_moves);
	TTF_SizeText(font, buffer, &w_text, NULL);
	draw_ttf_string(renderer, font, buffer, (SCREEN_WIDTH - w_text) / 2 , 45 );
	SDL_RenderPresent(renderer);
}

////////////////////////////////////////////////////////////////////////
///  fonction créneau
////////////////////////////////////////////////////////////////////////
double f_creneau(double x) {
	return (1.0 - (1.0 / (1.0 + 10.0 * x * x ) ) );
}



////////////////////////////////////////////////////////////////////////
// Dessine l'arrière plan
////////////////////////////////////////////////////////////////////////
void draw_background(int n) {
	int i = 0, x = 0, y = 0;
	char c = 1;
	for (i = 0; c != '\0'; i++) {
		c = back[n].t[i];
		x = (i % 32) * spr_decor.spr_w;
		y = (i / 32) * spr_decor.spr_h;
		draw_sprite(renderer, spr_decor, 2, x, y);
		draw_sprite(renderer, spr_decor, back[n].t[i] - 40, x, y);
	}

}

//////////////////////////////////////////////////////////////////////////
// Pour le menu
//////////////////////////////////////////////////////////////////////////
void draw_menu(int choix) {
	// int c = 0;
	char buffer[50];
	//draw_menu(int c);
	int xd[3], w, h;
	int yd = (choix == 0) ? 500 : ((choix == 1) ? 550 : 600);

	sprintf(buffer, "Start");
	TTF_SizeText(font, buffer,  &w, &h);
	xd[0] = (SCREEN_WIDTH - w) / 2;
	draw_ttf_string(renderer, font, buffer, xd[0], 500);

	sprintf(buffer, "Level Select : %i", current_lvl);
	TTF_SizeText(font, buffer,  &w, &h);
	xd[1] = (SCREEN_WIDTH - w) / 2;
	draw_ttf_string(renderer, font, buffer, xd[1], 550);

	sprintf(buffer, "Quit");
	TTF_SizeText(font, buffer,  &w, &h);
	xd[2] = (SCREEN_WIDTH - w) / 2;
	draw_ttf_string(renderer, font, buffer, xd[2], 600);

	draw_sprite(renderer, spr_decor, 4, xd[choix] - 1.5 * spr_decor.spr_w, yd - 2);
	draw_sprite(renderer, spr_decor, 4, SCREEN_WIDTH - xd[choix] + 0.5 * spr_decor.spr_w, yd - 2);
}


//////////////////////////////////////////////////////////////////////////
// Draw_title : le titre qui rebondi
//////////////////////////////////////////////////////////////////////////
void draw_title() {
	int width, height;
	SDL_Rect dst;
	SDL_QueryTexture(logo, NULL, NULL, &width, &height);
	double zoom = 1.0 + f_rebond(SDL_GetTicks(), 500) / 8.0;
	double angle = 10 * f_rebond(SDL_GetTicks(), 2500) - 5;
	dst.w = width * zoom;
	dst.h = height * zoom;
	dst.x = ( SCREEN_WIDTH - dst.w ) / 2;
	dst.y = (500 - dst.h) / 2;
	SDL_RenderCopyEx(renderer, logo, NULL, &dst, angle, NULL, SDL_FLIP_NONE);
}

double f_rebond(int t, int period) {
	t = t % period;
	return fabs( sin(t * 3.14 / period) );
}
