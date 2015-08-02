#include "btree.h"

int main()
{
	BNode *root = NULL;
	root = createBtree(1,15);
	
	levelSort(root);
	printf("\n");
	preSort(root);
	printf("\n");
	midSort(root);
	printf("\n");
	aftSort(root);
	printf("\n");
	destroy(root);
	return 0;
}
