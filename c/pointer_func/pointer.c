#include <stdio.h>

void (*funcp)();
void FileFunc(),EditFunc();

int main(void)
{
	funcp = FileFunc;
	(*funcp)();
	funcp = EditFunc;
	(*funcp)();
	return 0;
}

void FileFunc()
{
	printf("FileFunc\n");
}

void EditFunc()
{
	printf("EditFunc\n");
}

