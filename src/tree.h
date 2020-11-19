#include "define_hariak.h"

struct tree
{
    struct node *root;
    struct node *leftmost;
    int tamaina;
};


struct node
{
    struct process_control_block data; //node will store an integer
    struct node *right_child; // right child
    struct node *left_child; // left child
};

struct node* search(struct node *root, struct process_control_block x);
struct node* find_minimum(struct node *root);
struct process_control_block get_min(struct node *root);
struct node* new_node(struct process_control_block x);
struct node* insert(struct node *root, struct process_control_block x);
struct node* delete(struct node *root, struct process_control_block x);
void inorder(struct node *root);
