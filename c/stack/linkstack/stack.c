#include "stack.h"


node_t * create_node(int value)
{
	node_t *node = NULL;
	node = malloc(sizeof(node_t));
	node->value = value;
	node->next = NULL;
	return node;
}
stack_t *stack_init(int length)
{
	stack_t *s = NULL;
	s = malloc(sizeof(stack_t));

	s->tlen = length;
	s->clen = 0;
	s->top = create_node(0);
	return s;
}

int push(stack_t *s,int value)
{
	node_t *temp = NULL;
	if(s->clen == s->tlen )
		return -1;
	temp = create_node(value);

	temp->next = s->top->next;
	s->top->next = temp;
	s->clen++;
	return 0;
}

int pop(stack_t *s,int *value)
{
	node_t *temp = NULL;
	if(s->clen == 0)
		return -1;
	temp = s->top->next;
	*value = temp->value;
	s->top->next = temp->next;
	free(temp);
	s->clen--;
	return 0;
}

int stack_destroy(stack_t*s)
{
	node_t *temp = NULL;
	temp = s->top;

	while(temp!=NULL) {
		s->top = s->top->next;
		free(temp);
		temp = s->top;
	}
	return 0;
}
