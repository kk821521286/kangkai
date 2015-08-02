#include <stdio.h>
#include <stdlib.h>
void Exception(void)                           //注册终止函数，通过挂接到此函数，捕获异常信息 
{
	printf("试图去除以一个为0的数字，出现异常！\n");
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
	scanf("%lf",&b);
	if(0==b)                    //如果除数为0终止程序 ,并挂接到模拟异常捕获的注册函数
	{

		atexit(Exception);                          
		exit(EXIT_FAILURE);
	} 
	result=diva(a,b);
	printf("相除的结果是: %.2lf\n",result);    
	return 0;
}
