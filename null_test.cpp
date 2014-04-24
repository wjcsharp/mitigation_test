#define _CRT_SECURE_NO_WARNINGS
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>
#include <winternl.h>
#include <stdio.h>
#include "console_color.hpp"
#pragma comment(lib, "ntdll")

EXTERN_C NTSYSAPI NTSTATUS NTAPI ZwAllocateVirtualMemory(
	_In_     HANDLE ProcessHandle,
	_Inout_  PVOID* BaseAddress,
	_In_     ULONG_PTR ZeroBits,
	_Inout_  PSIZE_T RegionSize,
	_In_     ULONG AllocationType,
	_In_     ULONG Protect
	);
int __cdecl main()
{
	PVOID address = reinterpret_cast<PVOID>( 1 );
	SIZE_T length = 0x200;
	NTSTATUS status = ZwAllocateVirtualMemory( GetCurrentProcess(), &address, 0, &length, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
	if( NT_SUCCESS( status ) )
	{
		*reinterpret_cast<volatile int*>( 0 ) = 0;
		puts_red( strcpy( reinterpret_cast<char*>( 1 ), "null page allocated" ) );
	}
	else switch( status )
	{
	case STATUS_INVALID_PARAMETER_2:
		puts_green( "null page alloc failed (may Windows 8 or MS13-031)" );
		break;
	case STATUS_CONFLICTING_ADDRESSES:
		puts_green( "null page alloc failed (may EMET)" );
		break;
	default:
		printf_red( "!ZwAllocateVirtualMemory" );
		printf( "->0x%lx\n", status );
	}
	return EXIT_SUCCESS;
}