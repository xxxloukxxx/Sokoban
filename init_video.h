#ifndef init_video_H_
#define init_video_H_

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOWS_TITLE "Gigi SOKOBAN"
#define WINDOWS_FLAGS SDL_WINDOW_SHOWN //| SDL_WINDOW_FULLSCREEN

SDL_Window* window;
SDL_Renderer* renderer;

void init_SDL2();
SDL_Window* init_window();
SDL_Renderer* init_renderer(SDL_Window* window);
int quit_SDL2();

#endif