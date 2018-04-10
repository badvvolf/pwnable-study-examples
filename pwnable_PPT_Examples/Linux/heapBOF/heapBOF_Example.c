
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	char * input = malloc(10);
	char * secret = malloc(10);
	
	printf("input addr : %p \n", input);
	printf("secret message addr : %p\n", secret);


	char * message = "Secret~\n";
	strncpy(secret, message, strlen(message));

	read(0, input, 50);

	printf("%s\n", input);

}


