#ifndef BTREE_H
#define BTREE_H
#include <stdio.h>
#include <stdlib.h>
//#include "queue.h"

typedef struct bnode_t {
	int value;
	struct bnode_t *lchild;
	struct bnode_t *rchild;
}BNode;

extern BNode * createBtree(int start,int end);
extern int preSort(BNode* root);
extern int midSort(BNode* root);
extern int aftSort(BNode* root);
extern int destroy(BNode* root);
extern BNode * createBNode(int value);
extern int levelSort(BNode* root);


#endif  
