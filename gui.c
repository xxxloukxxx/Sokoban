#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Mes includes
#include "init.h"
#include "init_video.h"
#include "sprites.h"
#include "levels.h"
#include "soko.h"
#include "gui.h"
#include "gui_draw.h"

// Initialisation variables globales
SprBank	spr_decor;
SprBank spr_ui;
int number_of_moves = 0;
int move_ok = 0;
int undo_ok = 0;
char direction = 'd';
int lvl_running = 1;
Tlevel lvl, backup, undo;
TTF_Font *font;
SDL_Texture *logo;

// Gestions des événements
int play_event_handler();

//////////////////////////////////////////////////////////////////////////
// Initialise les sprites, fonts et compagnie !
//////////////////////////////////////////////////////////////////////////
void init_gui() {
	SDL_Surface *surface_logo;
	spr_decor = load_sprites(renderer, "gfx\\sprites40x40.png", 40, 40);
	spr_ui = load_sprites(renderer, "gfx\\ui.png", 50, 50);
	
	surface_logo = IMG_Load("gfx\\logo.png");
	logo = SDL_CreateTextureFromSurface(renderer, surface_logo);
	SDL_FreeSurface(surface_logo);

	font = TTF_OpenFont("gfx\\upheavtt.ttf", 30);
}

//////////////////////////////////////////////////////////////////////////
// play() : have fun !!!!
//////////////////////////////////////////////////////////////////////////
int play() {
	lvl.t  = malloc( levels[current_lvl].c * levels[current_lvl].l * sizeof(char) );
	backup.t  = malloc( levels[current_lvl].c * levels[current_lvl].l * sizeof(char) );
	undo.t  = malloc( levels[current_lvl].c * levels[current_lvl].l * sizeof(char) );
	copy_lvl(levels[current_lvl], &lvl);
	copy_lvl(lvl, &backup);
	copy_lvl(lvl, &undo);
	number_of_moves = 0;
	lvl_running = 1;
	while (lvl_running) {
		play_event_handler();
		draw_playground();
		draw_hud();
		SDL_RenderPresent(renderer);
		SDL_Delay(DELAY);
	}
	// Sortie de la fonction
	switch (game_status) {
	case GS_MENU : // retour menu .. voir fondu en noir
		SDL_Delay(100);
		break;
	case GS_PLAY : // reset level
		SDL_Delay(100);
		break;
	case GS_PLAY_NEXT : // play next level
		if (is_solve(lvl)) {
			draw_level_complete();
			SDL_Delay(1500);
		}
		SDL_Delay(100);
		break;
	}
	// Libération
	free(lvl.t);
	lvl.t = NULL;
	free(backup.t);
	backup.t = NULL;
	free(undo.t);
	undo.t = NULL;
	return game_status;
}



//////////////////////////////////////////////////////////////////////////
// Gestion des events pour play()
//////////////////////////////////////////////////////////////////////////
int play_event_handler() {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	char key_pressed = direction;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			game_status = GS_EXIT;
			lvl_running = 0;
			break;
		case SDL_KEYDOWN:
			// Gestion des touches pour déplacer bébert
			key_pressed = ( (event.key.keysym.sym == SDLK_UP) ? 'u' : \
			                (event.key.keysym.sym == SDLK_DOWN) ? 'd' : \
			                (event.key.keysym.sym == SDLK_LEFT) ? 'l' : \
			                (event.key.keysym.sym == SDLK_RIGHT) ? 'r' : '\0' );
			if (key_pressed != '\0') {
				move_ok = move(&lvl, key_pressed);
				direction = key_pressed;
				if (move_ok) {
					number_of_moves = (number_of_moves + 1) % 100000;
					copy_lvl(backup, &undo);
					copy_lvl(lvl, &backup);
					undo_ok = 1;
				}
			}

			// Level fini ou F12 pour avancer d'un niveau
			if (state[SDL_SCANCODE_F12] || is_solve(lvl)) {
				lvl_running = 0;
				game_status = GS_PLAY_NEXT;
				break;
			}

			// Undo
			if (state[SDL_SCANCODE_F1]) {
				if (undo_ok) {
					copy_lvl(undo, &backup);
					copy_lvl(undo, &lvl);
					undo_ok = 0;
					break;
				}
			}

			// Level précedent : F11
			if (state[SDL_SCANCODE_F11]) {
				lvl_running = 0;
				current_lvl = ( current_lvl == 0 ) ? 0 : ( current_lvl - 1 ) % lvl_max;
				game_status = GS_PLAY;
				break;
			}

			// Retour menu
			if (state[SDL_SCANCODE_ESCAPE]) {
				lvl_running = 0;
				game_status = GS_MENU;
				break;
			}

			// Reset level
			if (state[SDL_SCANCODE_RETURN]) {
				lvl_running = 0;
				game_status = GS_PLAY;
				break;
			}
			break;
		}
	}
	return lvl_running;
}

//////////////////////////////////////////////////////////////////////////
// Le menu
//////////////////////////////////////////////////////////////////////////
int menu() {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	SDL_Event event;
	int menu_running = 1;
	int choix = 0 ;
	while (menu_running) {

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				game_status = GS_EXIT;
				menu_running = 0;
				break;
			case SDL_KEYDOWN:
				// Gestion des touches deplacer dans le menu
				if (event.key.keysym.sym == SDLK_DOWN) {
					if (choix < 2) choix++;
				}

				if (event.key.keysym.sym == SDLK_UP) {
					if (choix > 0) choix--;
				}

				if (event.key.keysym.sym == SDLK_LEFT) {
					if (choix == 1 && current_lvl > 0) {
						current_lvl--;
					}
					if (current_lvl < 0) current_lvl = 0;
				}

				if (event.key.keysym.sym == SDLK_RIGHT) {
					if (choix == 1 && current_lvl < lvl_max - 1) {
						current_lvl++;
					}
					if (current_lvl > lvl_max - 1) current_lvl = lvl_max - 1;
				}

				if (state[SDL_SCANCODE_RETURN]) {
					menu_running = 0;
					game_status = GS_PLAY;
					break;
				}
				break;
			}

		}
		draw_background(0);
		draw_title();
		draw_menu(choix);
		SDL_RenderPresent(renderer);
		SDL_Delay(DELAY);
	}

	if (game_status != GS_EXIT) {
		game_status = (choix == 2) ? GS_EXIT : GS_PLAY;
	}

	return game_status;
}


