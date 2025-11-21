/*

 /////////////////////// BASIC GAME MECHANISM \\\\\\\\\\\\\\\\\\\\\\\\

 start-> 1.New game 
         2.Load game
 
 [DONE] initiate game -> array of revelaed places, flagged places, mine places

 [DONE] place random mines -> array of mine places

  a function used to calculate the numbers around a mine.

 display 9x9 dot matrix -> not an array it self
 
 a function used to reveald places

 a function used to flag places
 
 a function used to show the mines after the game was over

 a function to handle FILE -> save & load

 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#define ROWS 9
#define COLS 9
#define MINES 10

// Macros: Making my life easier
#define FOR_EACH_ROW(i) for (int i = 0; i < ROWS; i++)
#define FOR_EACH_COL(j) for (int j = 0; j < COLS; j++)

// The heart of the game
typedef struct {
    int mines[ROWS][COLS];      // Stores the places of mines
    int flagged[ROWS][COLS];    // Stores the number of mines around each safe place
    int revealed[ROWS][COLS];   // stores the boolean value of places revealed by user
    int gameover;
    int won;
    int once;
} Game;

// Prototypes
void initGame(Game *);
void placeMines(Game *);
void calculateNumbers(Game *);
void gameBoard(Game *, bool);
void revealMines(Game *, int, int);
void toggleFlag(Game *, int, int);
void revealZeros(Game *);

// For testing purpose only
void pArray(Game *);

void main() {
    Game g;
    int choice, r, c, action;

    srand(time(NULL));

    printf("\n\t|-------------MINESWEEPER-------------|\n");
    printf("\t|1. NEW GAME                          |\n");
    printf("\t|2. LOAD GAME                         |\n");
    printf("\t|_____________________________________|\n");
    printf("\tChoice: ");
    scanf("%d", &choice);

    if (choice > 2 || choice < 1)
        printf("\nWRONG OPTION, try again..\n");
    else {
        initGame(&g);
        placeMines(&g);
        calculateNumbers(&g);

        while(!g.gameover) {
            // system("cls");
            gameBoard(&g, false);

            printf("\n\nEnter row number (0-9): ");
            scanf("%d", &r);
            printf("\nEnter column number (0-9): ");
            scanf("%d", &c);

            printf("\n1. reveal  2. Flag  3. Save & Exit");
            printf("\nAction: ");
            scanf("%d", &action);

            if (action == 1)
                revealMines(&g, r, c);
            else if (action == 2)
                toggleFlag(&g, r, c);
        }
    }
    
}

void initGame(Game *g) {
    FOR_EACH_ROW(i) {
        FOR_EACH_COL(j) {
            g->mines[i][j] = 0;
            g->flagged[i][j] = 0;
            g->revealed[i][j] = 0;
        }
    }
    g->gameover = 0;
    g->won = 0;
    g->once = 0;
}

void placeMines(Game *g) {
    int placed = 0;
    while (placed < MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;

        if (g->mines[r][c] == -1) continue;
        g->mines[r][c] = -1;
        placed++;
    }    

    pArray(g);
}

/*
 * calculating the numbers of mines around each safe places
 *
 * (-1, -1)   (0, -1)   (1, -1)
 * (-1, 0)    (0, 0)    (1, 0)
 * (-1, 1)    (0, 1)    (1, 1)
*/
void calculateNumbers(Game *g) {
    int coorX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int coorY[] = {-1, 0, 1, 1, -1, -1, 1, 0};
    
    FOR_EACH_ROW(i) {
        FOR_EACH_COL(j) {
            int count = 0;
            if (g->mines[i][j] != -1) {

                for (int k = 0; k < 8; k++) {
                    int dx = i + coorX[k];
                    int dy = j + coorY[k];

                    if (dx >= 0 && dx < ROWS && dy >= 0 && dy < COLS) {
                        if (g->mines[dx][dy] == -1) count++;
                    }
                }
            
            g->mines[i][j] = count;
            }
        }
    }

    pArray(g);
    
}

void gameBoard(Game *g, bool showGame) {
    printf("   ");
    FOR_EACH_COL(j) {
        printf("%d ", j);
    }

    printf("\n");
    printf("   ");
    FOR_EACH_COL(j) {
        printf("- ");
    }

    FOR_EACH_ROW(i) {
        printf("\n%d|", i);
        FOR_EACH_COL(j) {
            if (g->flagged[i][j] && !showGame) {
                printf(" F");
            } else if (!g->revealed[i][j] && !g->flagged[i][j] && !showGame) {
                printf(" .");
            } else if (g->mines[i][j] == 0 && g->revealed[i][j]) {
                printf("  ");
            } else if (g->revealed[i][j] && !showGame && g->mines[i][j] != -1){
                printf(" %d", g->mines[i][j]);
            } else if(g->revealed[i][j] && g->mines[i][j] == -1 && showGame) {
                printf(" *");
            }
        }
    }
}

void revealMines(Game *g, int r, int c) {
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return; // TODO[understand]: question
    if (g->revealed[r][c]) return;

    // Reveals the current cell
    g->revealed[r][c] = 1;

    // If zero, then reveals the neigbour cells
    // And does a recursion, which reveals all the safe blocks in the game board
    if (g->mines[r][c] == 0) {
        int coorX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int coorY[] = {-1, 0, 1, 1, -1, -1, 1, 0};

        for (int k = 0; k < 8; k++) {
            revealMines(g, r + coorX[k], c + coorY[k]);
        }
    }

    if (g->mines[r][c] == -1) {
        g->gameover = 1;
        return;
    }
            
}

void toggleFlag(Game *g, int r, int c) {
    if (!g->flagged[r][c])
        g->flagged[r][c] = !g->flagged[r][c];
}

void loadGame() {}

void pArray (Game *g) {
    printf("\n\n");
    FOR_EACH_ROW(i) {
        FOR_EACH_COL(j) {
            printf("%2d", g->mines[i][j]);
        }
        printf("\n");
    }
}
