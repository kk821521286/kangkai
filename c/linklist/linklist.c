#include <stdio.h>
#include <stdlib.h>
typedef struct linknode 
{
	int data;
	struct linknode *next;
}linknode_t;
typedef struct linklist 
{
	 linknode_t *head;
	 linknode_t *tail;
	 int cur_length;
	 int total_length;
}linklist_t ;
linklist_t*	linklist_init(int num);

linknode_t* linknode_create(linknode_t *node);
int linklist_insert(linklist_t *L,int value);
int linklist_show(linklist_t *L);
int main()
{
	linklist_t *L;
	int i = 1;
	L = linklist_init(10);
	while(i < 10) {
		linklist_insert(L,i++);
	} 
//	linklist_insert();
	linklist_show(L);
	return 0;
}
 linklist_t *	linklist_init(int num)
{
	linklist_t *L = NULL;
	L = malloc(sizeof(linklist_t));
	L->head  = linknode_create(0);
	L->tail = L->head;

	L->cur_length = 0;
	L->total_length = num;
	return L;
}
linknode_t* linknode_create(linknode_t *node)
{
	node = malloc(sizeof(linknode_t));
	return node;
}
int linklist_insert(linklist_t *L,int value)
{
	linknode_t *node = NULL;
	if(L->cur_length == L->total_length)
		return -1;
	node = malloc(sizeof(linknode_t));
	node->data = value;
	node->next = NULL;
	L->tail->next = node;
	L->tail = L->tail->next;
	L->cur_length++;
		return 0;
}
int linklist_show(linklist_t *L)
{
	linknode_t *p = NULL;
	p = L->head->next;
	while(p != NULL) {
		printf("%d ",p->data);
		p = p->next;
	}
	printf("\n");
	return 0;
}
