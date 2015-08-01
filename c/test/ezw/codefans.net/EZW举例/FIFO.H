//先入现出结构定义


#ifndef __FIFO_H__
#define __FIFO_H__


typedef struct __ezw_element {
  int x;
  int y;
  int code;
} ezw_element;

extern char fifo_empty;

void put_in_fifo(ezw_element d);
ezw_element get_from_fifo(void);
void destroy_fifo(void);
void initialize_fifo(void);


#endif /* __FIFO_H__ */

