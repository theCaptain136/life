#include "./my.h"

bool **int_grid(int size)
{
    bool **grid = malloc(sizeof(bool *) * size + 2);

    for (int y = 0; y <= size; y++) {
        grid[y] = malloc(sizeof(bool) * size + 2);
        for (int x = 0; x <= size; x++) {
            grid[y][x] = false;
        }
    }
    return (grid);
}

void draw_grid(sfRenderWindow **window, bool **grid, int size, sfConvexShape *convex)
{
    int m_x = 0;
    int m_y = 0;

    sfConvexShape_setPoint(convex, 0, (sfVector2f){0, 0});
    sfConvexShape_setPoint(convex, 1, (sfVector2f){1920 / size, 0});
    sfConvexShape_setPoint(convex, 2, (sfVector2f){1920 / size, 1080 / size});
    sfConvexShape_setPoint(convex, 3, (sfVector2f){0, 1080 / size});
    sfConvexShape_setOutlineThickness(convex, 1.0);
    sfConvexShape_setOutlineColor(convex, sfBlack);

    for (int y = 0; y <= size; y++) {
        for (int x = 0; x <= size; x++) {
            if (grid[y][x] == true) {
                sfConvexShape_setFillColor(convex, sfColor_fromRGB(162, 159, 255));
            } else {
                sfConvexShape_setFillColor(convex, sfColor_fromRGB(82, 5, 5));
            }
            sfConvexShape_setPosition(convex, (sfVector2f){m_x, m_y});
            sfRenderWindow_drawConvexShape(*window, convex, NULL);
            m_x += 1920 / size;
        }
        m_y += 1080 / size;
        m_x = 0;
    }
}

int count_neighbur(bool **grid, int x, int y, int size)
{
    int res = 0;

    if (x >= size || y >= size)
        return (-1);
    if (x > 0 && grid[y][x - 1] == true)
        res++;
    if (x < size - 1 && grid[y][x + 1] == true)
        res++;
    if (y > 0 && grid[y - 1][x] == true)
        res++;
    if (y < size - 1 && grid[y + 1][x] == true)
        res++;
    if (y > 0 && x > 0 && grid[y - 1][x - 1] == true)
        res++;
    if (y > 0 && x < size - 1 && grid[y - 1][x + 1] == true)
        res++;
    if (y < size - 1 && x < size - 1 && grid[y + 1][x + 1] == true)
        res++;
    if (y < size - 1 && x > 0 && grid[y + 1][x - 1] == true)
        res++;
    return (res);
}

void click(bool **grid, int x, int y, int size)
{
    int gx = x / (1920 / size);
    int gy = y / (1080 / size);

    if (grid[gy][gx] == false)
        grid[gy][gx] = true;
    else
        grid[gy][gx] = false;
}

void sim(bool **grid, int size, int run)
{
    int n = 0;

    if (run == -1)
        return;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            n = count_neighbur(grid, x, y, size);
            if (grid[y][x] == true) {
                if (n < 2)
                    grid[y][x] = false;
                else if (n > 3)
                    grid[y][x] = false;
            }
            else {
                if (n == 3)
                    grid[y][x] = true;
            }
        }
    }
}


void reset(bool **grid, int size)
{
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            grid[y][x] = false;
        }
    }
}

void free_grid(bool ***grid, int size)
{
    for (int y = 0; y < size; y++) {
        free((*grid)[y]);
    }
    free(*grid);
}

int main(int ac, char **av)
{
    sfVideoMode mode = {1920, 1080, 32};
    sfRenderWindow* window;
    sfEvent event;
    bool **grid = NULL;
    int size = 0;
    sfConvexShape *conv = sfConvexShape_create();
    int run = -1;

    sfConvexShape_setPointCount(conv, 4);
    if (ac < 2)
        return (84);
    size = my_getnbr(av[1]);
    grid = int_grid(size);
    window = sfRenderWindow_create(mode, "game of life", sfResize | sfClose, NULL);
    if (!window)
        return EXIT_FAILURE;
    sfRenderWindow_setFramerateLimit(window, 30);
    while (sfRenderWindow_isOpen(window))
    {
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            if (event.mouseButton.type == sfEvtMouseButtonPressed) {
                click(grid, event.mouseButton.x, event.mouseButton.y, size);
            }
            if (event.type == sfEvtKeyPressed) {
                switch (event.key.code) {
                    case sfKeySpace: run *= -1; break;
                    case sfKeyR: reset(grid, size); break;
                    default: break;
                }
            }
        }
        sfRenderWindow_clear(window, sfBlack);
        draw_grid(&window, grid, size, conv);
        sim(grid, size, run);
        sfRenderWindow_display(window);
    }
    free_grid(&grid, size);
    sfRenderWindow_destroy(window);
    return EXIT_SUCCESS;
}