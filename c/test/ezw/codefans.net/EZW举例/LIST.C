//零树的确定与操作


#include "list.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct {
  list_type data;
  struct __list_element *next;
} list_element;

list_element *list_root = NULL;
list_element *list_current = NULL;
list_element *list_end = NULL;
int list_length=0;

void append_to_list(list_type d)
{
  list_element *p;
  p = malloc(sizeof(list_element));
  if (p!=NULL) {
    p->data = d;
    p->next = NULL;
    if (list_root==NULL) list_root = p;
    if (list_end!=NULL) list_end->next = p;
    list_end = p;
    list_length++;
  }
}

void destroy_list(void)
{
  list_element *p;

  p = list_root;
  while (p!=NULL) {
    list_root = p->next;
    free(p);
    p = list_root;
  }
  list_root = NULL;
  list_current = NULL;
  list_end = NULL;
  list_length = 0;
}

void display_list(void)
{
  list_element *p;
  p = list_root;
  while (p!=NULL) {
    printf("(%d,%d)",p->data.x,p->data.y);
    p = p->next;
  }
}

list_type get_list_element(int pos, char *found)
{
  list_element *p;
  list_type d;
  int i;

  i = 0;
  p = list_root;
  while ((i<pos) && (p!=NULL)) {
    i++;
    p = p->next;
  }
  if ((i<pos) || (p==NULL)) *found = 0;
  else {
    d = p->data;
    *found = 1;
  }
  return d;
}

void put_list_element(list_type d, int pos, char *found)
{
  list_element *p;
  int i;

  i = 0;
  p = list_root;
  while ((i<pos) && (p!=NULL)) {
    i++;
    p = p->next;
  }
  if ((i<pos) || (p==NULL)) *found = 0;
  else {
    p->data = d;
    *found = 1;
  }
}

void reset_list(void)
{
  list_current = list_root;
}

list_type get_next_list_element(char *found)
{
  list_type d;

  if (list_current==NULL) *found = 0;
  else {
    d = list_current->data;
    *found = 1;
    list_current = list_current->next;
  }
  return d;
}

void write_list_info(void)
{
  printf("\n");
  printf("element size: %ld, length: %d\n",sizeof(list_type),list_length);
}

void initialize_list(void)
{
  list_root = NULL;
  list_current = NULL;
  list_end = NULL;
  list_length = 0;
}
