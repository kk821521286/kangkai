#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_SIZE 32
int main()
{
	FILE *fp = NULL ,*fp1 = NULL;
	char buf[MAX_SIZE] ;
	int ret ;
	fp = fopen("./file.txt","r");
	if (fp == NULL) {
		perror("fopen error\n");
		return -1;
	}
	
	fp1 = fopen("./ff.txt","w");
	if(fp1 == NULL) {
	
		perror("fopen error\n");
		return -1;
	}

	while(!feof(fp)) {
		fgets(buf,sizeof(buf),fp);
		if(!feof(fp)) {
		ret = fputs(buf,fp1);
		if(ret == EOF) {
			perror("fputs error\n");
			return -1;
		}
		}
	}
	printf("write success\n");
	return 0;
}
