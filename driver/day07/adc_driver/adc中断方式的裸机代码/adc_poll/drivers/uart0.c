#include "uart.h"

void init_uart0(void)
{
	//初始化管脚
	//GPA0.GPA0CON = 0x22 ;

	//step 1 : 如何设置115200 8 n 1
	// 线控寄存器主要用于设置 UART通信的数据格式  数据位  停止位 奇偶校验位
					//NORMAL   odd        stop         data
	UART0.ULCON0 = (0 << 6) | (0 << 3) | ((0 << 2)) | (0X3) ;  //8 N 1

	// FIFO控制寄存器
	UART0.UFCON0 = 0 ; //不用FIFO
	UART0.UMCON0 = 0;  //使用硬件流控制 RTS CTS信号用这个寄存器

	//115200波特率
	UART0.UBRDIV0 = 36 -1 ;	   //波特率发生
	UART0.UDIVSLOT0 = 0x0080 ; //时钟补偿
	// UART控制寄存器主要设置 时钟源  发送 接收使能的
	UART0.UCON0 = (0 << 10) | (0x1 << 2) | (0x1 << 0) ;
}

/*
 * UART0接收一个字节函数
 */
unsigned char recv_one_byte (void)
{
	unsigned char tmp ;
	while (0 == (UART0.UTRSTAT0 & 0x1)) ;
	tmp = UART0.URXH0 ;

	return tmp ;
}

/*
 * UART0发送一个字节函数
 */
void send_one_byte (unsigned char sByte)
{
	while (0 == (UART0.UTRSTAT0 & 0x4)) ;
	UART0.UTXH0 = sByte ;
}

/*
 * UART0发送一个字符串，以'\0'结束
 */
void send_string (unsigned char *s)
{
	while (*s != '\0') {
		send_one_byte (*s);
		s++ ;
	}
}
