#include <stdio.h>

int main()
{
	char *s = "abcde";

	s += 2;
	printf("%d\n",*s);
	printf("%c\n",*s);
	printf("%s\n",s);
	return 0;
}
