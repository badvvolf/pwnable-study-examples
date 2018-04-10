
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

void myFunction()
{
	printf("원본 함수 실행!\n");
}

void weirdFunction()
{
	printf("오버플로우로 실행시킨 함수!\n");
}


int main()
{
	char * input = malloc(10);

	//함수 포인터를 저장할 변수
	void ** fp = malloc(sizeof(void *));

	*fp = myFunction;

	printf("input addr : %p \n", input);
	printf("function pointer addr : %p\n", fp);


	//원본 함수 포인터 이용
	((void (*)())*fp)();
	

	//OVERFLOW!!
	read(0, input, 50);

	//조작된 함수 포인터 이용
	((void (*)())*fp)();

}


