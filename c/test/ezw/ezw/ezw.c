/*
EZW������
����ɨ��ķ�ʽ���£�
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

����ƽ���Ƶ���ֲ����£�
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

//��������
matrix_2d *M;
//���ֵ
char error;
//����ͳ��ĳһ��������1��0�ĸ���
int zeroes, ones;
//����������������ļ�
FILE *ezw_file;
//����ֽڼ������־λ
unsigned char output_byte, mask;
//����һ��������������ļ�ͷ
ezw_file_header header;



void load_data(matrix_2d *m)
{
	int row, col;
	for (row=0; row<8; row++)
	{
		for (col=0; col<8; col++)
		{
			//���й�������ĸ�ֵ
			m->m[row][col] = example[row][col];
		}
	}
}


/*
 * Puts a bit in the output stream.
 */
void put_bit(char bit)
{
//�������ı���Ϊ��1,ͳ��1�ĸ�������֮ͳ��0�ĸ���
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
	//����һ���Ѿ������ұ��������ݣ����ж��������͡���������6�֣�������ͷ�ļ��ж��������
	switch (code)
	{
	case ZERO:
		put_bit('0');         //����Ƕ�������0�������һ������λ��ֵ����
#ifdef debug
		printf("0");
#endif
		break;
	case ONE:
		put_bit('1');        //����Ƕ�������1, �����һ������λ��ֵ����
#ifdef debug
		printf("1");
#endif
		break;
	case POS:
		put_bit('0');     //���������ϵ������˳�������������λ��ֵ����Ϊ0,1
		put_bit('1');
#ifdef debug
		printf("p");
#endif
		break;
	case NEG:
		put_bit('1');     //����Ǹ���ϵ������˳�������������λ��ֵ����Ϊ1,1
		put_bit('1');
#ifdef debug
		printf("n");
#endif
		break;
	case ZTR:
		put_bit('0');     //���������������˳�������������λ��ֵ����Ϊ0,0
		put_bit('0');
#ifdef debug
		printf("t");
#endif
		break;
	case IZ:
		put_bit('1');     //����ǹ¶��㣬��˳�������������λ��ֵ����Ϊ1,0
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
  //maxΪ�������ֵ
  element_type temp, max;
  //�����ǵ�һ���ڵ㣬���ؾ����Ԫ�����ֵ
  if ((x==0) && (y==0))
  {
    temp = m->m[0][0];
    m->m[0][0] = min_element_type;
    max = matrix_2d_abs_max(m);
    m->m[0][0] = temp;
  }
  //����ɨ�����
  else
  {
    min_x = x << 1;
    min_y = y << 1;
    max_x = (x+1) << 1;
    max_y = (y+1) << 1;
    //����ͼ��ɨ�������û�нṹ���˳�
    if ((min_x==m->col) || (min_y==m->row))
	{
      return (0);
    }
    //��ʼ�����ֵ
    max = 0;
    while ((max_y<=m->row) && (max_x<=m->col))
	{
      //ɨ�����
      for (i=min_y; i<max_y; i++)
	  {
        for (j=min_x; j<max_x; j++)
		{
          temp = abs(m->m[i][j]);
          if (temp>max) max = temp;
        }
      }
      //��ʼ������ֵ����1λ�󣬼���
      min_x <<= 1;
      max_x <<= 1;
      min_y <<= 1;
      max_y <<= 1;
    }

  }
//�������ֵ
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
    //����ǰϵ�����뻺����
    temp = m->m[0][0];
    m->m[0][0] = min_element_type;
    //ȡ������Ԫ�ص�������ֵ
    max = matrix_2d_abs_max(m);
    m->m[0][0] = temp;
    //������ֵ���ڵ�ǰ��ֵ��ֹͣ
    if (max>=threshold)
	stop = 1;
  }
  //������Ǿ���ĵ�һ��Ԫ�أ�����ֵ������1λ
  else
  {
    //��������Сֵ����1λ
    min_x = x << 1;
    //��������Сֵ����1λ
    min_y = y << 1;
    //���������ֵ����1λ
    max_x = (x+1) << 1;
    //���������ֵ����1λ
    max_y = (y+1) << 1;
    //���ɨ����ʼ������ֵ����ͼ��ı߽�����ֵ��������ֵ
    if ((min_x==m->col) || (min_y==m->row))
	{
		return (1);
    }
    max = 0;
    while ((max_y<=m->row) && (max_x<=m->col))
	{
      //���ϵ��ɨ��
      for (i=min_y; i<max_y; i++)
	  {
        for (j=min_x; j<max_x; j++)
		{
          temp = abs(m->m[i][j]);
          //������ִ�ϵ����ֹͣ
          if (temp>=threshold)
		  {
            stop = 1;
            break;
          }
        }
        if (stop==1) break;
      }
      if (stop==1) break;
      //����ֵ������1λ��
      //ѭ�����ڼ���
      min_x <<= 1;
      max_x <<= 1;
      min_y <<= 1;
      max_y <<= 1;
    }
  }
  if (stop==1) return (0);
  //������ֵ
  return (1);
}



/*
 * Returns a dominant-pass code from the alphabet [POS,NEG,ZTR,IZ].
 */
int code(matrix_2d *m, int x, int y, element_type threshold)
{
	element_type temp;
	//��������������ݴ浽һ������
	temp = m->m[y][x];
	//�����С��ϵ���ķ�ֵ���ڵ�ǰ��ֵ�����Ǵ�ϵ�������ǲ�֪��������
	if (abs(temp)>=threshold) 
	{
		//���������0,�򷵻�ΪPOS����"����ϵ��"��
		if (temp>=0) 
			return (POS);
		else
			//���򷵻�ΪNEG��"����ϵ��"��
			return (NEG);
	}
	//������ķ�ֵС�ڵ�ǰ��ֵ������Сϵ��������֪���Ƿ�"������"��
	else
	{
		//��������ж�Ϊ�棬�򷵻�ZTR��"������"��
		if (zerotree(m,x,y,threshold)==1)
			return (ZTR);
		//���򷵻�IZ��"�¶���"��
		else return (IZ);
	}
}


/*
 * Appends a value to the subordinate list.
 */
void to_sub_list(element_type value)
{
	list_type d;
	//�����������ϵ���ķ���ֵ�Ĵ�С��ϵ���ķ����Ѿ�����
	d.x = abs(value);
	d.y = 0;
	append_to_list(d);
}



/*
 * Builds a dominant pass EZW-element from a matrix element and a threshold.
 */
void process_element(matrix_2d *m, element_type threshold, ezw_element *s)
{
	//��С�����ݣ�ϵ�������������ֵ��Ϊ���룬���б��루�����Ż���
	s->code = code(m,s->x,s->y,threshold);
	//�������Ϊ��ϵ��������������������������ӱ���
	if ((s->code==POS) || (s->code==NEG))
	{
		to_sub_list(m->m[s->y][s->x]);
		//����λ�ø�λ
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
	//����ǰɨ��λ������example[0][0]�����������жϡ�����
	s.x = 0;
	s.y = 0;
	//����ϵ���뵱ǰ��ֵ���бȽϣ����Ż�
	process_element(m,threshold,&s);
	//����һ���Ľ�������������
	output_code(s.code);
	//��example[1][0],example[0][1],example[1][1]�����жϡ�����
	s.x = 1;
	s.y = 0;
	//����ϵ���뵱ǰ��ֵ���бȽϣ����Ż�
	process_element(m,threshold,&s);
	//����һ���Ľ���ŵ�ɨ��������
	put_in_fifo(s);
	s.x = 0;
	s.y = 1;
	//����ϵ���뵱ǰ��ֵ���бȽϣ����Ż�
	process_element(m,threshold,&s);
	//����һ���Ľ���ŵ�ɨ��������
	put_in_fifo(s);
	s.x = 1;
	s.y = 1;
	//����ϵ���뵱ǰ��ֵ���бȽϣ����Ż�
	process_element(m,threshold,&s);
	//����һ���Ľ���ŵ�ɨ��������
	put_in_fifo(s);
	//��ɨ��������ȡ��һ��ϵ��
	s = get_from_fifo();
	//��������Ϊ�գ�ֱ�ӽ������������
	if (fifo_empty==0) 
		output_code(s.code);
	while (fifo_empty==0) 
	{
		//�����ǰ���벻ΪZTR���򽫵�ǰɨ��������һƵ��
		if (s.code!=ZTR) 
		{
			//��������Сֵ����1λ�����ӱ�
			min_x = s.x << 1;
			//���������ֵΪ��Сֵ��1
			max_x = min_x+1;
			//��������Сֵ����1λ�����ӱ�
			min_y = s.y << 1;
			//���������ֵΪ��Сֵ��1
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
	list_type d;    //����һ������
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
	while (threshold!=0)            //��ʼ��������ֵΪ0
	{
		dominant_pass(m,threshold);
#ifdef debug
		printf("\n");
#endif
		//��������ֵ����1λ������ֵΪԭ����1/2
		//����ɨ���������ֵ��������ӱ��ɨ��
		subordinate_pass(threshold>>1);
#ifdef debug
		printf("\n");
#endif
		//���������ӱ�ɨ�����
		//��������ֵ����1λ������ֵΪԭ����1/2
		threshold >>= 1;
	}
}



int main(void)
{
	printf("\n");

//��������ĸ߶�Ϊ8
	header.height = 8;
	//��������Ŀ��Ϊ8
	header.width = 8;
	//����һ����������Ĺ�������
	M = matrix_2d_create(header.height,header.width);
	if (M==NULL)
		//�������������䲻�ɹ����˳�
		exit(1);
	//��ʵ��С��ϵ��ת�浽����������
	load_data(M);   
#ifdef debug
	matrix_2d_write(M);
#endif
	//׼��������������������ļ�
	header.threshold = 1 << (int)( floor (log10 (matrix_2d_abs_max(M)) / log10(2) ));
	if ((ezw_file=fopen("out.ezw","w"))==NULL)
	{
		printf("Could not open output file.\n");
		//�������ļ�û�д򿪣��˳�
		exit(1);
	};
	//������ļ���д���ļ�ͷ
	fwrite(&header,sizeof(header),1,ezw_file);
	//����EZW��������
	zeroes = 0;     //��ʼ��������û�б���λ��������0
	ones = 0;
	//��ʼ����ֽ���Ϊ0
	output_byte = 0;
	//���λΪ1�ı�־λ
	mask = 0x80;
	//������������Ϊ���룬������ֵ���б���
	EZW_code(M,header.threshold);
	//ˢ���������
	if (mask!=0)
	{
		fwrite(&output_byte,sizeof(output_byte),1,ezw_file);
	}
//#ifdef debug
	printf("\n");
	printf(" bits: %d, %d zeroes, %d ones.\n",zeroes+ones,zeroes,ones);
//#endif
	//������е���ʱ�ļ����ڴ�ռ��
	fclose(ezw_file);
	//�ͷŹ���������ռ�Ŀռ�
	matrix_2d_destroy(M);
	//�ͷ�����ɨ������е���ʱ����
	destroy_fifo();
	//�������ɨ������е���ʱ����
	destroy_list();
	//����
	return 0;

}
