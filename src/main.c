#include "game/game.h"

int main(int argc, char *argv[])
{
    initialize_sdl();
    create_window_and_renderer("Sand Simulator");
    initialize_text();

    initialize_sound();

    initialize_grid();
    start_game_and_keep_running();

    game_cleanup();

    return EXIT_SUCCESS;
}