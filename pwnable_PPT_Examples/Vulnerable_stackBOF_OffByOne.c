//Release 모드, 최적화 없앰, DEP 해제, ASLR 해제, SG 해제

#include<stdio.h>
#include<string.h>

int vulfunc(char * buf)
{
	char input[100];
	strncpy(input, buf, 101);
}

int main(int argc, char *argv[])
{
	vulfunc(argv[1]);

	return 0;
}



