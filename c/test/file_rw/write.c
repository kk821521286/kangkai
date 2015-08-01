#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE* fp = NULL;
	char a[20] = {0};

	fp = fopen("./file.txt","w+");

	while(1) {
		fgets(a,sizeof(a),stdin);
		if(strncmp(a,"quit",4) == 0) {
			break;
		}
		fwrite(a,1,strlen(a),fp);
		bzero(a,sizeof(a));
	}

	fclose(fp);
	return 0;
}
