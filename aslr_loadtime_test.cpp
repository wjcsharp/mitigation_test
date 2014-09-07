#include <windows.h>
#define PSAPI_VERSION 1
#include <psapi.h>
#include <stdio.h>
#include "console_color.hpp"
#pragma comment(lib, "psapi")

struct _TEB
{
	NT_TIB Tib;
};

bool ImageIsRelocated( _In_ HMODULE module );
bool PrimaryThreadStackIsRandomized( _In_ PVOID stack_top );
__declspec( dllimport ) int dllimport;
int __cdecl main()
{
	PVOID stack_top = NtCurrentTeb()->Tib.StackBase;
	fputs( "Stack Top:", stdout );
	if( PrimaryThreadStackIsRandomized( stack_top ) )
		printf_green( "%p", stack_top );
	else
		printf( "%p", stack_top );
	fputs( " ", stdout );

	HMODULE exe_handle = GetModuleHandle( nullptr );
	fputs( "EXE:", stdout );
	if( ImageIsRelocated( exe_handle ) )
		printf_green( "%p", exe_handle );
	else
		printf( "%p", exe_handle );
	fputs( " ", stdout );

	HMODULE module;
	if( !GetModuleHandleExW( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<PCWSTR>( &dllimport ), &module ) )
		abort();
	WCHAR dll[MAX_PATH];
	if( !GetModuleBaseNameW( GetCurrentProcess(), module, dll, ARRAYSIZE( dll ) ) )
		abort();
	printf( "%S:", dll );
	if( module )
		if( ImageIsRelocated( module ) )
			printf_green( "%p", module );
		else
			printf( "%p", module );
	else
		abort();
	puts( "" );
}