#include <stdio.h>
#include <stdlib.h>

double diva(double num1,double num2)
{
	double re;
	re = num1 / num2;
	return re;
}

int main()
{
	double a,b,result;
	printf("请输入第一个数字：");
	scanf("%lf",&a);
	printf("请输入第二个数字：");
	scanf("%lf",&b);

	if(0 == b)
	exit(EXIT_FAILURE);
	result = diva(a,b);
	printf("相除的结果是：%.2lf\n",result);
	return 0;
}
