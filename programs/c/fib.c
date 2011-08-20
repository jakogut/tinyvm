#include <stdio.h>

int main()
{
	int a = 1, b = 0;

loop:
	a += b;
	b += a;

	printf("%i\n%i\n", a, b);

	if(a > 0 && b > 0) goto loop;

	return 0;
}
