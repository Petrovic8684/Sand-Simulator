#include "grid.h"

struct grid *grid = NULL;
struct grid *grid_snapshot = NULL;

void initialize_grid(void)
{
    grid = (struct grid *)malloc(sizeof(struct grid));
    memset(grid->content, 0, sizeof(Uint8[GRID_DIMENSION][GRID_DIMENSION]));

    grid_snapshot = (struct grid *)malloc(sizeof(struct grid));
    memset(grid_snapshot->content, 0, sizeof(Uint8[GRID_DIMENSION][GRID_DIMENSION]));
}

void render_grid()
{
    Uint8 cell_size = WINDOW_WIDTH / GRID_DIMENSION;

    SDL_Rect cell;
    cell.w = cell.h = cell_size;

    SDL_Rect rect;
    rect.w = rect.h = cell_size;

    for (Uint8 i = 0; i < GRID_DIMENSION; i++)
        for (Uint8 j = 0; j < GRID_DIMENSION; j++)
        {
            if (grid_style == GRID_ON)
            {
                SDL_SetRenderDrawColor(renderer, color_gray.r, color_gray.g, color_gray.b, 255);

                cell.x = j * cell_size;
                cell.y = i * cell_size;
                SDL_RenderDrawRect(renderer, &cell);
            }

            if (grid->content[i][j] == 1)
            {
                rect.x = j * cell_size;
                rect.y = i * cell_size;
                SDL_SetRenderDrawColor(renderer, grid->color[i][j].r, grid->color[i][j].g, grid->color[i][j].b, 255);
                SDL_RenderFillRect(renderer, &rect);

                if (grid_style == GRID_ON)
                {
                    SDL_SetRenderDrawColor(renderer, color_gray.r, color_gray.g, color_gray.b, 255);
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }
        }
}

void move_grain_of_sand(Uint8 i_before, Uint8 j_before, Uint8 i_after, Uint8 j_after)
{
    grid_snapshot->content[i_before][j_before] = 0;
    grid_snapshot->color[i_before][j_before] = color_gray;

    grid_snapshot->content[i_after][j_after] = 1;
    grid_snapshot->color[i_after][j_after] = color_orange;
}

void update_grid(void)
{
    for (Uint8 i = 0; i < GRID_DIMENSION; i++)
        for (Uint8 j = 0; j < GRID_DIMENSION; j++)
        {
            if (grid->content[i][j] == 1)
            {
                if (grid->content[i + 1][j] == 0 && i < GRID_DIMENSION - 1) // Falling straight down
                    move_grain_of_sand(i, j, i + 1, j);

                else if (grid->content[i + 1][j] == 1 && grid->content[i + 1][j - 1] == 0 && grid->content[i + 1][j + 1] == 1) // Falling to the left
                    move_grain_of_sand(i, j, i + 1, j - 1);

                else if (grid->content[i + 1][j] == 1 && grid->content[i + 1][j + 1] == 0 && grid->content[i + 1][j - 1] == 1) // Falling to the right
                    move_grain_of_sand(i, j, i + 1, j + 1);

                else if (grid->content[i + 1][j] == 1 && grid->content[i + 1][j - 1] == 0 && grid->content[i + 1][j - 1] == 0)
                {
                    if ((double)rand() / RAND_MAX > 0.5)
                        move_grain_of_sand(i, j, i + 1, j + 1);
                    else
                        move_grain_of_sand(i, j, i + 1, j - 1);
                }
            }
        }

    memcpy(grid, grid_snapshot, sizeof(struct grid));
}

void grid_cleanup(void)
{
    free(grid);
    grid = NULL;

    free(grid_snapshot);
    grid_snapshot = NULL;
}