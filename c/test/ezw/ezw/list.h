//零树结构的定义

#ifndef __LIST_H__
#define __LIST_H__


/*
 * list_type = integer;
 */
typedef struct {
  int x;
  int y;
} list_type;

extern int list_length;

void append_to_list(list_type d);
void destroy_list(void);
void display_list(void);
list_type get_list_element(int pos, char *found);
void put_list_element(list_type d, int pos, char *found);
void reset_list(void);
list_type get_next_list_element(char *found);
void write_list_info(void);
void initialize_list(void);


#endif //__LIST_H__

