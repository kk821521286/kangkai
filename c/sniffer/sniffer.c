#include "sniffer.h"
void getPacket(u_char *arg,const struct pcap_pkthdr *pkthdr,const u_char *packet)
{
	static int id = 0;
	const struct sniff_ethernet *ethernet; /*以太网帧头部*/
	const struct sniff_ip *ip; /*IP包头部*/
	const struct sniff_tcp *tcp; /*TCP包头部*/
	const char *payload; /*数据包的有效载荷*/
	
	int size_ethernet = sizeof(struct sniff_ethernet);
	int size_ip = sizeof(struct sniff_ip);
	int size_tcp = sizeof(struct sniff_tcp);

	ethernet = (struct sniff_ethernet*)(packet);
	ip = (struct sniff_ip*)(packet + size_ethernet);
	tcp = (struct sniff_tcp*)(packet +size_ethernet +size_ip);
	payload = (u_char*)(packet +size_ethernet + size_ip + size_tcp);

	int sport = ntohs(tcp->th_sport);
	int dport = ntohs(tcp->th_dport);

	//for QQ
	if(dport != 8000 || sport > 4000)
	{
		return ;
	}
	printf("packet:%d\n",++id);
	printf("%s:%d -> ",inet_ntoa(ip->ip_src),sport);
	printf("%s:%d \n",inet_ntoa(ip->ip_dst),dport);
	printf("QQ:%d\n",packet[49]*16*16*16*16*16*16 +
					 packet[50]*16*16*16*16 + 
					 packet[51]*16*16+
					 packet[52]);
	printf("\n");

}

int main(int argc,char **argv)
{
	pcap_t *devic = NULL;
	char *devStr = NULL;
	char errBuf[PCAP_ERRBUF_SIZE] = "";
	char *filter_rule = "dst port 8000";
	struct bpf_program filter;

	devStr = pcap_lookupdev(errBuf);
	if(!devStr)
	{
		printf("Error :%s\n",errBuf);
		return -1;
	}
	printf("Success:%s\n",devStr);

	devic = pcap_open_live(devStr,65535,1,0,errBuf);
	if(!devic)
	{
		printf("Error:%s\n",errBuf);
		return -1;
	}
	pcap_compile(devic,&filter,filter_rule,1,0);
	pcap_setfilter(devic,&filter);

	pcap_loop(devic,-1,getPacket,NULL);
	pcap_close(devic);
	return 0;
}





















