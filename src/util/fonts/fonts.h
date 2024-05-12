#ifndef FONTS_H
#define FONTS_H

#include <SDL2/SDL_ttf.h>
#include "../../game/game.h"

struct text
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect text_rect;
};

extern TTF_Font *font;

extern struct text sand_simulator_text;
extern struct text made_by_text;
extern struct text press_enter_to_start_text;

extern bool has_game_text_changed;
extern bool should_render_sand_simulator_text;

void initialize_text(void);
void update_text(const char *content, SDL_Color color, int x, int y, struct text *text);
void render_sand_simulator_text(void);
void render_menu_text(void);
void text_cleanup(struct text *text);
void font_cleanup(void);

#endif