#include <stdio.h>
#include <stdlib.h>
#include <err.h>



int isAvailable(int puzzle[9][9], int row, int col, int num)
{
    int start_row = (row/3) * 3;    //Define the starting row
    int start_column = (col/3) * 3; //Define the starting column


    for(int i = 0; i < 9; ++i)
    {
        if (puzzle[row][i] == num) return 0; //Row not available
        if (puzzle[i][col] == num) return 0; //Col not available
        if (puzzle[start_row + (i%3)][start_column + (i/3)] == num) return 0; //Box not available
    }
    return 1; //Column or row or box is available
}

int solveSudoku(int puzzle[9][9], int row, int col)
{
    int i;
    if(row<9 && col<9)
    {
        if(puzzle[row][col] != 0) //Case not empty
        {
            if((col+1)<9) return solveSudoku(puzzle, row, col+1);    //We fill the grid column per column
            else if((row+1)<9) return solveSudoku(puzzle, row+1, 0); //We fill the grid row per row
            else return 1;
        }
        else
        {
            for(i=0; i<9; ++i)
            {
                if(isAvailable(puzzle, row, col, i+1))
                {
                    puzzle[row][col] = i+1; //We fill the case
                    if((col+1)<9) //If we did not fill all the columns
                    {
                        if(solveSudoku(puzzle, row, col +1)) return 1;
                        else puzzle[row][col] = 0;
                    }
                    else if((row+1)<9) //If we did not fill all the rows
                    {
                        if(solveSudoku(puzzle, row+1, 0)) return 1;
                        else puzzle[row][col] = 0;
                    }
                    else return 1;
                }
            }
        }
        return 0; //Sudoku can't be solved
    }
    else return 1; //Sudoku is solved
}



int FromFileToGrid(char* filename)
{
    int  grid[9][9];
    int i = 0;
    int j = 0;
    FILE* inputfile = fopen(filename,"r");
    if (inputfile == NULL)
        errx(1,"inputfile null");
    char c;
    
    while((c = fgetc(inputfile)) != EOF)
    {

        if(c == '\n' || c == '\0')
            continue;
        if(j==9)
        {
            j=0;
            i++;
        }
        if(c != '.')
            grid[i][j] = (int)(c-'0');
        else
            grid[i][j] = 0;
        j++;
    }
    
    for(int i = 0; i<9;i++)
    {
        for(int j = 0 ; j<9;j++)
        {
            printf("%d",grid[i][j]);
        }
        printf("\n");
    }
    return grid[9][9];
}

int main(int argc,char *argv[])
{
    if(argc != 2 )
        return 0;
    int test[9][9];    
    test[9][9] = FromFileToGrid(argv[1]);
    
    for(int i = 0; i<9;i++)
    {
        for(int j = 0 ; j<9;j++)
        {
            printf("%d",test[i][j]);
        }
        printf("\n");
    }
    //printf("AFTER PUZZLE");
    /*if(solveSudoku(puzzle, 0, 0))
    {
        printf("\n+-----+-----+-----+\n");
        for(int i=0; i<9; ++i)
        {
            for(int j=0; j<9; ++j) 
            {
                printf("|%d", puzzle[i-1][j-1]);
                printf("|\n");
            }
            if (i%3 == 0) printf("+-----+-----+-----+\n");
        }
    }
    else printf("\n\nNO SOLUTION\n\n");*/

    return 0;
}
