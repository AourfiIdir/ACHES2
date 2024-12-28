#include "arbre.h"
#include <stdio.h>
#include <stdlib.h>
#include "Node.h"
Node *Creat_Node(int val,int pos){
    Node *q=NULL;
    q = (Node*)malloc(sizeof(Node));
    q->val=val;
    q->pos = pos;
    return q;
}
void create_all_nodes(Node *n11,Node *n12,Node *n13,Node *n21,Node *n22,Node *n23,Node *n31,Node *n32,Node *n33){
    n11=Creat_Node(0,11);
    n12=Creat_Node(0,12);
    n13=Creat_Node(0,13);
    n21=Creat_Node(0,21);
    n22=Creat_Node(0,22);
    n23=Creat_Node(0,23);
    n31=Creat_Node(0,31);
    n32=Creat_Node(0,32);
    n33=Creat_Node(0,33);
}
void linking_all_nodes(Node *n11,Node *n12,Node *n13,Node *n21,Node *n22,Node *n23,Node *n31,Node *n32,Node *n33){
    linking(n11->pos,n11,n12,n13,n21,n22,n23,n31,n32,n33);
    linking(n12->pos,n11,n12,n13,n21,n22,n23,n31,n32,n33);
    linking(n13->pos,n11,n12,n13,n21,n22,n23,n31,n32,n33);
    linking(n21->pos,n11,n12,n13,n21,n22,n23,n31,n32,n33);
    linking(n22->pos,n11,n12,n13,n21,n22,n23,n31,n32,n33);
    linking(n23->pos,n11,n12,n13,n21,n22,n23,n31,n32,n33);
    linking(n31->pos,n11,n12,n13,n21,n22,n23,n31,n32,n33);
    linking(n32->pos,n11,n12,n13,n21,n22,n23,n31,n32,n33);
    linking(n33->pos,n11,n12,n13,n21,n22,n23,n31,n32,n33);
}

void linking(int pos,Node *n11,Node *n12,Node *n13,Node *n21,Node *n22,Node *n23,Node *n31,Node *n32,Node *n33){
    switch (pos){
        case 11:
            n11->right=n12;
            n11->down=n21;
            n11->rd=n22;
            n11->ld=NULL;
            n11->left=NULL;
            n11->up=NULL;
            n11->lup=NULL;
            n11->rup=NULL;
            
            break;
        case 12:
            n12->right=n13;
            n12->down=n22;
            n12->rd=NULL;
            n12->ld=NULL;
            n12->left=n11;
            n12->up=NULL;
            n12->lup=NULL;
            n12->rup=NULL;
            break;
        case 13:
            n13->right=NULL;
            n13->down=n23;
            n13->rd=NULL;
            n13->ld=n22;
            n13->left=n12;
            n13->up=NULL;
            n13->lup=NULL;
            n13->rup=NULL;
            break;
        case 21:
            n21->right=n22;
            n21->down=n31;
            n21->rd=NULL;
            n21->ld=NULL;
            n21->left=NULL;
            n21->up=n11;
            n21->lup=NULL;
            n21->rup=NULL;
            break;
        case 22:
            n22->right=n23;
            n22->down=n32;
            n22->rd=n33;
            n22->ld=n31;
            n22->left=n21;
            n22->up=n12;
            n22->lup=n11;
            n22->rup=n13;
            break;
        case 23:
            n23->right=NULL;
            n23->down=n33;
            n23->rd=NULL;
            n23->ld=NULL;
            n23->left=n22;
            n23->up=n13;
            n23->lup=NULL;
            n23->rup=NULL;
            break;
        case 31:
            n31->right=n32;
            n31->down=NULL;
            n31->rd=NULL;
            n31->ld=NULL;
            n31->left=NULL;
            n31->up=n21;
            n31->lup=NULL;
            n31->rup=n22;
            break;
        case 32:
            n32->right=n33;
            n32->down=NULL;
            n32->rd=NULL;
            n32->ld=NULL;
            n32->left=n31;
            n32->up=n22;
            n32->lup=NULL;
            n32->rup=NULL;
            break;
        case 33:
            n33->right=NULL;
            n33->down=NULL;
            n33->rd=NULL;
            n33->ld=NULL;
            n33->left=n32;
            n33->up=n23;
            n33->lup=n22;
            n33->rup=NULL;
            break;
        
    }
}
//placement ##done##
void placement(int where,int who, Node *n11, Node *n12, Node *n13, Node *n21, Node *n22, Node *n23, Node *n31, Node *n32, Node *n33) {
    
    Node *nodes[] = {n11, n12, n13, n21, n22, n23, n31, n32, n33};
    int valid=0;
    for (int i = 0; i < 9; i++) {
        if (nodes[i]->pos == where) {
                if (check_if_solid_movement(where, n11, n12, n13, n21, n22, n23, n31, n32, n33) == 1) {
                nodes[i]->val = who;
                valid=1;
                } else {
                printf("The place is not empty");
                }
                break;
        }
    }
     // If the position is not found in the nodes
}
//check position ##done##
int check_if_solid_movement(int where, Node *n11, Node *n12, Node *n13, Node *n21, Node *n22, Node *n23, Node *n31, Node *n32, Node *n33) {
    Node *nodes[] = {n11, n12, n13, n21, n22, n23, n31, n32, n33};
    for (int i = 0; i < 9; i++) {
        if (nodes[i]->pos == where) {
            return (nodes[i]->val == 0) ? 1 : 0;//the case is empty
        }
    }
    return 0;  // Invalid position
}

void deplacement(int who,Node *n11, Node *n12, Node *n13, Node *n21, Node *n22, Node *n23, Node *n31, Node *n32, Node *n33){
    Node *nodes[] = {n11, n12, n13, n21, n22, n23, n31, n32, n33};
    int pos_t;
    int where;
    int valid;
    do{ 
        valid=0;
        printf("quelle piece a deplace player %d",who);
        scanf("%d",&pos_t);
        printf("where");
        scanf("%d",&where);
        if(check_if_solid_movement(where,n11,n12,n13,n21,n22,n23,n31,n32,n33)==1){
            valid=1;
            for(int i=0;i<9;i++){
                if(nodes[i]->pos==pos_t){
                    nodes[i]->val=0;
                    break;
                }
            }
            for(int j=0;j<9;j++){
                        if(nodes[j]->pos==where){
                            nodes[j]->val=who;
                            break;
                        }
                    }
        }else{
            printf("no valid");
        }
    }while(valid==0);
    

}

void draw_board(Node *n11, Node *n12, Node *n13, Node *n21, Node *n22, Node *n23, Node *n31, Node *n32, Node *n33){
    printf("\n%d | %d | %d\n",n11->val,n12->val,n13->val);
    printf("---------\n");
    printf("%d | %d | %d\n",n21->val,n22->val,n23->val);
    printf("---------\n");
    printf("%d | %d | %d",n31->val,n32->val,n33->val);
}
void check_win(int *gameover, int who, Node *n11, Node *n12, Node *n13, Node *n21, Node *n22, Node *n23, Node *n31, Node *n32, Node *n33) {
    // Check rows
    if ((n11->val == who && n12->val == who && n13->val == who) ||
        (n21->val == who && n22->val == who && n23->val == who) ||
        (n31->val == who && n32->val == who && n33->val == who)) {
        *gameover = 1;
        printf("\nTHE WINNER IS PLAYER %d\n", who);
        return;
    }
    // Check columns
    if ((n11->val == who && n21->val == who && n31->val == who) ||
        (n12->val == who && n22->val == who && n32->val == who) ||
        (n13->val == who && n23->val == who && n33->val == who)) {
        *gameover = 1;
        printf("\nTHE WINNER IS PLAYER %d\n", who);
        return;
    }
    // Check diagonals
    if ((n11->val == who && n22->val == who && n33->val == who) ||
        (n13->val == who && n22->val == who && n31->val == who)) {
        *gameover = 1;
        printf("\nTHE WINNER IS PLAYER %d\n", who);
        return;
    }
}
void init_board(int board[3][3]){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            board[i][j]=0;
        }
    }
}
int check_winner_board(int board[3][3])
{
    // Check rows for a winner
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            return board[i][0];
        }
    }

    // Check columns for a winner
    for (int j = 0; j < 3; j++)
    {
        if (board[0][j] != 0 && board[0][j] == board[1][j] && board[1][j] == board[2][j])
        {
            return board[0][j];
        }
    }

    // Check diagonals for a winner
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        return board[0][0];
    }

    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        return board[0][2];
    }

    // No winner
    return 0;
}
