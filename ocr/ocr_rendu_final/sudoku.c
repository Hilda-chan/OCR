#include <stdio.h>

int isAvailable(int puzzle[][9], int row, int col, int num)
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

int solveSudoku(int puzzle[][9], int row, int col)
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

int main()
{
    int i, j;
    int puzzle[9][9]={{0, 0, 0, 0, 0, 0, 0, 9, 0},
                      {1, 9, 0, 4, 7, 0, 6, 0, 8},
                      {0, 5, 2, 8, 1, 9, 4, 0, 7},
                      {2, 0, 0, 0, 4, 8, 0, 0, 0},
                      {0, 0, 9, 0, 0, 0, 5, 0, 0},
                      {0, 0, 0, 7, 5, 0, 0, 0, 9},
                      {9, 0, 7, 3, 6, 4, 1, 8, 0},
                      {5, 0, 6, 0, 8, 1, 0, 7, 4},
                      {0, 8, 0, 0, 0, 0, 0, 0, 0}};

    if(solveSudoku(puzzle, 0, 0))
    {
        printf("\n+-----+-----+-----+\n");
        for(i=1; i<10; ++i)
        {
            for(j=1; j<10; ++j) printf("|%d", puzzle[i-1][j-1]);
            printf("|\n");
            if (i%3 == 0) printf("+-----+-----+-----+\n");
        }
    }
    else printf("\n\nNO SOLUTION\n\n");

    return 0;
}
