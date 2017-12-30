#include<stdio.h>
int main(int argc, char * argv[], char * envp[])
{
	printf("argc 주소 : %p\n", &argc);
	printf("argv 주소 : %p\n", &argv[0]);
	printf("envp 주소 : %p\n", &envp[0]);
	
	//스택에 저장된 환경변수 접근
	for(int i =0; envp[i] != NULL; i++)
	{
		printf("메인함수 인자 주소 : %p, 저장하는 포인터 값 : %p , 환경변수 값 : %s\n", &envp[i], envp[i], (char*)(envp[i]));
	}
}



