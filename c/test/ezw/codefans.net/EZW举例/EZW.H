"EZW.H"//EZW编码器的头文件
#ifndef __EZW_H__
#define __EZW_H__

//这是一个数据格式的头文件，大家只需要知道它是用于程序中全部矩阵运算的完成即可
#include "matrix2d.h"  
//定义编码输出数据流存储的文件头格式
typedef struct __ezw_file_header
{
	int height;
	int width;
	element_type threshold;
} ezw_file_header;
//范例数据：取自Shapiro的实验数据
const int example[8][8] = {
  { 63,-34, 49, 10,  7, 13,-12,  7 },
  {-31, 23, 14,-13,  3,  4,  6, -1 },
  { 15, 14,  3,-12,  5, -7,  3,  9 },
  { -9, -7,-14,  8,  4, -2,  3,  2 },
  { -5,  9, -1, 47,  4,  6, -2,  2 },
  {  3,  0, -3,  2,  3, -2,  0,  4 },
  {  2, -3,  6, -4,  3,  6,  3,  6 },
  {  5, 11,  5,  6,  0,  3, -4,  4 }
};
//定义小波系数量化的字符：ZTR、POS、NEG和IZ
#define ZERO  0 		//二进制数0
#define ONE   1 		//二进制数1
#define ZTR   2 		//二进制数00
#define POS   3 		//二进制数01
#define NEG   4 		//二进制数11
#define IZ    5 		//二进制数10

#endif /* __EZW_H__ */
