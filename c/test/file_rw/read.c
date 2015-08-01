#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE* fp = NULL;
	char a[20] = {0};

	fp = fopen("./file.txt","r");

	while(!feof(fp)) {
		fread(a,1,sizeof(a),fp);
		printf("%s",a);
		bzero(a,sizeof(a));
	}

	fclose(fp);
	return 0;
}
