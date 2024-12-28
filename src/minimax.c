#include <stdlib.h>
#include <stdio.h>
#include "arbre.h"
#include <stdbool.h>

#define PLAYER1 1
#define PLAYER2 -1
#define INFINITY 1000


int end_game(int matrix[3][3], int who1, int who2) {
    for (int i = 0; i < 3; i++) {
        if (matrix[i][0] == matrix[i][1] && matrix[i][1] == matrix[i][2]) {
            if (matrix[i][0] == who1) return 10;
            if (matrix[i][0] == who2) return -10;
        }
        if (matrix[0][i] == matrix[1][i] && matrix[1][i] == matrix[2][i]) {
            if (matrix[0][i] == who1) return 10;
            if (matrix[0][i] == who2) return -10;
        }
    }
    if (matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2]) {
        if (matrix[0][0] == who1) return 10;
        if (matrix[0][0] == who2) return -10;
    }
    if (matrix[0][2] == matrix[1][1] && matrix[1][1] == matrix[2][0]) {
        if (matrix[0][2] == who1) return 10;
        if (matrix[0][2] == who2) return -10;
    }
    return 0;
}

void get_possible_move(int matrix[3][3], int list[9][2], int *count) {
    int cnt = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrix[i][j] == 0) {
                list[cnt][0] = i;
                list[cnt][1] = j;
                cnt++;
            }
        }
    }
    *count = cnt;
}

int minimaxe(int matrix[3][3], bool maximizer) {
    int score = end_game(matrix, PLAYER2, PLAYER1);
    if (score == 10 || score == -10) return score;

    int possible_moves[9][2];
    int count;
    get_possible_move(matrix, possible_moves, &count);

    if (count == 0) return 0;

    if (maximizer) {
        int best = -INFINITY;
        for (int i = 0; i < count; i++) {
            int row = possible_moves[i][0];
            int col = possible_moves[i][1];
            matrix[row][col] = PLAYER2;
            int value = minimaxe(matrix, !maximizer);
            best = (best < value) ? value : best;
            matrix[row][col] = 0;
        }
        return best;
    } else {
        int best = INFINITY;
        for (int i = 0; i < count; i++) {
            int row = possible_moves[i][0];
            int col = possible_moves[i][1];
            matrix[row][col] = PLAYER1;
            int value = minimaxe(matrix, !maximizer);
            best = (best > value) ? value : best;
            matrix[row][col] = 0;
        }
        return best;
    }
}

void get_best_move(int matrix[3][3], int *best_row, int *best_col) {
    int best_val = -INFINITY;
    *best_row = -1;
    *best_col = -1;

    int possible_moves[9][2];
    int count;
    get_possible_move(matrix, possible_moves, &count);

    for (int i = 0; i < count; i++) {
        int row = possible_moves[i][0];
        int col = possible_moves[i][1];
        matrix[row][col] = PLAYER2;
        int move_val = minimaxe(matrix, false);
        matrix[row][col] = 0;

        if (move_val > best_val) {
            *best_row = row;
            *best_col = col;
            best_val = move_val;
        }
    }
}

