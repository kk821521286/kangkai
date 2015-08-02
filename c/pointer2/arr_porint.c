#include <stdio.h>

char *Name[] = {
	"kangkai",
	"zhangsan",
	"lisi",
	"wangwu",
	"zhaoliu",
	0
};

int main(void)
{
	char **fp = Name;
	while(*fp != 0) {
		printf("%s\n",*fp);
		*fp++;
	}
	return 0;
}
