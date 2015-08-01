#include <stdio.h>

void print(int *a,int size);
void selectSort (int *a,int size);
int main()
{
	int a[] = {5,1,2,33,23,11,4,9,10,7};
	print(a,sizeof(a)/sizeof(a[0]));
	selectSort(a,sizeof(a)/sizeof(a[0]));
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

void selectSort (int *a,int size)
{
	int i,j,temp;
	int min ;
	for(i=0;i<size-1;i++) {
		min = i;
		for(j=i;j<size ;j++) {
			if(a[min] > a[j]) {
				temp = min;
				min = j;
				j = temp;
			}
		}
		if(min != i) {
			temp = a[i];
			a[i] = a[min];
			a[min] = temp;
		}
	}
}
