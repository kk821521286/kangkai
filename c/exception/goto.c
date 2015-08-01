#include <stdio.h>
#include <stdlib.h>
double diva(double num1,double num2)         //两数相除函数 
{
	double re;
	re=num1/num2;
	return re;
}
int main()
{
	double a,b,result;
	int flag = 0;
	if(flag == 1) {
Throw:
		printf("除数为0，出现异常\n");
		exit(EXIT_FAILURE);
	}
	//flag = 1;
	printf("请输入第一个数字：");
	scanf("%lf",&a);
	printf("请输入第二个数字：");
	scanf("%lf",&b);
	if(b==0)                                   //捕获异常（或许这么说并不恰当，暂且这么理解）
		goto Throw;                                //抛出异常 
	result=diva(a,b);
	printf("相除的结果是: %.2lf\n",result);    

	return 0;
}
