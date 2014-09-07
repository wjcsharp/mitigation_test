#include <windows.h>

/**
	`module::IMAGE_NT_HEADERS::OptionalHeader::ImageBase == module` can't use.
	VC++ made ASLR aware DLL's ImageBase is in relocation infomation.
*/
bool ImageIsRelocated( _In_ HMODULE module )
{
	WCHAR file_name[MAX_PATH];
	if( !GetModuleFileNameW( module, file_name, ARRAYSIZE( file_name ) ) )
		abort();
	HANDLE file = CreateFileW( file_name, FILE_READ_DATA, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr );
	if( file == INVALID_HANDLE_VALUE )
		abort();
	OVERLAPPED overlapped = {};
	overlapped.Offset = FIELD_OFFSET( IMAGE_DOS_HEADER, e_lfanew );
	ULONG e_lfanew;
	if( !ReadFile( file, &e_lfanew, sizeof e_lfanew, nullptr, &overlapped ) )
		abort();
	overlapped.Offset = e_lfanew + FIELD_OFFSET( IMAGE_NT_HEADERS, OptionalHeader ) + FIELD_OFFSET( IMAGE_OPTIONAL_HEADER, ImageBase );
	HMODULE ImageBase;
	if( !ReadFile( file, &ImageBase, sizeof ImageBase, nullptr, &overlapped ) )
		abort();
	CloseHandle( file );
	return ImageBase != module;
}
bool PrimaryThreadStackIsRandomized( _In_ PVOID stack_top )
{
	BOOL run_under_wow64 = IsWow64Process( GetCurrentProcess(), &run_under_wow64 ) ? run_under_wow64 : FALSE;
#pragma warning(push)
#pragma warning(disable:4996)
#pragma warning(disable:28159)
	DWORD windows_version = GetVersion();
#pragma warning(pop)
	switch( LOBYTE( LOWORD( windows_version ) ) )
	{
	case 6:
		switch( HIBYTE( LOWORD( windows_version ) ) )
		{
		case 3:
			__fallthrough;// Windows 8.1 use same address as Windows 8.
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
	return false;
}