//Release 모드, 최적화 없앰, DEP 해제, ASLR 해제, SG 해제

#include<stdio.h>
#include<stdlib.h>

int num = 1;
int main()
{
	char input[200];
	scanf("%s", input);
	
	if(num == 2)
	{	
		system("/bin/sh");
		exit(0);
	}
}



