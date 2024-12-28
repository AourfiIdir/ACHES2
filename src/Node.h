#ifndef Node_h
#define Node_h
typedef struct Node
{
    int val;
    int pos;
    struct Node *right;
    struct Node *left;
    struct Node *down;
    struct Node *up;
    struct Node *rd;
    struct Node *rup;
    struct Node *ld;
    struct Node *lup;
    
}Node;
#endif