#include "grid.h"

struct grid *grid = NULL;

void initialize_grid(void)
{
    grid = (struct grid *)malloc(sizeof(struct grid));
    memset(grid->content, 0, sizeof(Uint8[GRID_DIMENSION][GRID_DIMENSION]));
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

void update_grid(void)
{
}

void grid_cleanup(void)
{
    free(grid);
    grid = NULL;
}