/*
EZW编码器
数据扫描的方式如下：
Morton order scan:
==================

   1 | 2 |  5   6 | 17  18  21  22
  ---+---|        |
   3 | 4 |  7   8 | 19  20  23  24
  -------+--------|
   9  10 | 13  14 | 25  26  29  30
         |        |
  11  12 | 15  16 | 27  28  31  32
  ----------------+---------------
  33  34   37  38 | 49  50  53  54
                  |
  35  36   39  40 | 51  52  55  56
                  |
  41  42   45  46 | 57  58  61  62
                  |
  43  44   47  48 | 59  60  63  64


Raster scan:
============

   1 | 2 |  5   6 | 17  18  19  20
  ---+---|        |
   3 | 4 |  7   8 | 21  22  23  24
  -------+--------|
   9  10 | 13  14 | 25  26  27  28
         |        |
  11  12 | 15  16 | 29  30  31  32
  ----------------+---------------
  33  34   35  36 | 49  50  51  52
                  |
  37  38   39  40 | 53  54  55  56
                  |
  41  42   43  44 | 57  58  59  60
                  |
  45  46   47  48 | 61  62  63  64

数据平面的频带分布如下：
Subband distribution:
=====================

  LL | HL | HL   HL | HL   HL   HL   HL
  ---+--- |         |
  LH | HH | HL   HL | HL   HL   HL   HL
  --------+---------|
  LH   LH | HH   HH | HL   HL   HL   HL
          |         |
  LH   LH | HH   HH | HL   HL   HL   HL
  ------------------+------------------
  LH   LH   LH   LH | HH   HH   HH   HH
                    |
  LH   LH   LH   LH | HH   HH   HH   HH
                    |
  LH   LH   LH   LH | HH   HH   HH   HH
                    |
  LH   LH   LH   LH | HH   HH   HH   HH



*/


#define debug 

#include "ezw.h"
#include "fifo.h"
#include "list.h"
#include "matrix2d.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//工作矩阵
matrix_2d *M;
//误差值
char error;
//用于统计某一数据流中1和0的个数
int zeroes, ones;
//编码数据流的输出文件
FILE *ezw_file;
//输出字节及编码标志位
unsigned char output_byte, mask;
//建立一个数据流输出的文件头
ezw_file_header header;



void load_data(matrix_2d *m)
{
	int row, col;
	for (row=0; row<8; row++)
	{
		for (col=0; col<8; col++)
		{
			//进行工作矩阵的赋值
			m->m[row][col] = example[row][col];
		}
	}
}


/*
 * Puts a bit in the output stream.
 */
void put_bit(char bit)
{
//如果放入的比特为是1,统计1的个数；反之统计0的个数
	if (bit=='1') 
	{
		output_byte |= mask;
		ones++;
  	}
	else zeroes++;
 	mask >>= 1;
 	if (mask==0) 
	{
		fwrite(&output_byte,sizeof(output_byte),1,ezw_file);
		output_byte = 0;
		mask = 0x80;
	}
}


/*
 * Puts dominant-pass and subordinate-pass codes in the output stream.
 */
void output_code(int code)
{
	//对于一个已经量化且编码后的数据，来判断它的类型。其类型有6种，正如在头文件中定义的那样
	switch (code)
	{
	case ZERO:
		put_bit('0');         //如果是二进制数0，则输出一个比特位，值不变
#ifdef debug
		printf("0");
#endif
		break;
	case ONE:
		put_bit('1');        //如果是二进制数1, 则输出一个比特位，值不变
#ifdef debug
		printf("1");
#endif
		break;
	case POS:
		put_bit('0');     //如果是正大系数，则按顺序输出两个比特位，值依次为0,1
		put_bit('1');
#ifdef debug
		printf("p");
#endif
		break;
	case NEG:
		put_bit('1');     //如果是负大系数，则按顺序输出两个比特位，值依次为1,1
		put_bit('1');
#ifdef debug
		printf("n");
#endif
		break;
	case ZTR:
		put_bit('0');     //如果是零树根，则按顺序输出两个比特位，值依次为0,0
		put_bit('0');
#ifdef debug
		printf("t");
#endif
		break;
	case IZ:
		put_bit('1');     //如果是孤独零，则按顺序输出两个比特位，值依次为1,0
		put_bit('0');
#ifdef debug
		printf("z");
#endif
		break;
	}
}


/*
 * Returns the largest value in a descendance tree.
 */
element_type max_descendant(matrix_2d *m, int x, int y)
{
  int i, j, min_x, max_x, min_y, max_y;
  //max为函数输出值
  element_type temp, max;
  //假如是第一个节点，返回矩阵得元素最大值
  if ((x==0) && (y==0))
  {
    temp = m->m[0][0];
    m->m[0][0] = min_element_type;
    max = matrix_2d_abs_max(m);
    m->m[0][0] = temp;
  }
  //否则，扫描继续
  else
  {
    min_x = x << 1;
    min_y = y << 1;
    max_x = (x+1) << 1;
    max_y = (y+1) << 1;
    //整幅图像扫描完毕仍没有结构，退出
    if ((min_x==m->col) || (min_y==m->row))
	{
      return (0);
    }
    //初始化最大值
    max = 0;
    while ((max_y<=m->row) && (max_x<=m->col))
	{
      //扫描进行
      for (i=min_y; i<max_y; i++)
	  {
        for (j=min_x; j<max_x; j++)
		{
          temp = abs(m->m[i][j]);
          if (temp>max) max = temp;
        }
      }
      //起始点坐标值左移1位后，继续
      min_x <<= 1;
      max_x <<= 1;
      min_y <<= 1;
      max_y <<= 1;
    }

  }
//返回最大值
  return (max);
}


/*
 * Returns 1 if descendance tree is a zerotree.
 */

char zerotree(matrix_2d *m, int x, int y, int threshold)
{
  int i, j, min_x, max_x, min_y, max_y;
  element_type temp, max;
  char stop;
  stop = 0;
  if ((x==0) && (y==0))
  {
    //将当前系数放入缓存区
    temp = m->m[0][0];
    m->m[0][0] = min_element_type;
    //取矩阵中元素的最大绝对值
    max = matrix_2d_abs_max(m);
    m->m[0][0] = temp;
    //如果最大值大于当前阈值，停止
    if (max>=threshold)
	stop = 1;
  }
  //如果不是矩阵的第一个元素，坐标值将左移1位
  else
  {
    //横坐标最小值左移1位
    min_x = x << 1;
    //纵坐标最小值左移1位
    min_y = y << 1;
    //横坐标最大值左移1位
    max_x = (x+1) << 1;
    //纵坐标最大值左移1位
    max_y = (y+1) << 1;
    //如果扫描起始点坐标值超过图像的边界坐标值，返回真值
    if ((min_x==m->col) || (min_y==m->row))
	{
		return (1);
    }
    max = 0;
    while ((max_y<=m->row) && (max_x<=m->col))
	{
      //逐个系数扫描
      for (i=min_y; i<max_y; i++)
	  {
        for (j=min_x; j<max_x; j++)
		{
          temp = abs(m->m[i][j]);
          //如果出现大系数，停止
          if (temp>=threshold)
		  {
            stop = 1;
            break;
          }
        }
        if (stop==1) break;
      }
      if (stop==1) break;
      //坐标值再左移1位，
      //循环仍在继续
      min_x <<= 1;
      max_x <<= 1;
      min_y <<= 1;
      max_y <<= 1;
    }
  }
  if (stop==1) return (0);
  //返回真值
  return (1);
}



/*
 * Returns a dominant-pass code from the alphabet [POS,NEG,ZTR,IZ].
 */
int code(matrix_2d *m, int x, int y, element_type threshold)
{
	element_type temp;
	//将欲处理的数据暂存到一块区域
	temp = m->m[y][x];
	//如果该小波系数的幅值大于当前阈值，则是大系数（但是不知道正负）
	if (abs(temp)>=threshold) 
	{
		//如果它大于0,则返回为POS（即"正大系数"）
		if (temp>=0) 
			return (POS);
		else
			//否则返回为NEG（"负大系数"）
			return (NEG);
	}
	//如果它的幅值小于当前阈值，则是小系数（但不知道是否"零树根"）
	else
	{
		//如果零树判断为真，则返回ZTR（"零树根"）
		if (zerotree(m,x,y,threshold)==1)
			return (ZTR);
		//否则返回IZ（"孤独零"）
		else return (IZ);
	}
}


/*
 * Appends a value to the subordinate list.
 */
void to_sub_list(element_type value)
{
	list_type d;
	//仅仅往树添加系数的幅度值的大小，系数的符号已经编码
	d.x = abs(value);
	d.y = 0;
	append_to_list(d);
}



/*
 * Builds a dominant pass EZW-element from a matrix element and a threshold.
 */
void process_element(matrix_2d *m, element_type threshold, ezw_element *s)
{
	//将小波数据，系数坐标和量化阈值作为输入，进行编码（即符号化）
	s->code = code(m,s->x,s->y,threshold);
	//假如编码为大系数（无论正负），将编码放入子表中
	if ((s->code==POS) || (s->code==NEG))
	{
		to_sub_list(m->m[s->y][s->x]);
		//编码位置复位
		m->m[s->y][s->x] = 0;
	}
}


/*
 * Performs one complete dominant pass. Dominant-pass-codes are sent to the
 * output stream and the subordinate list is updated.
 */
void dominant_pass(matrix_2d *m, element_type threshold)
{
	ezw_element s;
	int min_x, max_x, min_y, max_y;
	//将当前扫描位置移至example[0][0]处，并进行判断、量化
	s.x = 0;
	s.y = 0;
	//将该系数与当前阈值进行比较，符号化
	process_element(m,threshold,&s);
	//将上一步的结果进行输出编码
	output_code(s.code);
	//对example[1][0],example[0][1],example[1][1]进行判断、量化
	s.x = 1;
	s.y = 0;
	//将该系数与当前阈值进行比较，符号化
	process_element(m,threshold,&s);
	//将上一步的结果放到扫描序列中
	put_in_fifo(s);
	s.x = 0;
	s.y = 1;
	//将该系数与当前阈值进行比较，符号化
	process_element(m,threshold,&s);
	//将上一步的结果放到扫描序列中
	put_in_fifo(s);
	s.x = 1;
	s.y = 1;
	//将该系数与当前阈值进行比较，符号化
	process_element(m,threshold,&s);
	//将上一步的结果放到扫描序列中
	put_in_fifo(s);
	//从扫描序列中取出一个系数
	s = get_from_fifo();
	//假如序列为空，直接将其送输出编码
	if (fifo_empty==0) 
		output_code(s.code);
	while (fifo_empty==0) 
	{
		//如果当前编码不为ZTR，则将当前扫描移至下一频段
		if (s.code!=ZTR) 
		{
			//横坐标最小值左移1位，即加倍
			min_x = s.x << 1;
			//横坐标最大值为最小值加1
			max_x = min_x+1;
			//纵坐标最小值左移1位，即加倍
			min_y = s.y << 1;
			//纵坐标最大值为最小值加1
			max_y = min_y+1;
			if ((max_x<=m->col) && (max_y<=m->row)) 
			{
				for (s.y=min_y; s.y<max_y+1; s.y++) 
				{
					for (s.x=min_x; s.x<max_x+1; s.x++) 
					{
						process_element(m,threshold,&s);
						put_in_fifo(s);
					}
				}
			}
		}
		s = get_from_fifo();
		if (fifo_empty==0) output_code(s.code);
	}
}



/*
 * Performs one subordinate pass.
 */
void subordinate_pass(element_type threshold)
{
	list_type d;    //定义一颗零树
	int i;
	char found;
	if (threshold>0) 
	{
		for (i=0; i<list_length; i++) 
		{
			d = get_list_element(i,&found);
			if (found==1) 
			{
				if ((d.x&threshold)!=0)
					output_code(ONE);
				else 
					output_code(ZERO);
			}
		}
	}
}


/*
 * EZW-codes matrix m, returns initial threshold.
 */
void EZW_code(matrix_2d *m, element_type threshold)
{
	while (threshold!=0)            //初始化量化阈值为0
	{
		dominant_pass(m,threshold);
#ifdef debug
		printf("\n");
#endif
		//将量化阈值右移1位，即数值为原来的1/2
		//主表扫描结束后，阈值减半继续子表的扫描
		subordinate_pass(threshold>>1);
#ifdef debug
		printf("\n");
#endif
		//本次主表、子表扫描结束
		//将量化阈值右移1位，即数值为原来的1/2
		threshold >>= 1;
	}
}



int main(void)
{
	printf("\n");

//工作矩阵的高度为8
	header.height = 8;
	//工作矩阵的宽度为8
	header.width = 8;
	//创建一个用于运算的工作矩阵
	M = matrix_2d_create(header.height,header.width);
	if (M==NULL)
		//如果工作矩阵分配不成功，退出
		exit(1);
	//将实验小波系数转存到工作矩阵中
	load_data(M);   
#ifdef debug
	matrix_2d_write(M);
#endif
	//准备编码后的数据流输出的文件
	header.threshold = 1 << (int)( floor (log10 (matrix_2d_abs_max(M)) / log10(2) ));
	if ((ezw_file=fopen("out.ezw","w"))==NULL)
	{
		printf("Could not open output file.\n");
		//如果输出文件没有打开，退出
		exit(1);
	};
	//往输出文件中写入文件头
	fwrite(&header,sizeof(header),1,ezw_file);
	//进行EZW编码运算
	zeroes = 0;     //初始数据流中没有比特位，所以置0
	ones = 0;
	//初始输出字节数为0
	output_byte = 0;
	//最高位为1的标志位
	mask = 0x80;
	//将工作矩阵作为输入，给定阈值进行编码
	EZW_code(M,header.threshold);
	//刷新输出区域
	if (mask!=0)
	{
		fwrite(&output_byte,sizeof(output_byte),1,ezw_file);
	}
//#ifdef debug
	printf("\n");
	printf(" bits: %d, %d zeroes, %d ones.\n",zeroes+ones,zeroes,ones);
//#endif
	//清除所有的临时文件和内存占用
	fclose(ezw_file);
	//释放工作矩阵所占的空间
	matrix_2d_destroy(M);
	//释放主表扫描过程中的临时数据
	destroy_fifo();
	//清除零树扫描过程中的临时数据
	destroy_list();
	//返回
	return 0;

}
