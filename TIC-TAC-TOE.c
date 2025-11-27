#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

// ==========================================================
//  BASIC INPUT CHECKING
// ==========================================================

bool isANumber(const char* input)
{
    bool hasDecimal = false;
    int len = strlen(input);

    if (len == 0) return false;

    if (len == 1 && (input[0] == '.' || input[0] == '-'))
        return false;

    for (int i = 0; i < len; i++)
    {
        if (input[i] == '.')
        {
            if (hasDecimal)
                return false;
            hasDecimal = true;
        }
        else if (input[i] == '-' && i == 0)
        {

        }
        else if (!isdigit((unsigned char)input[i]))
            return false;
    }
    return true;
}

int checkInput(const char* prompt, int minRange, int maxRange)
{
    char buffer[100];

    while (true)
    {
        printf("%sPlease enter a number (%d - %d): ", prompt, minRange, maxRange);

        if (!fgets(buffer, sizeof(buffer), stdin))
            continue;

        buffer[strcspn(buffer, "\n")] = 0;

        if (!isANumber(buffer))
        {
            printf("Not even a number!\n\n");
            continue;
        }

        char* end;
        double input = strtod(buffer, &end);

        if (strchr(buffer, '.') != NULL)
        {
            double intPart;
            if (modf(input, &intPart) != 0.0)
            {
                printf("Must be a whole number!\n\n");
                continue;
            }
        }

        if (input < minRange || input > maxRange)
            printf("Number out of range!\n\n");
        else
            return (int)input;
    }
}

// ==========================================================
//  DRAW BOARD
// ==========================================================

void drawBoard(char spaces[])
{
    printf("\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n", spaces[0], spaces[1], spaces[2]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n", spaces[3], spaces[4], spaces[5]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n", spaces[6], spaces[7], spaces[8]);
    printf("     |     |     \n\n");
}

// ==========================================================
//  GAME LOGIC
// ==========================================================

bool isMovesLeft(char spaces[])
{
    for (int i = 0; i < 9; i++)
        if (spaces[i] == ' ')
            return true;
    return false;
}

int evaluateBoard(char spaces[], char player, char computer)
{
    for (int row = 0; row < 3; row++)
    {
        if (spaces[row * 3] == spaces[row * 3 + 1] &&
            spaces[row * 3 + 1] == spaces[row * 3 + 2])
        {
            if (spaces[row * 3] == computer) 
                return 10;
            if (spaces[row * 3] == player)  
                return -10;
        }
    }
    for (int col = 0; col < 3; col++)
    {
        if (spaces[col] == spaces[col + 3] &&
            spaces[col + 3] == spaces[col + 6])
        {
            if (spaces[col] == computer) return 10;
            if (spaces[col] == player)   return -10;
        }
    }

    if (spaces[0] == spaces[4] && spaces[4] == spaces[8])
    {
        if (spaces[0] == computer) return 10;
        if (spaces[0] == player)   return -10;
    }

    if (spaces[2] == spaces[4] && spaces[4] == spaces[6])
    {
        if (spaces[2] == computer) return 10;
        if (spaces[2] == player)   return -10;
    }

    return 0;
}

int minimax(char spaces[], int depth, bool isMax, char player, char computer)
{
    int score = evaluateBoard(spaces, player, computer);

    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    if (!isMovesLeft(spaces)) return 0;

    if (isMax)
    {
        int best = INT_MIN;
        for (int i = 0; i < 9; i++)
        {
            if (spaces[i] == ' ')
            {
                spaces[i] = computer;
                best = (best > minimax(spaces, depth + 1, false, player, computer) ?
                    best :
                    minimax(spaces, depth + 1, false, player, computer));
                spaces[i] = ' ';
            }
        }
        return best;
    }
    else
    {
        int best = INT_MAX;
        for (int i = 0; i < 9; i++)
        {
            if (spaces[i] == ' ')
            {
                spaces[i] = player;
                best = (best < minimax(spaces, depth + 1, true, player, computer) ?
                    best :
                    minimax(spaces, depth + 1, true, player, computer));
                spaces[i] = ' ';
            }
        }
        return best;
    }
}

void computerMove(char spaces[], char computer, char player)
{
    int bestVal = INT_MIN;
    int bestMove = -1;

    for (int i = 0; i < 9; i++)
    {
        if (spaces[i] == ' ')
        {
            spaces[i] = computer;
            int v = minimax(spaces, 0, false, player, computer);
            spaces[i] = ' ';

            if (v > bestVal)
            {
                bestVal = v;
                bestMove = i;
            }
        }
    }

    spaces[bestMove] = computer;
}

void playerMove(char spaces[], char player)
{
    while (true)
    {
        int number = checkInput("Enter a spot to place a marker: ", 1, 9);

        if (spaces[number - 1] == ' ')
        {
            spaces[number - 1] = player;
            break;
        }
    }
}

int checkWinner(char spaces[], char player, char computer)
{
    int score = evaluateBoard(spaces, player, computer);

    if (score == 10)
    {
        printf("YOU LOSE!\n");
        return 1;
    }
    if (score == -10)
    {
        printf("YOU WIN!!\n");
        return 1;
    }
    return 0;
}

bool checkTie(char spaces[])
{
    if (!isMovesLeft(spaces))
    {
        printf("IT'S A TIE!\n");
        return true;
    }
    return false;
}

// ==========================================================
//  SAVE / LOAD SYSTEM
// ==========================================================

// Count number of existing saves
int countSaves()
{
    int count = 0;
    char name[20];

    for (int i = 1; i <= 3; i++)
    {
        sprintf(name, "save%d.txt", i);
        FILE* f = fopen(name, "r");
        if (f)
        {
            fclose(f);
            count++;
        }
    }
    return count;
}

// Shift filenames down
void dropSaveNumbers()
{
    for (int i = 1; i <= 3; i++)
    {
        char oldName[20];
        sprintf(oldName, "save%d.txt", i);

        // If missing, shift all above it down
        if (access(oldName, 0) != 0)
        {
            for (int j = i + 1; j <= 3; j++)
            {
                char oldN[20], newN[20];
                sprintf(oldN, "save%d.txt", j);
                sprintf(newN, "save%d.txt", j - 1);

                if (access(oldN, 0) == 0)
                    rename(oldN, newN);
            }
            return;
        }
    }
}

// Save game
void saveGame(char spaces[])
{
    int current = countSaves();

    if (current == 3)
    {
        printf("You have 3 saves. Delete one?\n");
        int del = checkInput("Delete which save (1-3): ", 1, 3);

        char target[20];
        sprintf(target, "save%d.txt", del);
        remove(target);

        dropSaveNumbers();

        current = countSaves();
    }

    char filename[20];
    sprintf(filename, "save%d.txt", current + 1);

    FILE* f = fopen(filename, "w");

    fprintf(f,
        "     |     |     \n"
        "  %c  |  %c  |  %c  \n"
        "_____|_____ |_____\n"
        "     |     |     \n"
        "  %c  |  %c  |  %c  \n"
        "_____|_____ |_____\n"
        "     |     |     \n"
        "  %c  |  %c  |  %c  \n"
        "     |     |     \n",
        spaces[0], spaces[1], spaces[2],
        spaces[3], spaces[4], spaces[5],
        spaces[6], spaces[7], spaces[8]);

    fclose(f);
    printf("Game saved to %s\n", filename);
}

bool loadGame(char spaces[])
{
    int saves = countSaves();
    if (saves == 0)
    {
        printf("No saves found.\n");
        return false;
    }

    printf("Which save do you want to load? (1-%d)\n", saves);
    int pick = checkInput("", 1, saves);

    char filename[20];
    sprintf(filename, "save%d.txt", pick);

    FILE* f = fopen(filename, "r");
    if (!f)
    {
        printf("Could not open save.\n");
        return false;
    }

    char line[100];
    int index = 0;

    while (fgets(line, sizeof(line), f))
    {
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == 'X' || line[i] == 'O' || line[i] == ' ')
            {
                if (index < 9)
                    spaces[index++] = line[i];
            }
        }
    }

    fclose(f);
    return true;
}

void playGame(bool loadedGame)
{
    char spaces[9];
    char player = 'X';
    char computer = 'O';

    if (!loadedGame)
    {
        for (int i = 0; i < 9; i++) spaces[i] = ' ';
    }
    else
    {

    }

    system("CLS");
    drawBoard(spaces);

    if (loadedGame)
    {
        printf("Computer moves first after loading...\n");
        computerMove(spaces, computer, player);
        system("CLS");
        drawBoard(spaces);
    }

    while (true)
    {
        playerMove(spaces, player);
        system("CLS");
        drawBoard(spaces);
        if (checkWinner(spaces, player, computer) || checkTie(spaces))
            break;

        computerMove(spaces, computer, player);
        system("CLS");
        drawBoard(spaces);
        if (checkWinner(spaces, player, computer) || checkTie(spaces))
            break;
    }

    printf("\nWould you like to save this game?\n");
    printf("1. Yes\n2. No\n");

    int ans = checkInput("", 1, 2);
    if (ans == 1)
        saveGame(spaces);
}

int main()
{
    while (true)
    {
        printf("==== TIC TAC TOE ====\n");
        printf("1. New Game\n");
        printf("2. Load Game\n");
        printf("3. Exit\n");

        int choice = checkInput("", 1, 3);

        if (choice == 1)
        {
            playGame(false);
        }
        else if (choice == 2)
        {
            char spaces[9];
            if (loadGame(spaces))
            {
                playGame(true);
            }
        }
        else if (choice == 3)
        {
            return 0;
        }
    }
}
