#ifndef _M_IX86
#error DEP test is meaningful only on x86 arch.
#endif
#include <windows.h>
#include <cstdio>
#include "console_color.hpp"

#pragma comment(linker, "/SECTION:.noexec,!E")
#pragma code_seg(".noexec")
EXTERN_C void __cdecl mainCRTStartup();

void __declspec( naked ) _Maybe_raises_SEH_exception_ NXViolationStartup()
{
	__asm jmp mainCRTStartup
}
#pragma code_seg()
void PrintCurrentProcessDEPPolicy()
{
	DWORD flags;
	BOOL  permanent;
	if( !GetProcessDEPPolicy( GetCurrentProcess(), &flags, &permanent ) )
	{
		DWORD last_error = GetLastError();
		printf_red( "!GetProcessDEPPolicy" );
		printf( "->GetLastError()==%08lx\n", last_error );
	}
	else
	{
		printf( "GetProcessDEPPolicy()==" );
		if( !flags )
			puts_red( "Disable" );
		else
		{
			if( flags & PROCESS_DEP_ENABLE )
				printf_green( "Enable" );
			if( !( flags & PROCESS_DEP_DISABLE_ATL_THUNK_EMULATION ) )
				printf( " with ATL thunk emulation" );
			if( permanent == TRUE )
				printf( "(Permanent)" );
			puts( "" );
		}
	}
}
int __cdecl main()
{
	// Windows 8/8.1 bug workaround
	const BYTE mov_edi_edi[2] = { 0x8B, 0xFF };
	const BYTE xor_ecx_ecx[2] = { 0x33, 0xC9 };
	if( memcmp( GetProcessDEPPolicy, mov_edi_edi, sizeof mov_edi_edi ) == 0 )
		WriteProcessMemory( GetCurrentProcess(), GetProcessDEPPolicy, xor_ecx_ecx, sizeof xor_ecx_ecx, NULL );
	else
		fputs( "GetProcessDEPPolicy() entry point is not \"MOV EDI, EDI\"\n", stderr );

	PrintCurrentProcessDEPPolicy();
	// Vb40032.dll http://support.microsoft.com/kb/196286
	if( LoadLibraryW( L"vb40032" ) )
	{
		puts( "NX incompatible DLL load" );
		PrintCurrentProcessDEPPolicy();
	}
	if( !SetProcessDEPPolicy( PROCESS_DEP_ENABLE | PROCESS_DEP_DISABLE_ATL_THUNK_EMULATION ) )
	{
		DWORD last_error = GetLastError();
		printf_red( "!SetProcessDEPPolicy" );
		printf( "->GetLastError()==%08lx\n", last_error );
	}
	else
		puts( "SetProcessDEPPolicy()" );
	PrintCurrentProcessDEPPolicy();
}
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
LONG CALLBACK NXHandler( _Inout_ PEXCEPTION_POINTERS exception_information )
{
	RemoveVectoredExceptionHandler( NXHandler );
	ULONG_PTR base = reinterpret_cast<ULONG_PTR>( &__ImageBase );
	PIMAGE_NT_HEADERS nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>( base + ( &__ImageBase )->e_lfanew );
	if(
		exception_information->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION
		&& exception_information->ExceptionRecord->ExceptionInformation[0] == EXCEPTION_EXECUTE_FAULT
		&& exception_information->ContextRecord->Eip == base + nt_header->OptionalHeader.AddressOfEntryPoint
		)
	{
		const char str[] = "Entry point NX fault\n";
		DWORD junk;
		ChangeConsoleTextColor color( FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY );
		WriteConsoleA( GetStdHandle( STD_OUTPUT_HANDLE ), str, strlen( str ), &junk, NULL );
		exception_information->ContextRecord->Eip = PtrToUlong( mainCRTStartup );
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else
		return EXCEPTION_CONTINUE_SEARCH;
}
VOID NTAPI SetupNXHandler( PVOID, DWORD Reason, PVOID )
{
	switch( Reason )
	{
	case DLL_PROCESS_ATTACH:
		AddVectoredExceptionHandler( 0, NXHandler );
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	DEFAULT_UNREACHABLE;
	}
}
#pragma comment(linker, "/INCLUDE:__tls_used")
#pragma const_seg(".CRT$XLB")
extern const PIMAGE_TLS_CALLBACK XLB = SetupNXHandler;
#pragma comment(lib, "user32")
#pragma comment(linker, "/INCLUDE:_CopyRect@8") // Some Windows versions, user32.dll is needed for TLS callback. I don't know this reason.