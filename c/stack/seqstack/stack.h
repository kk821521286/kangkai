#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#define MAX 100
typedef struct stack_t {
	int a[MAX];
	int top;
}stack;

extern void init(stack *s);
extern int pop(stack *s,int *value);
extern int push(stack *s,int value);

#endif
