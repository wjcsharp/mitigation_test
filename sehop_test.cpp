#ifndef _M_IX86
#error SEHOP is x86 arch only.
#endif
#include <windows.h>
#include <intrin.h>
#include <stdio.h>
#include "console_color.hpp"

LONG CALLBACK CatchUnhandledException( _In_ PEXCEPTION_POINTERS exception_information )
{
	if( exception_information->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT )
	{
		puts_green( "Enable" );
		ExitProcess( EXIT_SUCCESS );
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
EXTERN_C EXCEPTION_DISPOSITION __cdecl ExceptionRoutine( _In_ EXCEPTION_RECORD, _In_ PVOID, _Inout_ PCONTEXT, _Inout_ PVOID )
{
	puts_red( "Disable" );
	ExitProcess( EXIT_SUCCESS );
}
#pragma warning(push)
#pragma warning(disable:4733)
int __cdecl main()
{
#ifndef NO_VEH
	AddVectoredContinueHandler( 0, CatchUnhandledException );
#endif
	printf( "SEHOP " );
	DWORD ExceptionRecord[2] = { ULONG_MAX, PtrToUlong( ExceptionRoutine ) };
	__writefsdword( FIELD_OFFSET( NT_TIB, ExceptionList ), PtrToUlong( &ExceptionRecord ) );
	RaiseException( EXCEPTION_BREAKPOINT, EXCEPTION_NONCONTINUABLE, 0, nullptr );
	__assume( 0 );
}
#pragma warning(pop)