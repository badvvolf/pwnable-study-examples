//Release 모드, 최적화 없앰, DEP 해제, ASLR 해제, SG 해제

#include<stdio.h>
#include<stdlib.h>

int main()
{
	char input[200];
	printf("address : %x\n", (unsigned int) input);

	scanf("%s", input);
}



