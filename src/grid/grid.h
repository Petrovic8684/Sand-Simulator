#ifndef GRID_H
#define GRID_H

#include <string.h>
#include <SDL2/SDL.h>
#include "../game/game.h"

#define GRID_DIMENSION 150
#define CELL_SIZE WINDOW_HEIGHT / GRID_DIMENSION

struct grid
{
    Uint8 content[GRID_DIMENSION][GRID_DIMENSION];
    SDL_Color color[GRID_DIMENSION][GRID_DIMENSION];
};

extern struct grid *grid;
extern struct grid *grid_snapshot;

void initialize_grid(void);
void render_grid(void);
void render_paint_radius(void);
void move_grain_of_sand(Uint8 i_before, Uint8 j_before, Uint8 i_after, Uint8 j_after);
void update_grid(void);
void grid_cleanup(void);

#endif