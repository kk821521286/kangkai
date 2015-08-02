#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>


typedef struct __node {
	int value;
	struct __node *next;
}node_t;
typedef struct __stack {
	node_t *top;
	int clen;
	int tlen;
}stack_t;

extern node_t*create_node(int value);
extern stack_t *stack_init(int length);
extern int push(stack_t * s,int value);
extern int pop(stack_t * s,int *value);
extern int stack_destroy(stack_t *s);

#endif
