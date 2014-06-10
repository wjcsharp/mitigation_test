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
	printf( "      EXE:" );
	if( ImageIsRelocated( exe_handle ) )
		printf_green( "%p\n", exe_handle );
	else
		printf( "%p\n", exe_handle );
	PVOID stack_top = NtCurrentTeb()->Tib.StackBase;
	printf( "Stack Top:" );
	if( PrimaryThreadStackIsRandomized( stack_top ) )
		printf_green( "%p\n", stack_top );
	else
		printf( "%p\n", stack_top );
	const PCWSTR dlls[] = { L"DLL_aslraware", L"DLL_relocatable", L"DLL_fixed" };
	for( int i = 0; i < ARRAYSIZE( dlls ); ++i )
	{
		HMODULE module = GetModuleHandleW( dlls[i] );
		printf( "%S:", dlls[i] );
		if( module )
			if( ImageIsRelocated( module ) )
				printf_green( "%p", module );
			else
				printf( "%p", module );
		else
			printf_red( "null" );
		printf( "  " );
	}
	puts( "" );
}
__declspec( dllimport ) int fixed;
int imp_fixed = fixed;
__declspec( dllimport ) int reloc;
int imp_reloc = reloc;
__declspec( dllimport ) int dynamic;
int imp_dynamic = dynamic;