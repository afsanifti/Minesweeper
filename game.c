/*
 ////////////////////////////// BASIC GAME MECHANISM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

 start-> 1.New game 
         2.Load game
 
 [DONE] initiate game -> array of revelaed places, flagged places, mine places

 [DONE] place random mines -> array of mine places

 [DONE] a function used to calculate the numbers around a mine.

 [IN PROGRESS] display 9x9 dot matrix -> not an array it self
 
 [DONE] a function used to reveald places

 [DONE] a function used to flag places
 
 [NOT NEEDED] a function used to show the mines after the game was over

 a function to handle FILE -> save & load


 [TODO](Improve): [DONE] If possible try to place the mines and calulate the mines 
                  [DONE] after user has revealed the first place,
                  [DONE] and then reveal the empty places.
                  try to not place mine in the first reveal.
                  try to have more safe places around the first reveal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#define ROWS 12
#define COLS 12
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
void saveGame(Game *);
int loadGame(Game *);
bool checkWin(Game *);

// For testing purpose only
void pArray(Game *);

int main() {
    Game g;
    int choice, r, c, action;

    // So that random number are never the same each time
    srand(time(NULL));

    printf("\n\t|-------------MINESWEEPER-------------|\n");
    printf("\t|1. NEW GAME                          |\n");
    printf("\t|2. LOAD GAME                         |\n");
    printf("\t|_____________________________________|\n");
    printf("\tChoice: ");
    scanf("%d", &choice);

    if (choice > 2 || choice < 1)
        printf("\nWRONG OPTION, try again..\n");
    else if (choice == 2 && loadGame(&g)) {
        printf("\nGame loaded successfully.\n");
    } else {
        initGame(&g);

        // Counting if the openning cell has been revealed
    }

    int counter_first_reveal = 0;
    while(!g.gameover) {
        system("cls");
        gameBoard(&g, false);

        printf("\n\nEnter row number (0-9): ");
        scanf("%d", &r);
        printf("\nEnter column number (0-9): ");
        scanf("%d", &c);

        printf("\n1. reveal  2. Flag  3. Save & Exit");
        printf("\nAction: ");
        scanf("%d", &action);

        if (action == 1 && counter_first_reveal == 0) {
            placeMines(&g);
            calculateNumbers(&g);

            revealMines(&g, r, c);
            counter_first_reveal++;
        } else if (action == 1) {
            revealMines(&g, r, c);
        } else if (action == 2) {
            toggleFlag(&g, r, c);
        } else if (action == 3) {
            saveGame(&g);
            g.gameover = 0;
            return 0;
        }

        if (checkWin(&g)) {
            g.won = 1;
            g.gameover = 1;
        }
    }

    printf("\n");
    gameBoard(&g, true);

    if (g.won) {
        printf("\n\n!!Game Over!! \n Congratulations, you won the game!!");
    } else {
        printf("\n\n!!Game Over!! \n Sorry you lost. Try again...");
    }
        
    return 0;
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
}

void placeMines(Game *g) {
    int placed = 0;
    while (placed < MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;

        // if a mine is placed, we skip that position.
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

// Display game board matrix
void gameBoard(Game *g, bool showGame) {
    printf("    ");
    FOR_EACH_COL(j) {
        printf("%02d ", j);
    }

    printf("\n");
    printf("    ");
    FOR_EACH_COL(j) {
        printf("-  ");
    }

    FOR_EACH_ROW(i) {
        printf("\n\n%02d| ", i);
        FOR_EACH_COL(j) {
            if (g->flagged[i][j] && !showGame) {
                printf("F  ");
            } else if (!g->revealed[i][j] && !g->flagged[i][j] && !showGame) {
                printf(".  ");
            } else if (g->mines[i][j] == 0) {
                printf("   ");
            } else if(g->mines[i][j] == -1) {
                printf("*  ");
            } else {
                printf("%d  ", g->mines[i][j]);
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
    if (!g->revealed[r][c])
        g->flagged[r][c] = !g->flagged[r][c];
}

void saveGame(Game *g) {
    FILE *f = fopen("minesweeper_save.dat", "wb");

    if (f) {
        fwrite(g, sizeof(Game), 1, f);
        fclose(f);
    }
}

int loadGame(Game *g) {
    FILE *f = fopen("minesweeper_save.dat", "rb");

    if (f) {
        fread(g, sizeof(Game), 1, f);
        fclose(f);
        return 1;
    }
    return 0;
}

bool checkWin(Game *g) {
    int count = 0;
    FOR_EACH_ROW(i) {
        FOR_EACH_COL(j) {
            if (g->revealed[i][j]) count++;
        }
    }

    if (count == ROWS * COLS - MINES) return true;

    return false;
}

void pArray (Game *g) {
    printf("\n\n");
    FOR_EACH_ROW(i) {
        FOR_EACH_COL(j) {
            printf("%2d", g->mines[i][j]);
        }
        printf("\n");
    }
}
