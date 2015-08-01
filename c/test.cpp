#include <iostream>
using namespace std;
int main(void)
{
	int a = 9; 
	const int  &ra = a;
	a = 8;
	cout <<"a = " << a << endl;;
	return 0;
}
