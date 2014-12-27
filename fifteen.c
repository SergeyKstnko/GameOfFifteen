 /**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;
// initializing empty tile coordinates 
int x, y;


// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d. Enter demensions of the board.\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();
    // initializing blank tile
    x = d - 1;
    y = d - 1;
    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        // move if possible, else report illegality
        if(!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    // Enumerating tiles for the game
    int k = d * d - 1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = k;
            k--;
        }
    }
    //check if #tiles is odd
    k = d * d - 1;
    if (k % 2)
    {
        int e = 0;
        e = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = e;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Drawing the board
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                printf(" _  ");
            }
            else
            {
                printf("%2d  ", board[i][j]);
            }
        }
        printf("\n\n");
     }

}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    //Memorize the initial blank tile position for the move
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            //Find position of the tile
            if (board[i][j] == tile)
            {
                //check if the tile is adjacent to the blank tile
                if (((i == x) && (j == y - 1 || j == y + 1)) || ((j == y) && (i == x - 1 || i == x + 1)))
                {
                    //assign intermediat variable
                    int e = 0;
                    //Swap value of the empty tile
                    e = board[i][j];
                    board[i][j] = board[x][y];
                    board[x][y] = e;
                    //Swaping coordinates of the empty tile and memorizing where it is located
                    e = i;
                    i = x;
                    x = e;
                    e = j;
                    j = y;
                    y = e;
                    printf("\nIt is moooooving\n");
                    return true;
                }             
            }
        }
     }
return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO Checking if the player won the game
    // Iterate over rows
    int k = 1;
    for (int i = 0; i < d; i++)
    {
        //Iterate over columns
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == k)
            {
                //printf("%d", k);
                k++;
                if (k == d * d)
                {
                    //printf("True returned, %d\n", k);
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        
    }
    
return false;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
