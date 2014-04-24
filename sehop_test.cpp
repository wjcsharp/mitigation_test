#ifndef _M_IX86
#error SEHOP is x86 arch only.
#endif
#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>
#include "console_color.hpp"

struct _TEB
{
	NT_TIB32 Tib;
};

LONG CALLBACK ExceptionRoutine( _In_ PEXCEPTION_POINTERS exception_information )
{
	if( exception_information->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT )
	{
		puts_green( "Enable" );
		ExitProcess( EXIT_SUCCESS );
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
void __declspec( noinline ) RaiseSEH()
{
	printf( "SEHOP " );
	__try
	{
		__debugbreak();
	}
	__except( GetExceptionCode() == EXCEPTION_BREAKPOINT ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH )
	{
		puts_red( "Disable" );
	}
}
int __cdecl main()
{
	_CrtSetReportMode( _CRT_WARN, IsDebuggerPresent() ? _CRTDBG_MODE_DEBUG : _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
#ifndef NO_VEH
	AddVectoredContinueHandler( 0, ExceptionRoutine );
#endif
	NtCurrentTeb()->Tib.ExceptionList = ULONG_MAX;
	RaiseSEH();
}