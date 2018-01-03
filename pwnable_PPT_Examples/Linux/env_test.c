#include<stdio.h>
int main(int argc, char * argv[], char * envp[])
{
	printf("argc 주소 : %p\n", &argc);
	printf("argv 주소 : %p\n", &argv);
	printf("envp 주소 : %p\n", &envp);
	printf("envp가 가리키는 배열의 시작 주소 : %p\n", &envp[0]);

	//스택에 저장된 환경변수 접근
	for(int i =0; envp[i] != NULL; i++)
	{
		printf("배열 주소 : %p, 저장하는 포인터 값 : %p , 환경변수 값 : %s\n", &envp[i], envp[i], (char*)(envp[i]));
	}
}


