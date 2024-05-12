#ifndef GRID_H
#define GRID_H

#include <string.h>
#include <SDL2/SDL.h>
#include "../game/game.h"

#define GRID_DIMENSION 200

struct grid
{
    Uint8 content[GRID_DIMENSION][GRID_DIMENSION];
    SDL_Color color[GRID_DIMENSION][GRID_DIMENSION];
};

extern struct grid *grid;

void initialize_grid(void);
void render_grid(void);
void update_grid(void);
void grid_cleanup(void);

#endif