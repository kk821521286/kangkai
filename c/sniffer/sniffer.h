#ifndef __SNIFFER_H__
#define __SNIFFER_H__

#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

/*以太网帧头部*/
#define ETHER_ADDR_LEN 6

struct sniff_ethernet{
	u_char ether_dhost[ETHER_ADDR_LEN];/*目的主机地址*/
	u_char ether_shost[ETHER_ADDR_LEN];/*源主机地址*/
	u_short ether_type;
};

/*IP数据包的头部*/
struct sniff_ip{
	#if BYTE_ORDER == LITTLE_ENDIAN
	u_int ip_hl:4,	/*头部长度*/
	ip_v:4;			/*版本号*/
	#if BYTE_ORDER == BIG_ENDIAN
	u_int ip_v:4,	/*版本号*/
	ip_hl:4;		/*头部长度*/
	#endif
	#endif			/*not _IP_VHL*/

	u_char ip_tos;	/*服务的类型*/
	u_short ip_len;	/*总长度*/
	u_short ip_id;	/*包标志号*/
	u_short ip_off;	/*碎片偏移*/
	#define IP_RF 0x8000	/*保留的碎片标志*/
	#define IP_DF 0x4000	/*不保留碎片标志*/
	#define IP_MF 0x2000	/*多碎片标志*/
	#define IP_OFFMASK 0x1fff /*分段位*/
	
	u_char ip_ttl; /*数据包的生存时间 */
	u_char ip_p;	/*所使用的协议*/
	u_short ip_sum; /*校验和*/
	struct in_addr ip_src,ip_dst; /*源地址，目的地址*/

};

/*TCP 数据包的头部*/
typedef u_int tcp_seq;

struct sniff_tcp{
	u_short th_sport;  /*源端口*/
	u_short th_dport;  /*目的端口*/
	tcp_seq th_seq;		/*包序号*/
	tcp_seq th_ack;    /*确认序号*/
	#if BYTE_ORDER == LITTLE_ENDIAN
	u_int th_x2:4,		/*还没用到*/
	th_off:4;			/*数据偏移*/
	#endif
	#if BYTE_ORDER == BIG_ENDIAN
	u_int th_off:4,		/*数据偏移*/
	th_x2:4;			/*还没有用到*/
	#endif

	u_char th_flags;
	#define TH_FIN 0x01
	#define	TH_SYN 0x02
	#define TH_RST 0x04
	#define TH_PUSH 0x08
	#define TH_ACK 0x10
	#define TH_URG 0x20
	#define TH_ECE 0x40
	#define THCWR  0x80
	#define TH_FLAGS(TH_FINTH_SYNTH_RTTH_ACKTH_URGTH_ECETH_CWR)
	u_short th_win;  /*TCP滑动窗口*/
	u_short th_sum;  /*头部校验和*/
	u_short th_urp;  /*紧急服务位*/
};

#endif /*__SNIFFER_H__*/

