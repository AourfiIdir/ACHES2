
#ifndef ARBRE_H
#define ARBRE_h
#include "Node.h"
#include <stdbool.h>
Node *Creat_Node(int val,int pos);
void linking(int pos,Node *n11,Node *n12,Node *n13,Node *n21,Node *n22,Node *n23,Node *n31,Node *n32,Node *n33);
void linking_all_nodes(Node *n11,Node *n12,Node *n13,Node *n21,Node *n22,Node *n23,Node *n31,Node *n32,Node *n33);
void placement(int where,int who,Node *n11,Node *n12,Node *n13,Node *n21,Node *n22,Node *n23,Node *n31,Node *n32,Node *n33);
void draw_board(Node *n11,Node *n12,Node *n13,Node *n21,Node *n22,Node *n23,Node *n31,Node *n32,Node *n33);
int check_if_solid_movement(int where,Node *n11,Node *n12,Node *n13,Node *n21,Node *n22,Node *n23,Node *n31,Node *n32,Node *n33);
void deplacement(int who,Node *n11, Node *n12, Node *n13, Node *n21, Node *n22, Node *n23, Node *n31, Node *n32, Node *n33);
void check_win(int *gameover,int who,Node *n11, Node *n12, Node *n13, Node *n21, Node *n22, Node *n23, Node *n31, Node *n32, Node *n33);
void create_all_nodes(Node *n11,Node *n12,Node *n13,Node *n21,Node *n22,Node *n23,Node *n31,Node *n32,Node *n33);
void init_board(int board[3][3]);
int check_winner_board(int board[3][3]);
#endif