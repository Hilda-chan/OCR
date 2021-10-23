#include<stdio.h>
#include<stdlib.h>
#include<string.h>


// Take in parameters: the grid's name, pointers.
// Modify sudoku[] with grid's file.
// Update nkv[], nkv_poz.
// Return 0 if there's no error // else: return 1
int Initialize_sudoku(char grid_name[], unsigned long* p[]) {
 
    /*Open File*/
    FILE* unsolved_grid = fopen(grid_name, "r");
    if (unsolved_grid == NULL) {
        printf("Error when trying to open the file_to_read %s\n", grid_name);
        return -1;
    }

    /*Initiate pointers*/
    for (size_t i = 0; i < 81; i++) {
        p[1][i] = 0;    // p[1] == nkv
    }
    char c = fgetc(unsolved_grid);
    size_t i = 0;
    while (c != EOF && i < 81)
    {
        if ('1' <= c && c <= '9'){
            p[0][i] = c - '0';  // p[0][i] == sudoku[i]
            p[1][*p[2]] = i;    // p[1][j] == nkv[j] 
            *p[2] += 1;         // *p[2] == nkv_poz
            i++;
        }
        if (c == '.') {
            p[0][i] = 123456789;
            i++;
        }
        c = fgetc(unsolved_grid);  // c get the next caracter of unsolved_grid.
    }

    fclose(unsolved_grid);
    return 0;
}


// Take in parameters: sudoku, cell's poz, value to del
// Update sudoku[poz]: function deleting "value" of sudoku[poz]'s probs
void delete_proba(unsigned long sudoku[], size_t poz, unsigned long value) {
    unsigned long prob = sudoku[poz];
    unsigned long new_prob = 0;
    while (prob != 0) {
        if (prob % 10 != value) {
            new_prob = new_prob * 10 + prob % 10;
        }
        prob = prob / 10;
    }
    sudoku[poz] = new_prob;
}

// Take in parameters: pointers, poz of cell to modify, value of sudoku[x].
// Call delete_proba
// Update sudoku[poz]: if value belongs to sudoku[poz]
// Update nkv[], nkv_poz: if sudoku[poz] becomes a nkv
void __del_proba(unsigned long* p[], size_t poz, unsigned long value) {
    delete_proba(p[0], poz, value);
    if (p[0][poz] < 10) {
        p[1][*p[2]] = poz;
        *p[2] += 1;
    }
}


// Take in parameters: pointers, cell's poz (x), sudoku[x].
// Call __del_proba
// Update the row of sudoku[x], nkv[], nkv_poz.
void update_row_sudoku(unsigned long* p[], size_t x, unsigned long value) {

    size_t start = x - x % 9;
    for (size_t i = start; i < start + 9; i++) {
        if (p[0][i] > 10) {
            __del_proba(p, i, value);
        }
    }
}


// Take in parameters: pointers, cell's poz (x), sudoku[x].
// Call __del_proba
// Update the column of sudoku[x], nkv[], nkv_poz.
void update_col_sudoku(unsigned long* p[], size_t x, unsigned long value) {

    size_t start = x % 9;
    for (size_t j = start; j < 81; j += 9) {
        if (p[0][j] > 10) {
            __del_proba(p, j, value);
        }
    }
}


// Take in parameters: pointers, cell's poz (x), sudoku[x].
// Call __del_proba
// Update the block of sudoku[x], nkv[], nkv_poz.
void update_block_sudoku(unsigned long* p[], size_t x, unsigned long value) {

    size_t start = x / 27 * 27 + x % 9 / 3 * 3;
    for (size_t k = start; k < start + 27; k += 9) {
        for (size_t l = k; l < k + 3; l++) {
            if (p[0][l] > 10) {
                __del_proba(p, l, value);
            }
        }
    }
}


// Take in parameters: pointers, cell's poz.
// Call all update_sudoku functions
// Update sudoku[], nkv[] and nkv_poz.
void update_sudoku(unsigned long* p[], size_t x) {
    unsigned long value = p[0][x];
    update_row_sudoku(p, x, value);
    update_col_sudoku(p, x, value);
    update_block_sudoku(p, x, value);
}


int save(char grid_name[], unsigned long sudoku[]){
    FILE* new_file = fopen(grid_name, "w");
    if (new_file == NULL){
        printf("Error when trying to open the file_to_write %s\n", grid_name);
        return -1;
    }
    size_t n = 0;
    for(size_t i = 0; i < 3; i++){
        for(size_t j = 0; j < 3; j++){
            for(size_t k = 0; k < 3; k++){
                for(size_t l = 0; l < 3; l++){

                    unsigned long value = sudoku[n];
                    if(value > 10)
                        fputc('.', new_file);
                    else{
                        char c = value + '0';
                        fputc(c, new_file);
                        }
                    n++;
                }
                fputc(' ', new_file);
            }
            fputc('\n', new_file);
        }
        fputc('\n', new_file);
    }

    fclose(new_file);
    return 0;
}


int resolve(char grid_name[]) {

    /*Initiate sudoku and nkv*/
    unsigned long sudoku[81];
    size_t new_known_values[81];
    size_t nkv_poz = 0;

    unsigned long *pointer[3];

    pointer[0] = & sudoku[0];
    pointer[1] = & new_known_values[0];
    pointer[2] = & nkv_poz;

    if (Initialize_sudoku(grid_name, pointer) != 0)
        return -1;

    /*Resolve sudoku*/
    for (size_t i = 0; i < 80; i++)
    {
        update_sudoku(pointer, pointer[1][i]);
    }

    /*Write and save solved sudoku*/
    size_t size = strlen(grid_name) + strlen(".result");
    char* result_file_name = (char*)malloc(size * sizeof(char) + 1);
    *result_file_name = '\0';
    strcat(result_file_name, grid_name);
    strcat(result_file_name, ".result");
    if (save(result_file_name, pointer[0]) != 0)
        return -1;

    return 0;
}
