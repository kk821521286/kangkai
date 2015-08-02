/*
EZW编解码器的数据操作
*/


#include "matrix2d.h"
#include <stdlib.h>
#include <stdio.h>


/*
 * Allocate memory for a two-dimensional RxC matrix.
 * Returns NULL on failure.
 */
matrix_2d *matrix_2d_create(int r, int c)
{
  int row;
  matrix_2d *m;

  m = malloc(sizeof(matrix_2d));

  if (m!=NULL) {
    m->row = r;
    m->col = c;
    m->m = malloc(r*sizeof(element_type*));
    if (m->m==NULL) return NULL;
    for (row=0; row<r; row++) {
      m->m[row] = malloc(c*sizeof(element_type));
      if (m->m[row]==NULL) return NULL;
    }
  }

  return m;
}


/*
 * Destroy a matrix.
 */
void matrix_2d_destroy(matrix_2d *m)
{
  int row;
  for (row=0; row<m->row; row++) {
    free(m->m[row]);
  }
  free(m);
}


/*
 * Set all matrix elements to zero.
 */
void matrix_2d_clear(matrix_2d *m)
{
  int row, col;

  for (row=0; row<m->row; row++) {
    for (col=0; col<m->col; col++) {
      m->m[row][col] = 0;
    }
  }
}

/*
 * Display a matrix.
 */
void matrix_2d_write(matrix_2d *m)
{
  int row, col;
  for (row=0; row<m->row; row++) {
    for (col=0; col<m->col; col++) {
      printf("%3d",m->m[row][col]);
    }
    printf("\n");
  }
}


/*
 * Returns max(abs(matrix)), the absolute maximum value of a matrix.
 */
element_type matrix_2d_abs_max(matrix_2d *m)
{
  int row, col;
  element_type max = 0;

  for (row=0; row<m->row; row++) {
    for (col=0; col<m->col; col++) {
      if (abs(m->m[row][col])>max) max = abs(m->m[row][col]);
    }
  }

  return max;
}

