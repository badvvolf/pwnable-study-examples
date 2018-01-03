#if 1

//ÇÔ¼ö ÁÖ¼Ò ¾Ë¾Æ³»±â
#include <windows.h>
#include <stdio.h>

int main()
{
	HANDLE ker32 = LoadLibraryA("Kernel32.dll");
	void * fp = GetProcAddress(ker32, "WinExec");
	printf("function address : %x\n", fp);

	fp = GetProcAddress(ker32, "ExitProcess");
	printf("function address : %x\n", fp);
}

#elif 1

int main()
{
	__asm{
	
			mov ebp, esp

			xor eax, eax
			add eax, 40h
			shl eax, 4h
			sub esp, eax

			lea eax, [esp-4]

			xor ebx, ebx
			add ebx, 64h
			shl ebx, 8h
			add ebx, 6Dh
			shl ebx, 8h
			add ebx, 63h
			push ebx

			push 5
			push eax

			push 76h
			pop eax
			shl eax, 4h
			add eax, 8h
			shl eax, 4h
			add eax, 4h
			add eax, 5h
			shl eax, 8h
			add eax, 2bh
			shl eax, 8h

			call eax

			mov eax, 76853cb0h
			push 1
			call eax
	};

}


#endif
