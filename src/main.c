#include "backend.h"
#include "frontend.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main() {
  int xmax;
  int ymax;
  char game_over[] = "Game Over!\n";
  int *score = (int *) calloc(1, sizeof(int));

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE); // make keys work
  curs_set(0); // hide cursor
  timeout(100);
  getmaxyx(stdscr, ymax, xmax);
  enum Direction dir = RIGHT;

  Board* board = create_board(create_snake(), NULL, xmax, ymax);
  int i;
  for (i = 0; i < 6; i++) {
    add_new_food(board);
  }

  while(true) {
    clear();
    display_points(board->snake, ACS_BLOCK);
    display_points(board->foods, ACS_DIAMOND);
    mvprintw(1, xmax-10, "Score: %d\n", (score)?*score: 0);
    refresh();
    dir = get_next_move(dir);
    enum Status status = move_snake(board, dir, score);
    if (status == FAILURE) break;
  }

  attron(A_BOLD);
  mvprintw(ymax/2, (xmax - strlen(game_over))/2, game_over);
  refresh();
  sleep(2);
  endwin();
  free(score);
  return 0;
}
