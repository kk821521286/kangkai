/*
EZW解码器

解码器数据扫描的方式：

Morton order scan:
====================================

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
========================

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

数据平面的频段分布：
Subband distribution:
==========================================

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

#include <stdlib.h>
#include <stdio.h>


matrix_2d *M;					//定义工作矩阵
char error;						//定义误差
FILE *ezw_file;					//已编码文件
ezw_file_header header;			//文件头
long int pixels;
long int zeroes, ones;
unsigned char input_byte, mask;


void show_code(int code)
{
  	switch (code)
	{
    	case ZERO:			//如果编码为ZERO,则输出为0
   			printf("0");
			break;

		case ONE:			//如果编码为ONE,则输出为1
			printf("1");
			break;

		case POS:			//如果编码为POS,则输出为p
			printf("p");
			break;

		case NEG:			//如果编码为NEG,则输出为n
			printf("n");
			break;

		case ZTR:			//如果编码为ZTR,则输出为t
      		printf("t");
    		break;

		case IZ:			//如果编码为IZ,则输出为z
      		printf("z");
    		break;
	}
}


//从一个输入流中读出一个比特位
char get_bit(void)
{
  	char bit;		//定义读出的比特位存储空间
  	if (mask==0) 
  	{
  		//从文件中读出一个字节，存放在input_byte中
		fread(&input_byte,sizeof(input_byte),1,ezw_file);
  		mask = 0x80;	//最高位标志
  	}
  	//如果读出字节的最高位为0,则读出比特位的值为0
	if ((input_byte&mask)==0) 
	{
  		bit = '0';
   		zeroes++;
  	}
	//否则读出比特位的值为1
  	else
	{
    	bit = '1';
    	ones++;
  	}
	//最高标志位右移一位
	mask >>= 1;
	//返回当前判断的比特位的值
	return (bit);
}

 
//从一个输入流中读出一个编码字节
int input_code(int count)
{
	switch (get_bit())
	{
		//如果读出的比特位的值为0
		case '0':
			//假如是读出字节的最高位，则返回ZERO
    		if (count==1) return (ZERO);
 			//否则，读取比特位
			else 
			{
 				switch (get_bit()) 
				{
					case '0': return (ZTR);	//如果读出为0,返回为ZTR
					case '1': return (POS);	//如果读出为1,返回为POS
        		}
      		}
			break;
		//如果读出的比特位的值为1
		case '1':
			//假如是读出字节的最高位，则返回ONE
      		if (count==1) return (ONE);
			//否则，读取比特位
      		else
			{
				switch (get_bit())
				{
					case '0': return (IZ);	//如果读出为0,返回为IZ
          			case '1': return (NEG);	//如果读出为1,返回为NEG
        		}
      		}
    		break;
	}
	//缺省返回值，不可返回
	return 0;
}

 
//利用主表元素和阈值解码后填充当前矩阵元素
void input_element(matrix_2d *m, element_type t, ezw_element *s)
{
	list_type d;
	d.x = s->x;
	d.y = s->y;
	s->code = input_code(2);

#ifdef debug
	show_code(s->code);
#endif

	if ((s->code==POS)) 
	{
		m->m[s->y][s->x] = t;
		append_to_list(d);
  	}
  	else if ((s->code==NEG))
	{
    	m->m[s->y][s->x] = -t;
    	append_to_list(d);
  	}
}


/*
 * Performs one dominant pass.
 */

void dominant_pass(matrix_2d *m, element_type threshold)
{
  	ezw_element s;
  	int min_x, max_x, min_y, max_y;

	s.x = 0;
	s.y = 0;
	input_element(m,threshold,&s);
	if ((s.code==POS) || (s.code==NEG)) pixels++;

	s.x = 1;
	s.y = 0;
	input_element(m,threshold,&s);
	put_in_fifo(s);
	s.x = 0;
	s.y = 1;
	input_element(m,threshold,&s);
	put_in_fifo(s);
	s.x = 1;
	s.y = 1;
	input_element(m,threshold,&s);
	put_in_fifo(s);

	s = get_from_fifo();
	if (fifo_empty==0)
	{
		if ((s.code==POS) || (s.code==NEG)) pixels++;
  	}

  	while (fifo_empty==0)
	{
  		if (s.code!=ZTR)
		{
     		min_x = s.x << 1;
      		max_x = min_x+1;
      		min_y = s.y << 1;
      		max_y = min_y+1;
      		if ((max_x<=m->col) && (max_y<=m->row))
			{
        		for (s.y=min_y; s.y<=max_y; s.y++)
				{
					for (s.x=min_x; s.x<=max_x; s.x++)
					{
            			input_element(m,threshold,&s);
            			put_in_fifo(s);
          			}
        		}
      		}
    	}
    	s = get_from_fifo();
    	if (fifo_empty==0)
		{
			if ((s.code==POS) || (s.code==NEG)) pixels++;
    	}
	}
}


/*
 * Performs one subordinate pass.
 */
void subordinate_pass(matrix_2d *m, element_type threshold)
{
  	long int i;
  	element_type temp;
  	list_type d;
  	char found;

  	if (threshold>0) 
	{
    	for (i=0; i<pixels; i++)
		{
      		d = get_list_element(i,&found);
      		if (found==1)
			{
        		temp = m->m[d.y][d.x];
        		if (input_code(1)==ONE)
				{
					#ifdef debug
						show_code(ONE);
					#endif
          			if (temp<0)
					{
						m->m[d.y][d.x] = temp - threshold;
          			}
          			else
					{
						m->m[d.y][d.x] = temp + threshold;
          			}
        		}
#ifdef debug
				else show_code(ZERO);
#endif
			}
		}
	}
}
 
//将编码文件数据解码存放到工作矩阵m中
void EZW_decode(matrix_2d *m)
{
  	element_type threshold;			//阈值
  	pixels = 0;
  	threshold = header.threshold;
  	//从初始阈值开始知道阈值为0
	while (threshold!=0) 
	{
    	dominant_pass(m,threshold);
    	subordinate_pass(m,threshold >> 1);
    	threshold >>= 1;		//阈值减半
  	}
}
 
//主函数
int main(void)
{
	printf("\n");

	//以二进制只读模式打开编码文件
	if ((ezw_file=fopen("out.ezw","r"))==NULL) 
	{
		printf("Could not open input file.\n");
		exit(1);
  	};
  	//读文件的头信息
	fread(&header,sizeof(header),1,ezw_file);

  	//创建一个工作矩阵（给出高和宽）
	M = matrix_2d_create(header.height,header.width);
  	if (M==NULL) exit(1);
  	//矩阵清零
	matrix_2d_clear(M);
	//初始值设置
	zeroes = 0;
	ones = 0;
	input_byte = 0;
	mask = 0;
	//将工作矩阵作为参数，解码
	EZW_decode(M);

#ifdef debug
 	printf("\n");
	//工作矩阵赋值（写操作）
	matrix_2d_write(M);
 	printf("%ld bits: %ld zeroes, %ld ones\n", zeroes+ones, zeroes, ones);
#endif

 	//关闭先前的编码文件
	fclose(ezw_file);
	//释放工作矩阵占据的存储空间
 	matrix_2d_destroy(M);
	//释放主表扫描过程中的临时数据
 	destroy_fifo();
	//清除零树扫描过程中的临时数据
	destroy_list();
	//缺省返回值
 	return 0;
}
int iEZW(void)
{
	printf("\n");

  if ((ezw_file=fopen("out.ezw","rb"))==NULL) {
    printf("Could not open input file.\n");
    exit(1);
  };
  fread(&header,sizeof(header),1,ezw_file);

  M = matrix_2d_create(header.height,header.width);
  if (M==NULL) exit(1);
  matrix_2d_clear(M);

  zeroes = 0;
  ones = 0;
  input_byte = 0;
  mask = 0;
  EZW_decode(M);

#ifdef debug
  printf("\n");
  matrix_2d_write(M);
  printf("%ld bits: %ld zeroes, %ld ones\n", zeroes+ones, zeroes, ones);
#endif

  fclose(ezw_file);
  matrix_2d_destroy(M);
  destroy_fifo();
  destroy_list();

  return 0;
}
