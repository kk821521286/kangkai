#include "uart.h"

void init_uart0(void)
{
	//��ʼ���ܽ�
	//GPA0.GPA0CON = 0x22 ;

	//step 1 : �������115200 8 n 1
	// �߿ؼĴ�����Ҫ�������� UARTͨ�ŵ����ݸ�ʽ  ����λ  ֹͣλ ��żУ��λ
					//NORMAL   odd        stop         data
	UART0.ULCON0 = (0 << 6) | (0 << 3) | ((0 << 2)) | (0X3) ;  //8 N 1

	// FIFO���ƼĴ���
	UART0.UFCON0 = 0 ; //����FIFO
	UART0.UMCON0 = 0;  //ʹ��Ӳ�������� RTS CTS�ź�������Ĵ���

	//115200������
	UART0.UBRDIV0 = 36 -1 ;	   //�����ʷ���
	UART0.UDIVSLOT0 = 0x0080 ; //ʱ�Ӳ���
	// UART���ƼĴ�����Ҫ���� ʱ��Դ  ���� ����ʹ�ܵ�
	UART0.UCON0 = (0 << 10) | (0x1 << 2) | (0x1 << 0) ;
}

/*
 * UART0����һ���ֽں���
 */
unsigned char recv_one_byte (void)
{
	unsigned char tmp ;
	while (0 == (UART0.UTRSTAT0 & 0x1)) ;
	tmp = UART0.URXH0 ;

	return tmp ;
}

/*
 * UART0����һ���ֽں���
 */
void send_one_byte (unsigned char sByte)
{
	while (0 == (UART0.UTRSTAT0 & 0x4)) ;
	UART0.UTXH0 = sByte ;
}

/*
 * UART0����һ���ַ�������'\0'����
 */
void send_string (unsigned char *s)
{
	while (*s != '\0') {
		send_one_byte (*s);
		s++ ;
	}
}
