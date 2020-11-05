#ifndef rbtree_h
#define rbtree_h



#include <stdio.h>
#include <stdlib.h>
#include "define_hariak.h"


enum nodeColor {
  RED,
  BLACK
};

struct rbNode {
  int color;
  struct process_control_block data;
  struct rbNode *link[2];
};

extern struct rbNode *root;

struct rbNode *createNode(struct process_control_block data); 

void insertion(struct process_control_block data);
void inorderTraversal(struct rbNode *node);
void deletion(struct process_control_block data);

#endif
