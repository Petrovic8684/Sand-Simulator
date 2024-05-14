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

void render_grid(void)
{
    SDL_Rect rect;
    rect.w = rect.h = CELL_SIZE;

    for (Uint8 i = 0; i < GRID_DIMENSION; i++)
        for (Uint8 j = 0; j < GRID_DIMENSION; j++)
        {
            if (grid->content[i][j] == 1)
            {
                rect.x = j * CELL_SIZE;
                rect.y = i * CELL_SIZE;
                SDL_SetRenderDrawColor(renderer, grid->color[i][j].r, grid->color[i][j].g, grid->color[i][j].b, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
}

void render_paint_radius(void)
{
    SDL_Rect cell;
    cell.w = cell.h = CELL_SIZE;

    SDL_SetRenderDrawColor(renderer, color_white.r, color_white.g, color_white.b, 255);

    cell.x = current_mouse_cell_j * CELL_SIZE;
    cell.y = current_mouse_cell_i * CELL_SIZE;

    cell.h = paint_radius * CELL_SIZE;
    cell.w = paint_radius * CELL_SIZE;

    SDL_RenderDrawRect(renderer, &cell);
}

void move_grain_of_sand(Uint8 i_before, Uint8 j_before, Uint8 i_after, Uint8 j_after)
{
    if (j_after < 0 || j_after >= GRID_DIMENSION)
        return;

    grid_snapshot->content[i_before][j_before] = 0;
    grid_snapshot->color[i_before][j_before] = color_gray;

    grid_snapshot->content[i_after][j_after] = 1;
    grid_snapshot->color[i_after][j_after] = color_orange;
}

void update_grid(void)
{
    for (Uint32 i = 0; i < GRID_DIMENSION; i++)
        for (Uint32 j = 0; j < GRID_DIMENSION; j++)
        {
            if (grid->content[i][j] == 1)
            {
                if (grid->content[i + 1][j] == 0 && i < GRID_DIMENSION - 1) // Falling straight down
                    move_grain_of_sand(i, j, i + 1, j);

                else if (grid->content[i + 1][j] == 1 && grid->content[i + 1][j - 1] == 0 && grid->content[i + 1][j + 1] == 1) // Falling to the left
                    move_grain_of_sand(i, j, i + 1, j - 1);

                else if (grid->content[i + 1][j] == 1 && grid->content[i + 1][j + 1] == 0 && grid->content[i + 1][j - 1] == 1) // Falling to the right
                    move_grain_of_sand(i, j, i + 1, j + 1);

                else if (grid->content[i + 1][j] == 1 && grid->content[i + 1][j - 1] == 0 && grid->content[i + 1][j - 1] == 0) // 0.5 probability of falling left, 0.5 of right
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