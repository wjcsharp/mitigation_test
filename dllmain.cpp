//  http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
#include <windows.h>
#pragma comment(lib, "kernel32")
#pragma comment(linker, "/Entry:DllMain")

#pragma optimize( "t", on )
BOOL APIENTRY DllMain( _In_ HMODULE hModule, DWORD ul_reason_for_call, _In_opt_ LPVOID lpReserved )
{
	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( hModule );
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if( lpReserved == NULL )
		{
			__noop;
		}
		break;
	DEFAULT_UNREACHABLE;
	}
	return TRUE;
}
#pragma optimize( "", on )