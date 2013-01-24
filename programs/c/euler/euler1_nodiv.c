#include <stdio.h>

int main()
{
	unsigned upper_bound = 1000000, sum = 0, iteration = 0;

	for(int i = 3; i < upper_bound; i += 3) sum += i;
	for(int i = 5; i < upper_bound; i += 15) sum += i;
	for(int i = 10; i < upper_bound; i+= 15) sum += i;

	printf("%i\n", sum);
}
