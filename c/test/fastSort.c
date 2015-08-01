#include <stdio.h>
#include <stdlib.h>

void fastSort(int *a,int start,int end);
void quickSort(int a[],int left,int right);
void print(int *a,int size);
int main()
{
	int a[] = {3,5,2,13,9,8,4,1,10};
	print(a,sizeof(a)/sizeof(a[0]));
	fastSort(a,0,sizeof(a)/sizeof(a[0]) - 1);
	//quickSort(a,0,sizeof(a)/sizeof(a[0]) - 1);
	print(a,sizeof(a)/sizeof(a[0]));
	return 0;
}
void fastSort(int *a,int start,int end)
{
	int key;
	int low ,high;
	low = start;
	high = end;
	key = a[end];
	if(start >= end)
		return ;
	while(low < high) {
		while(low < high && a[low] <= key ) {
			low++;
		}
		a[high] = a[low];

		while(low < high && a[high] >= key) {
			high--;
		}
		a[low] = a[high];
	}
	a[low] = key;
	fastSort(a,start,low - 1);
	fastSort(a,low + 1,end);
}
void print(int *a,int size)
{
	int i ;
	for(i=0;i<size;i++) {
		printf("%d ",a[i]);
	}
	printf("\n");
}
void quickSort(int a[],int left,int right)
{
	int i=left;
	int j=right;
	int temp=a[left];
	if(left>=right)
		return;
	while(i!=j)
	{
		while(i<j&&a[j]>=temp) 
			j--;
		if(j>i)
			a[i]=a[j];//a[i]已经赋值给temp,所以直接将a[j]赋值给a[i],赋值完之后a[j],有空位
		while(i<j&&a[i]<=temp)
			i++;
		if(i<j)
			a[j]=a[i];
	}
	a[i]=temp;//把基准插入,此时i与j已经相等R[low..pivotpos-1].keys≤R[pivotpos].key≤R[pivotpos+1..high].keys
	quickSort(a,left,i-1);/*递归左边*/
	quickSort(a,i+1,right);/*递归右边*/
}
