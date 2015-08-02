#include <stdio.h>

void print(int *a,int size);
void bubbleSort(int *a,int size);
int main()
{
	int a[] = {5,1,2,33,23,11,4,9,10,7};
	print(a,sizeof(a)/sizeof(a[0]));
	bubbleSort(a,sizeof(a)/sizeof(a[0]));
	print(a,sizeof(a)/sizeof(a[0]));

	return 0;
}
void print(int *a,int size)
{
	int i ;
	for(i=0;i<size;i++) {
		printf("%d ",a[i]);
	}
	printf("\n");
}

void bubbleSort(int *a,int size)
{
	int i,j,temp;
	for(i=0;i<size-1;i++) {
		for(j=0;j<size-i ;j++) {
			if(a[j]>a[j+1]) {
				temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			}
		}
	}
}
