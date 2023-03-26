#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define SNAKE_MAX_LENGTH 100
#define FOOD_MAX_COUNT 5

typedef struct
{
    int x;
    int y;
} Position;

Position snake[SNAKE_MAX_LENGTH];
int snake_length = 1;
int score = 0; // Score du joueur
int direction = KEY_RIGHT;
int gameover = 0;
Position food[FOOD_MAX_COUNT];
char terrain[HEIGHT][WIDTH];

void setup()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100);
    srand(time(NULL));

    // Initialize terrain with walls
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
            {
                terrain[i][j] = '*';
            }
            else
            {
                terrain[i][j] = ' ';
            }
        }
    }

    // Initial snake position
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;

    // Generate food positions
    for (int i = 0; i < FOOD_MAX_COUNT; i++)
    {
        food[i].x = rand() % WIDTH;
        food[i].y = rand() % HEIGHT;
    }
}

void draw()
{
    clear();

    // Draw terrain
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            mvprintw(i, j, "%c", terrain[i][j]);
        }
    }

    // Draw food
    for (int i = 0; i < FOOD_MAX_COUNT; i++)
    {
        mvprintw(food[i].y, food[i].x, "*");
    }

    // Draw snake
    for (int i = 0; i < snake_length; i++)
    {
        mvprintw(snake[i].y, snake[i].x, "o");
        mvprintw(snake[i].y, snake[i].x + 1, "  ");
    }

    // Draw score
    mvprintw(HEIGHT + 1, 0, "Score: %d", score);

    refresh();
}

void input()
{
    int c = getch();

    switch (c)
    {
    case KEY_LEFT:
    case KEY_RIGHT:
    case KEY_UP:
    case KEY_DOWN:
        direction = c;
        break;
    case 27: // ESC key
        gameover = 1;
        break;
    }
}

void logic()
{
    // Move snake
    for (int i = snake_length - 1; i > 0; i--)
    {
        snake[i] = snake[i - 1];
    }
    switch (direction)
    {
    case KEY_LEFT:
        snake[0].x--;
        break;
    case KEY_RIGHT:
        snake[0].x++;
        break;
    case KEY_UP:
        snake[0].y--;
        break;
    case KEY_DOWN:
        snake[0].y++;
        break;
    }

    // Check collision with food
    for (int i = 0; i < FOOD_MAX_COUNT; i++)
    {
        if (snake[0].x == food[i].x && snake[0].y == food[i].y)
        {
            score++; // Incrémente le score lorsque le serpent mange un aliment
            snake_length++;
            food[i].x = rand() % WIDTH;
            food[i].y = rand() % HEIGHT;
        }
    }

    // Check collision with walls or self
    if (terrain[snake[0].y][snake[0].x] == '*' ||
        snake[0].x < 1 || snake[0].x >= WIDTH - 1 || snake[0].y < 1 || snake[0].y >= HEIGHT - 1)
    {
        gameover = 1;
    }
    else
    {
        for (int i = 1; i < snake_length; i++)
        {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            {
                gameover = 1;
                break;
            }
        }
    }
}

void cleanup()
{
    endwin();
}

int main()
{
    setup();
    while (!gameover)
    {
        draw();
        input();
        logic();
    }

    cleanup();
    return 0;
}
