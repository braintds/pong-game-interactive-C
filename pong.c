#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define SCALE_X 80
#define SCALE_Y 25
#define BALL_CODE 7

void game();
int movePlayer(int y, const int vector);
int moveBall(int X, int Y, int vector, const int player1, const int player2);
void printTable(const int ballX, int const ballY, int const player1,
                int const player2, const int scoreFirstPlayer,
                const int scoreSecondPlayer);

int main() {
  game();
  // printTable(12, 12, 5, 5);
  return 0;
}

// y: player position, vector: 0 - up, 1 - down
// add scales of player
int movePlayer(int y, const int vector) {
  switch (vector) {
    case 0:
      if (y + 3 < SCALE_Y) y++;
      break;
    case 1:
      if (y - 2 > 0) y--;
      break;
    default:
      break;
  }
  return y;
}

int moveBall(int X, int Y, int vector, const int player1, const int player2) {
  switch (vector) {
    case 1:
      X++;
      Y++;
      if (X == 75 && (Y == player2 || Y == player2 + 1 || Y == player2 - 1))
        vector = -1;
      if (Y == 24) vector = 2;
      break;
    case 2:
      X++;
      Y--;
      if (X == 75 && (Y == player2 || Y == player2 + 1 || Y == player2 - 1))
        vector = -2;
      if (Y == 0) vector = 1;
      break;
    case -1:
      X--;
      Y++;
      if (X == 4 && (Y == player1 || Y == player1 + 1 || Y == player1 - 1))
        vector = 1;
      if (Y == 24) vector = -2;
      break;
    case -2:
      X--;
      Y--;
      if (X == 4 && (Y == player2 || Y == player2 + 1 || Y == player2 - 1))
        vector = 2;
      if (Y == 0) vector = -1;
      break;
    default:
      break;
  }
  return vector;
}

void printTable(const int ballX, int const ballY, int const player1,
                int const player2, const int scoreFirstPlayer,
                const int scoreSecondPlayer) {
  for (int i = 0; i < SCALE_Y; i++)  // i = y coordinate
  {
    for (int j = 0; j < SCALE_X; j++)  // j = x coordinate
    {
      // print downside and upside lines
      if ((i == 0 || i == 24) && !(i == 0 && j >= 26 && j <= 50)) {
        printw("-");
        continue;
      }

      // ball print
      if (j == ballX && i == ballY) {
        printw(".");
        continue;
      }

      // print leftside and rightside lines
      if (j == 1 || j == 78) {
        printw("|");
        continue;
      }

      // print player1 racket
      if ((i == player1 + 1 || i == player1 || i == player1 - 1) && j == 4) {
        printw("|");
        continue;
      }

      // print player2 racket
      if ((i == player2 + 1 || i == player2 || i == player2 - 1) && j == 75) {
        printw("|");
        continue;
      }

      // print scoreboard
      if (i == 0 && j == 26) {
        printw("Player 1: %02d  Player 2: %02d", scoreFirstPlayer,
               scoreSecondPlayer);
        j = 51;
      }

      // print central line
      if (j == 39) printw("|");

      printw(" ");
    }
    printw("\n");
  }
}

void game() {
  int scoreFirstPlayer = 0;
  int scoreSecondPlayer = 0;
  int player1 = 13, player2 = 13;
  initscr();
  raw();
  cbreak();
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  int ballX = 36, ballY = 13, vector = 1;
  while (1) {
    char action;
    if (ballX >= 78) {
      vector = 1;
      scoreFirstPlayer++;
      ballX = 36;
      ballY = 13;
    }

    if (ballX <= 2) {
      vector = -2;
      scoreSecondPlayer++;
      ballX = 36;
      ballY = 13;
    }

    printTable(ballX, ballY, player1, player2, scoreFirstPlayer,
               scoreSecondPlayer);
    refresh();

    if (scoreFirstPlayer == 21) {
      printf("\nPlayer 1 WIN!\n");
      return;
    }

    if (scoreSecondPlayer == 21) {
      printw("\nPlayer 2 WIN!\n");
      return;
    }
    // read control key
    action = getch();

    // move player's racket
    switch (action) {
      case 'A':
        player1 = movePlayer(player1, 1);
        break;
      case 'Z':
        player1 = movePlayer(player1, 0);
        break;
      case 'K':
        player2 = movePlayer(player2, 1);
        break;
      case 'M':
        player2 = movePlayer(player2, 0);
        break;
      case ' ':
        break;
      default:
        break;
    }

    // calculate vector and move ball
    vector = moveBall(ballX, ballY, vector, player1, player2);
    switch (vector) {
      case 1:
        ballX++;
        ballY++;
        break;
      case 2:
        ballX++;
        ballY--;
        break;
      case -1:
        ballX--;
        ballY++;
        break;
      case -2:
        ballX--;
        ballY--;
        break;
      default:
        break;
    }
    usleep(75000);

    printTable(ballX, ballY, player1, player2, scoreFirstPlayer,
               scoreSecondPlayer);
  }
}