#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isANumber(const char* input) {
  bool hasDecimal = false;
  int len = strlen(input);

  if (len == 0) return 0;

  if (len == 1 && (input[0] == '.' || input[0] == '-')) return false;

  if (len == 2 && input[0] == '-' && input[1] == '.') return false;

  for (int i = 0; i < len; i++) {
    if (input[i] == '.') {
      if (hasDecimal) return false;

      hasDecimal = true;
    } else if (input[i] == '-' && i == 0) {
    } else if (!isdigit((unsigned char)input[i]))
      return false;
  }

  return true;
}

int checkInput(const char* prompt, int minRange, int maxRange) {
  char buffer[100];

  while (true) {
    printf("%sPlease enter a number (%d - %d): ", prompt, minRange, maxRange);

    if (!fgets(buffer, sizeof(buffer), stdin)) continue;

    buffer[strcspn(buffer, "\n")] = 0;

    if (!isANumber(buffer)) {
      printf("Not even a number!\n\n");
      continue;
    }

    char* end;
    double input = strtod(buffer, &end);

    int hasDecimal = strchr(buffer, '.') != NULL;

    if (hasDecimal) {
      double intPart;
      if (modf(input, &intPart) != 0.0) {
        printf("Must be a whole number!\n\n");
        continue;
      }
    }

    if (input < minRange || input > maxRange) {
      printf("Number out of range!\n\n");
    } else {
      return (int)input;
    }
  }
}

void drawBoard(char spaces[], int size) {
  for (int i = 0; i < size; i++) {
    int ele = 0;
    for (int x = 0; x < size; x++) {
      printf("     ");
      if (x != size - 1) printf("|");
    }
    printf("\n");

    for (int y = 0; y < size; y++) {
      printf("  %c  ", spaces[ele]);
      ele++;
      if (y != size - 1) printf("|");
    }
    printf("\n");

    if (i != size - 1) {
      for (int z = 0; z < size; z++) {
        printf("_____");
        if (z != size - 1) printf("|");
      }
      printf("\n");
    }
  }
  for (int i = 0; i < size; i++) {
    printf("     ");
    if (i != size - 1) printf("|");
  }
  printf("\n");
}

bool isMovesLeft(char spaces[]) {
  for (int i = 0; i < 9; i++)
    if (spaces[i] == ' ') return true;

  return false;
}

int evaluateBoard(char spaces[], char player, char computer) {
  for (int row = 0; row < 3; row++) {
    if (spaces[row * 3] == spaces[row * 3 + 1] &&
        spaces[row * 3 + 1] == spaces[row * 3 + 2]) {
      if (spaces[row * 3] == computer) return 10;
      if (spaces[row * 3] == player) return -10;
    }
  }

  for (int col = 0; col < 3; col++) {
    if (spaces[col] == spaces[col + 3] && spaces[col + 3] == spaces[col + 6]) {
      if (spaces[col] == computer) return 10;
      if (spaces[col] == player) return -10;
    }
  }

  if (spaces[0] == spaces[4] && spaces[4] == spaces[8]) {
    if (spaces[0] == computer) return 10;
    if (spaces[0] == player) return -10;
  }

  if (spaces[2] == spaces[4] && spaces[4] == spaces[6]) {
    if (spaces[2] == computer) return 10;
    if (spaces[2] == player) return -10;
  }

  return 0;
}

int minimax(char spaces[], int depth, bool isMax, char player, char computer) {
  int score = evaluateBoard(spaces, player, computer);

  if (score == 10) return score - depth;
  if (score == -10) return score + depth;

  if (!isMovesLeft(spaces)) return 0;

  if (isMax) {
    int best = INT_MIN;

    for (int i = 0; i < 9; i++) {
      if (spaces[i] == ' ') {
        spaces[i] = computer;
        int val = minimax(spaces, depth + 1, false, player, computer);
        spaces[i] = ' ';
        if (val > best) best = val;
      }
    }
    return best;
  } else {
    int best = INT_MAX;

    for (int i = 0; i < 9; i++) {
      if (spaces[i] == ' ') {
        spaces[i] = player;
        int val = minimax(spaces, depth + 1, true, player, computer);
        spaces[i] = ' ';
        if (val < best) best = val;
      }
    }
    return best;
  }
}

void computerMove(char spaces[], char computer, char player) {
  int bestVal = INT_MIN;
  int bestMove = -1;

  for (int i = 0; i < 9; i++) {
    if (spaces[i] == ' ') {
      spaces[i] = computer;
      int moveVal = minimax(spaces, 0, false, player, computer);
      spaces[i] = ' ';

      if (moveVal > bestVal) {
        bestVal = moveVal;
        bestMove = i;
      }
    }
  }

  spaces[bestMove] = computer;
}

void playerMove(char spaces[], char player, int size) {
  while (true) {
    int number = checkInput("Enter a spot to place a marker: ", 1, size);

    if (spaces[number - 1] == ' ') {
      spaces[number - 1] = player;
      break;
    }
  }
}

int checkWinner(char spaces[], char player, char computer) {
  int score = evaluateBoard(spaces, player, computer);
  if (score == 10) {
    printf("YOU LOSE!\n");
    return 1;
  }
  if (score == -10) {
    printf("YOU WIN!!\n");
    return 1;
  }
  return 0;
}

bool checkTie(char spaces[]) {
  if (!isMovesLeft(spaces)) {
    printf("IT'S A TIE!\n");
    return true;
  }
  return false;
}

int main() {
  int x = 0;
  while (x < 3 || x > 15) {
    printf("how big of a board (from 3 to 15):\n)");
    scanf("%d", &x);
    if (x < 3 || x > 15) {
      printf("out of range\n");
    }
    while (getchar() != '\n');
  }

  char spaces[x * x];
  for (int i = 0; i < x * x; i++) spaces[i] = ' ';

  char player = 'X';
  char computer = 'O';

  drawBoard(spaces, x);

  while (true) {
    playerMove(spaces, player, x);
    system("CLS");
    drawBoard(spaces, x);
    if (checkWinner(spaces, player, computer) || checkTie(spaces)) break;

    system("CLS");
    computerMove(spaces, computer, player);
    drawBoard(spaces, x);
    if (checkWinner(spaces, player, computer) || checkTie(spaces)) break;
  }

  printf("Thanks for playing!\n");
  return 0;
}
