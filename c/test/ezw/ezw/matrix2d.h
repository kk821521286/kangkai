//EZW编解码数据结构的定义：矩阵结构


#ifndef __MATRIX_2D_H__
#define __MATRIX_2D_H__

typedef int element_type;
#define min_element_type -32768
#define max_element_type  32767

typedef struct __matrix_2d {
  int row;
  int col;
  element_type **m;
} matrix_2d;


matrix_2d *matrix_2d_create(int r, int c);
void matrix_2d_destroy(matrix_2d *m);
void matrix_2d_clear(matrix_2d *m);
void matrix_2d_write(matrix_2d *m);
element_type matrix_2d_abs_max(matrix_2d *m);


#endif  //__MATRIX_2D_H__ 

