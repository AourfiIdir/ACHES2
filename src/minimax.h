#ifndef minimax_h
#define minimax_h
#include "arbre.h"
void get_best_move(int matrix[3][3], int *best_row, int *best_col);
int minimaxe(int matrix[3][3], bool maximizer);
void get_possible_move(int matrix[3][3], int list[9][2], int *count);
#endif