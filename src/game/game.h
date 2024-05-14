#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "../grid/grid.h"
#include "../util/colors/colors.h"
#include "../util/sounds/sounds.h"
#include "../util/fonts/fonts.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define GAME_SPEED 25
#define MAX_PAINT_RADIUS 15

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern bool is_window_open;
extern bool is_in_menu;
extern bool should_restart;

extern int current_mouse_cell_i;
extern int current_mouse_cell_j;
extern Uint8 paint_radius;

void initialize_sdl(void);
void create_window_and_renderer(const char *title);
void get_mouse_position(void);
void handle_input(SDL_Event event);
void handle_click(bool should_delete);
void poll_events(void);
void render(void);
void restart_game(void);
void start_game_and_keep_running(void);
void game_cleanup(void);

#endif