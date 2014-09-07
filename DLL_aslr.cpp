__declspec( dllexport ) int dllimport;

#pragma comment(linker,"/Entry:DllMain")
int __stdcall DllMain( void*, unsigned long, void* )
{
	return 1;
}