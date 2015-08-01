#include "btree.h"
#include "queue.h"

BNode * createBNode(int value)
{
	BNode *node = NULL;
	node = malloc(sizeof(BNode));
	node->value = value;
	node->lchild = NULL;
	node->rchild = NULL;
	return node;
}
 BNode* createBtree(int start,int end)
{
	 BNode  *root = NULL;
	 root = createBNode(start);
	 if(start*2 <= end) 
		root->lchild = createBtree(start*2,end);
	 if(start*2 + 1 <= end) 
		root->rchild = createBtree(start*2 + 1,end);
	 return root;
}


int preSort(BNode *root)
{
	if(!root)
		return -1;
	printf("%d ",root->value);
	preSort(root->lchild);
	preSort(root->rchild);
	return 0;
}

int midSort(BNode *root)
{
	if(!root)
		return -1;
	midSort(root->lchild);
	printf("%d ",root->value);
	midSort(root->rchild);
	return 0;
}
int aftSort(BNode *root)
{
	if(!root)
		return -1;
	aftSort(root->lchild);
	aftSort(root->rchild);
	printf("%d ",root->value);
	return 0;
}
int destroy(BNode *root)
{
	if(!root) 
		return -1;
	if(root) {
		destroy(root->lchild);
		destroy(root->rchild);
		free(root);
	}
	return 0;
}

int levelSort(BNode *root)
{
	if(!root) 
		return -1;
	
	queue_t *queue = NULL;
	queue = queue_init(100);
	BNode *temp = NULL;
	temp = root;
	
	enqueue(queue,temp);
	
	while(queue->clen != 0) {
		dequeue(queue,&temp);
		printf("%d ",temp->value);
		if(NULL != temp->lchild)
			enqueue(queue,temp->lchild);
		if(NULL != temp->rchild)
			enqueue(queue,temp->rchild);

	}
	return 0;
}

