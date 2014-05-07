#include <windows.h>
#include <stdio.h>
#include "console_color.hpp"
#pragma comment(lib, "dbghelp")

struct _TEB
{
	NT_TIB Tib;
};

bool ImageIsRelocated( _In_ HMODULE module )
{
	WCHAR file_name[MAX_PATH];
	GetModuleFileNameW( module, file_name, ARRAYSIZE( file_name ) );
	HANDLE file = CreateFileW( file_name, FILE_READ_DATA, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr );
	OVERLAPPED overlapped = {};
	overlapped.Offset = FIELD_OFFSET( IMAGE_DOS_HEADER, e_lfanew );
	decltype( IMAGE_DOS_HEADER::e_lfanew ) e_lfanew;
	ReadFile( file, &e_lfanew, sizeof( e_lfanew ), nullptr, &overlapped );
	overlapped.Offset = e_lfanew + FIELD_OFFSET( IMAGE_NT_HEADERS, OptionalHeader ) + FIELD_OFFSET( decltype( IMAGE_NT_HEADERS::OptionalHeader ), ImageBase );
	decltype( decltype( IMAGE_NT_HEADERS::OptionalHeader )::ImageBase ) ImageBase;
	ReadFile( file, &ImageBase, sizeof( ImageBase ), nullptr, &overlapped );
	CloseHandle( file );
	return reinterpret_cast<HMODULE>( ImageBase ) != module;
}
#pragma warning(push)
#pragma warning(disable:4996)
BOOL PrimaryThreadStackIsRandomized( _In_ PVOID stack_top )
{
	BOOL run_under_wow64 = IsWow64Process( GetCurrentProcess(), &run_under_wow64 ) ? run_under_wow64 : FALSE;
	DWORD windows_version = GetVersion();
	switch( LOBYTE( LOWORD( windows_version ) ) )
	{
	case 6:
		switch( HIBYTE( LOWORD( windows_version ) ) )
		{
		case 3:
			__fallthrough;
		case 2:
			return stack_top != ( run_under_wow64 ? ULongToPtr( 0x190000 ) : ULongToPtr( 0x140000 ) );
		case 1:
			return stack_top != ( run_under_wow64 ? ULongToPtr( 0x190000 ) : ULongToPtr( 0x130000 ) );
		case 0:
			return stack_top != ( run_under_wow64 ? ULongToPtr( 0x180000 ) : ULongToPtr( 0x130000 ) );
		default:
			__fallthrough;
		}
	default:
		__fallthrough;
	}
	return FALSE;
}
#pragma warning(pop)
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
	static const WCHAR dlls[][2] = { L"A", L"B", L"C", L"D", L"E", L"F" };
	for( int i = 0; i < ARRAYSIZE( dlls ); ++i )
	{
		HMODULE module = LoadLibraryW( dlls[i] );
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