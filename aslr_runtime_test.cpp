#include <windows.h>
#include <stdio.h>
#include "console_color.hpp"

struct _TEB
{
	NT_TIB Tib;
};

bool ImageIsRelocated( _In_ HMODULE module );
bool PrimaryThreadStackIsRandomized( _In_ PVOID stack_top );
int __cdecl main()
{
	HMODULE exe_handle = GetModuleHandle( nullptr );
	fputs( "      EXE:", stdout );
	if( ImageIsRelocated( exe_handle ) )
		printf_green( "%p\n", exe_handle );
	else
		printf( "%p\n", exe_handle );

	PVOID stack_top = NtCurrentTeb()->Tib.StackBase;
	fputs( "Stack Top:", stdout );
	if( PrimaryThreadStackIsRandomized( stack_top ) )
		printf_green( "%p\n", stack_top );
	else
		printf( "%p\n", stack_top );

	const PCWSTR dlls[] = { L"DLL_aslraware", L"DLL_relocatable", L"DLL_fixed" };
	for( int i = 0; i < ARRAYSIZE( dlls ); ++i )
	{
		HMODULE module = LoadLibraryW( dlls[i] );
		DWORD error = GetLastError();
		printf( "%S:", dlls[i] );
		if( module )
			if( ImageIsRelocated( module ) )
				printf_green( "%p", module );
			else
				printf( "%p", module );
		else
			printf_red( "@err=%08lx", error );
		fputs( "  ", stdout );
	}
	puts( "" );
}