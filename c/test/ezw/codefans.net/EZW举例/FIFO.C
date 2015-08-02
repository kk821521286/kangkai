/*
先入先出结构的实现和操作
*/


#include "fifo.h"
#include <stdlib.h>

typedef struct __fifo_element {
  ezw_element data;
  struct __fifo_element *previous;
} fifo_element;


fifo_element *fifo_first = NULL;
fifo_element *fifo_last = NULL;
char fifo_empty = 1;


void put_in_fifo(ezw_element d)
{
  fifo_element *p;

  p = malloc(sizeof(fifo_element));
  if (p!=NULL) {
    p->data = d;
    p->previous = NULL;
    if (fifo_last!=NULL) fifo_last->previous = p;
    fifo_last = p;
    if (fifo_first==NULL) fifo_first = p;
    fifo_empty = 0;
  }
}

ezw_element get_from_fifo(void)
{
  ezw_element d;
  fifo_element *p;

  p = fifo_first;
  if (p!=NULL) {
    d = p->data;
    fifo_first = p->previous;
    free(p);
  }
  else {
    fifo_last = NULL;
    fifo_empty = 1;
  }
  return d;
}

void destroy_fifo(void)
{
  fifo_element *p;
  while (fifo_first!=NULL) {
    p = fifo_first;
    fifo_first = p->previous;
    free(p);
  }
  fifo_last = NULL;
  fifo_empty = 1;
}

void initialize_fifo(void)
{
  fifo_first = NULL;
  fifo_last = NULL;
  fifo_empty = 1;
}
