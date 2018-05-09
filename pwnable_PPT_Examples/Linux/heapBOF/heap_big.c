#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	
	char *a = malloc(0x100);
	char *b = malloc(0x100);
	char *c = malloc(0x100);

	printf("address is a:%p , b:%p , c:%p\n", a, b, c);

	for(int i=0; i<0x100; i++)
		a[i] = '1';

	for(int i=0; i<0x100; i++)
		b[i] = '2';

	for(int i=0; i<0x100; i++)
		c[i] = '3';
		

	free(a);
	free(b);
	free(c);

}



