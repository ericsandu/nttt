#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 3

char board[BOARD_SIZE][BOARD_SIZE];
bool turn_X;
int cur_row = 0, cur_col = 0;

static void draw_board(void) {
  clear();
  int start_y = 2, start_x = 4;

  for (int r = 0; r < BOARD_SIZE; ++r) {
    for (int c = 0; c < BOARD_SIZE; ++c) {
      int y = start_y + r * 2;
      int x = start_x + c * 4;

      if (r == cur_row && c == cur_col) {
        attron(A_REVERSE);
        mvprintw(y, x, " %c ", board[r][c]);
        attroff(A_REVERSE);
      } else {
        mvprintw(y, x, " %c ", board[r][c]);
      }

      if (c < BOARD_SIZE - 1)
        mvprintw(y, x + 3, "|");
    }

    if (r < BOARD_SIZE - 1)
      mvprintw(start_y + r * 2 + 1, start_x, "---+---+---");
  }

  mvprintw(start_y + 7, start_x,
           "Player %c's turn. Arrow keys to move, Space/Enter to place.",
           turn_X ? 'X' : 'O');
  refresh();
}

static char check_winner(void) {
  for (int i = 0; i < BOARD_SIZE; ++i) {
    if (board[i][0] != ' ' && board[i][0] == board[i][1] &&
        board[i][1] == board[i][2])
      return board[i][0];
    if (board[0][i] != ' ' && board[0][i] == board[1][i] &&
        board[1][i] == board[2][i])
      return board[0][i];
  }

  if (board[0][0] != ' ' && board[0][0] == board[1][1] &&
      board[1][1] == board[2][2])
    return board[0][0];
  if (board[0][2] != ' ' && board[0][2] == board[1][1] &&
      board[1][1] == board[2][0])
    return board[0][2];

  return '\0';
}

static bool board_full(void) {
  for (int r = 0; r < BOARD_SIZE; ++r)
    for (int c = 0; c < BOARD_SIZE; ++c)
      if (board[r][c] == ' ')
        return false;
  return true;
}

int main(void) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  srand((unsigned)time(NULL));
  turn_X = (rand() % 2 == 0);

  for (int r = 0; r < BOARD_SIZE; ++r)
    for (int c = 0; c < BOARD_SIZE; ++c)
      board[r][c] = ' ';

  draw_board();

  while (true) {
    int ch = getch();
    switch (ch) {
    case KEY_UP:
      if (cur_row > 0)
        cur_row--;
      break;
    case KEY_DOWN:
      if (cur_row < BOARD_SIZE - 1)
        cur_row++;
      break;
    case KEY_LEFT:
      if (cur_col > 0)
        cur_col--;
      break;
    case KEY_RIGHT:
      if (cur_col < BOARD_SIZE - 1)
        cur_col++;
      break;

    case ' ':
    case '\n':
      if (board[cur_row][cur_col] == ' ') {
        board[cur_row][cur_col] = turn_X ? 'X' : 'O';

        char winner = check_winner();
        if (winner) {
          draw_board();
          mvprintw(12, 4, "Player %c wins! Press any key to exit.", winner);
          getch();
          endwin();
          return 0;
        }

        if (board_full()) {
          draw_board();
          mvprintw(12, 4, "It's a draw! Press any key to exit.");
          getch();
          endwin();
          return 0;
        }

        turn_X = !turn_X;
      }
      break;

    case 'q':
    case 'Q':
      endwin();
      return 0;
    }
    draw_board();
  }
}
