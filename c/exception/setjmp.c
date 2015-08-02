#include <stdio.h>
#include <setjmp.h>
jmp_buf j;
void Exception(void)
{
	longjmp(j,1);//此时longjmp()会调用setjmp(),setjmp()函数的返回值为longjmp()函数的第二个参数。
}
double diva(double num1,double num2)         //两数相除函数
{
	double re;
	re=num1/num2;
	return re;
}
int main()
{
	double a,b,result;


	printf("请输入第一个数字：");
	scanf("%lf",&a);
	printf("请输入第二个数字：");
	if(setjmp(j)==0)
	{
		scanf("%lf",&b);
		if(0==b)
			Exception();
		result=diva(a,b);
		printf("相除的结果是: %.2lf\n",result);
	}
	else
		printf("试图除以一个为0的数字\n");
	return 0;
}
