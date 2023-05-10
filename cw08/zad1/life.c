#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>


#define GRID_WIDTH 30
#define GRID_HEIGHT 30
#define CELL_AMOUNT 900

char *foreground;
char *background;

typedef struct {
	int x;
	int y;
} thread_data_t;


void* start_cell(void* arg)
{
    thread_data_t* data = (thread_data_t*) arg;
    int x = data->x;
    int y = data->y;
    while (1)
    {
		update_cell(x, y, foreground, background);
		usleep(10000);
        pause();
    }
    return NULL;
}

void handler1(int signum){ } // do nothing

int main()
{
    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode

    foreground = create_grid();
    background = create_grid();
	char *tmp;

	init_grid(foreground);

    thread_data_t args[CELL_AMOUNT];
    pthread_t threads[CELL_AMOUNT];
    for (int i = 0; i < CELL_AMOUNT; i++)
    {
		args[i].x = i % GRID_WIDTH;
		args[i].y = i / GRID_WIDTH;
        pthread_create(&threads[i], NULL, start_cell, &args[i]);
    }

    signal(SIGUSR1, handler1);

    while (true)
    {
        draw_grid(foreground);
        usleep(500 * 1000);

        // send SIGUSR1 to all threads
        for (int i = 0; i < CELL_AMOUNT; i++)
        {
            pthread_kill(threads[i], SIGUSR1);
        }

        tmp = foreground;
        foreground = background;
        background = tmp;
    }

    endwin(); // End curses mode
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}
