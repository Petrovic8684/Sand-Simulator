#include "fonts.h"

TTF_Font *font = NULL;

struct text sand_simulator_text;
struct text made_by_text;
struct text press_enter_to_start_text;

bool should_render_sand_simulator_text = true;

void initialize_text(void)
{
    if (TTF_Init() == -1)
    {
        fprintf(stderr, "SDL_ttf failed to initialize: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    font = TTF_OpenFont("assets/fonts/runescape.ttf", 36);
}

void update_text(const char *content, SDL_Color color, int x, int y, struct text *text)
{
    text->text_surface = TTF_RenderText_Solid(font, content, color);
    text->text_texture = SDL_CreateTextureFromSurface(renderer, text->text_surface);

    text->text_rect.x = x;
    text->text_rect.y = y;
    text->text_rect.w = text->text_surface->w;
    text->text_rect.h = text->text_surface->h;
}

void render_sand_simulator_text(void)
{
    if (should_render_sand_simulator_text == true)
    {
        TTF_SetFontSize(font, 60);
        update_text("S A N D   S I M U L A T O R", get_random_color(), 48, WINDOW_HEIGHT / 4 + 30, &sand_simulator_text);

        should_render_sand_simulator_text = false;
    }
}

void render_menu_text(void)
{
    TTF_SetFontSize(font, 36);
    update_text("Made by Jovan Petrovic", color_white, WINDOW_WIDTH / 5 + 25, WINDOW_HEIGHT / 3 + 75, &made_by_text);
    update_text("Press ENTER to start", color_white, WINDOW_WIDTH / 5 + 40, WINDOW_HEIGHT / 3 + 125, &press_enter_to_start_text);

    SDL_RenderCopy(renderer, sand_simulator_text.text_texture, NULL, &sand_simulator_text.text_rect);
    SDL_RenderCopy(renderer, made_by_text.text_texture, NULL, &made_by_text.text_rect);
    SDL_RenderCopy(renderer, press_enter_to_start_text.text_texture, NULL, &press_enter_to_start_text.text_rect);
};

void text_cleanup(struct text *text)
{
    SDL_FreeSurface(text->text_surface);
    SDL_DestroyTexture(text->text_texture);

    text->text_surface = NULL;
    text->text_texture = NULL;
}

void font_cleanup(void)
{
    TTF_CloseFont(font);
    font = NULL;

    TTF_Quit();
}