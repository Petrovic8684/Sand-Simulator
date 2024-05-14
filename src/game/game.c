#include "game.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool is_window_open = true;
bool is_in_menu = true;
bool should_restart = false;

int current_mouse_cell_i;
int current_mouse_cell_j;
Uint8 paint_radius = 1;

void initialize_sdl(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL failed to initialize: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void create_window_and_renderer(const char *title)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (window == NULL)
    {
        fprintf(stderr, "SDL window failed to initialize: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (renderer == NULL)
    {
        fprintf(stderr, "SDL renderer failed to initialize: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void get_mouse_position(void)
{
    int mouse_x;
    int mouse_y;

    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (mouse_x >= WINDOW_WIDTH)
        mouse_x = WINDOW_WIDTH - 1;
    if (mouse_x < 0)
        mouse_x = 0;
    if (mouse_y >= WINDOW_HEIGHT)
        mouse_y = WINDOW_HEIGHT - 1;
    if (mouse_y < 0)
        mouse_y = 0;

    int cell_size = CELL_SIZE;

    current_mouse_cell_j = mouse_x / cell_size;
    current_mouse_cell_i = mouse_y / cell_size;
}

void handle_input(SDL_Event event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_RETURN:
        if (is_in_menu == false)
            return;

        is_in_menu = false;
        break;
    case SDLK_ESCAPE:
        if (is_in_menu == true)
            return;

        is_in_menu = true;
        break;
    case SDLK_r:
        if (is_in_menu == true)
            return;

        should_restart = true;
        break;
    }
}

void handle_click(bool should_delete)
{
    if (is_in_menu == true)
        return;

    if (should_delete == false)
    {
        for (Sint8 i = 0; i < paint_radius; i++)
            for (Sint8 j = 0; j < paint_radius; j++)
            {
                if (grid->content[current_mouse_cell_i + i][current_mouse_cell_j + j] == 1)
                    continue;

                if (current_mouse_cell_i + i >= GRID_DIMENSION || current_mouse_cell_j + j >= GRID_DIMENSION)
                    continue;

                grid_snapshot->content[current_mouse_cell_i + i][current_mouse_cell_j + j] = 1;
                grid_snapshot->color[current_mouse_cell_i + i][current_mouse_cell_j + j] = color_orange;
            }
    }
    else
    {
        for (Sint8 i = 0; i < paint_radius; i++)
            for (Sint8 j = 0; j < paint_radius; j++)
            {
                if (grid->content[current_mouse_cell_i + i][current_mouse_cell_j + j] == 0)
                    continue;

                if (current_mouse_cell_i + i >= GRID_DIMENSION || current_mouse_cell_j + j >= GRID_DIMENSION)
                    continue;

                grid_snapshot->content[current_mouse_cell_i + i][current_mouse_cell_j + j] = 0;
                grid_snapshot->color[current_mouse_cell_i + i][current_mouse_cell_j + j] = color_gray;
            }
    }

    memcpy(grid, grid_snapshot, sizeof(struct grid));
}

void poll_events(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            is_window_open = false;
            break;
        case SDL_KEYUP:
            handle_input(event);
            break;
        case SDL_MOUSEMOTION:
            if (event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
                handle_click(false);
            if (event.motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT))
                handle_click(true);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                handle_click(false);
            if (event.button.button == SDL_BUTTON_RIGHT)
                handle_click(true);
            break;
        case SDL_MOUSEWHEEL:
            if (paint_radius + event.wheel.y < 1)
            {
                paint_radius = 1;
                return;
            }

            if (paint_radius + event.wheel.y > MAX_PAINT_RADIUS)
            {
                paint_radius = MAX_PAINT_RADIUS;
                return;
            }
            paint_radius += event.wheel.y;

            break;
        }
    }
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, color_black.r, color_black.g, color_black.b, 255);
    SDL_RenderClear(renderer);

    render_grid();
    render_paint_radius();

    SDL_RenderPresent(renderer);
}

void restart_game(void)
{
    should_restart = false;

    grid_cleanup();
    initialize_grid();

    srand(time(NULL));
}

void start_game_and_keep_running(void)
{
    Uint32 start_time = SDL_GetTicks();
    Uint32 end_time, elapsed_time;

menu:
    restart_game();
    while (is_window_open == true && is_in_menu == true)
    {
        SDL_SetRenderDrawColor(renderer, color_black.r, color_black.g, color_black.b, 255);
        SDL_RenderClear(renderer);

        end_time = SDL_GetTicks();
        elapsed_time = end_time - start_time;

        if (elapsed_time > 1500)
        {
            start_time = end_time;
            should_render_sand_simulator_text = true;
        }

        render_sand_simulator_text();
        render_menu_text();

        poll_events();
        SDL_RenderPresent(renderer);
    }

    start_time = SDL_GetTicks();

game:
    while (is_window_open == true && should_restart == false)
    {
        render();
        poll_events();

        if (is_in_menu == true)
            goto menu;

        end_time = SDL_GetTicks();
        elapsed_time = end_time - start_time;

        if (elapsed_time > GAME_SPEED)
        {
            start_time = end_time;

            get_mouse_position();
            update_grid();
        }
    }

    if (should_restart == true)
    {
        restart_game();
        goto game;
    }
}

void game_cleanup(void)
{
    text_cleanup(&sand_simulator_text);
    text_cleanup(&made_by_text);
    text_cleanup(&press_enter_to_start_text);

    font_cleanup();
    sound_cleanup();

    grid_cleanup();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}