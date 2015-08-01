/*
EZW������

����������ɨ��ķ�ʽ��

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

����ƽ���Ƶ�ηֲ���
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


matrix_2d *M;					//���幤������
char error;						//�������
FILE *ezw_file;					//�ѱ����ļ�
ezw_file_header header;			//�ļ�ͷ
long int pixels;
long int zeroes, ones;
unsigned char input_byte, mask;


void show_code(int code)
{
  	switch (code)
	{
    	case ZERO:			//�������ΪZERO,�����Ϊ0
   			printf("0");
			break;

		case ONE:			//�������ΪONE,�����Ϊ1
			printf("1");
			break;

		case POS:			//�������ΪPOS,�����Ϊp
			printf("p");
			break;

		case NEG:			//�������ΪNEG,�����Ϊn
			printf("n");
			break;

		case ZTR:			//�������ΪZTR,�����Ϊt
      		printf("t");
    		break;

		case IZ:			//�������ΪIZ,�����Ϊz
      		printf("z");
    		break;
	}
}


//��һ���������ж���һ������λ
char get_bit(void)
{
  	char bit;		//��������ı���λ�洢�ռ�
  	if (mask==0) 
  	{
  		//���ļ��ж���һ���ֽڣ������input_byte��
		fread(&input_byte,sizeof(input_byte),1,ezw_file);
  		mask = 0x80;	//���λ��־
  	}
  	//��������ֽڵ����λΪ0,���������λ��ֵΪ0
	if ((input_byte&mask)==0) 
	{
  		bit = '0';
   		zeroes++;
  	}
	//�����������λ��ֵΪ1
  	else
	{
    	bit = '1';
    	ones++;
  	}
	//��߱�־λ����һλ
	mask >>= 1;
	//���ص�ǰ�жϵı���λ��ֵ
	return (bit);
}

 
//��һ���������ж���һ�������ֽ�
int input_code(int count)
{
	switch (get_bit())
	{
		//��������ı���λ��ֵΪ0
		case '0':
			//�����Ƕ����ֽڵ����λ���򷵻�ZERO
    		if (count==1) return (ZERO);
 			//���򣬶�ȡ����λ
			else 
			{
 				switch (get_bit()) 
				{
					case '0': return (ZTR);	//�������Ϊ0,����ΪZTR
					case '1': return (POS);	//�������Ϊ1,����ΪPOS
        		}
      		}
			break;
		//��������ı���λ��ֵΪ1
		case '1':
			//�����Ƕ����ֽڵ����λ���򷵻�ONE
      		if (count==1) return (ONE);
			//���򣬶�ȡ����λ
      		else
			{
				switch (get_bit())
				{
					case '0': return (IZ);	//�������Ϊ0,����ΪIZ
          			case '1': return (NEG);	//�������Ϊ1,����ΪNEG
        		}
      		}
    		break;
	}
	//ȱʡ����ֵ�����ɷ���
	return 0;
}

 
//��������Ԫ�غ���ֵ�������䵱ǰ����Ԫ��
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
 
//�������ļ����ݽ����ŵ���������m��
void EZW_decode(matrix_2d *m)
{
  	element_type threshold;			//��ֵ
  	pixels = 0;
  	threshold = header.threshold;
  	//�ӳ�ʼ��ֵ��ʼ֪����ֵΪ0
	while (threshold!=0) 
	{
    	dominant_pass(m,threshold);
    	subordinate_pass(m,threshold >> 1);
    	threshold >>= 1;		//��ֵ����
  	}
}
 
//������
int main(void)
{
	printf("\n");

	//�Զ�����ֻ��ģʽ�򿪱����ļ�
	if ((ezw_file=fopen("out.ezw","r"))==NULL) 
	{
		printf("Could not open input file.\n");
		exit(1);
  	};
  	//���ļ���ͷ��Ϣ
	fread(&header,sizeof(header),1,ezw_file);

  	//����һ���������󣨸����ߺͿ�
	M = matrix_2d_create(header.height,header.width);
  	if (M==NULL) exit(1);
  	//��������
	matrix_2d_clear(M);
	//��ʼֵ����
	zeroes = 0;
	ones = 0;
	input_byte = 0;
	mask = 0;
	//������������Ϊ����������
	EZW_decode(M);

#ifdef debug
 	printf("\n");
	//��������ֵ��д������
	matrix_2d_write(M);
 	printf("%ld bits: %ld zeroes, %ld ones\n", zeroes+ones, zeroes, ones);
#endif

 	//�ر���ǰ�ı����ļ�
	fclose(ezw_file);
	//�ͷŹ�������ռ�ݵĴ洢�ռ�
 	matrix_2d_destroy(M);
	//�ͷ�����ɨ������е���ʱ����
 	destroy_fifo();
	//�������ɨ������е���ʱ����
	destroy_list();
	//ȱʡ����ֵ
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
