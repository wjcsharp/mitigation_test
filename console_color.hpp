#pragma once
#include <windows.h>
#include <cstdio>

struct ChangeConsoleTextColor
{
	ChangeConsoleTextColor( const WORD text_color )
		: console( GetStdHandle( STD_OUTPUT_HANDLE ) )
		, restore_color( GetCurrentConsoleColor() )
	{
		SetConsoleTextAttribute( console, text_color );
	}
	~ChangeConsoleTextColor()
	{
		SetConsoleTextAttribute( console, restore_color );
	}
private:
	const HANDLE console;
	const WORD restore_color;
	WORD GetCurrentConsoleColor() const
	{
		CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
		GetConsoleScreenBufferInfo( console, &console_screen_buffer_info );
		return console_screen_buffer_info.wAttributes;
	}
	ChangeConsoleTextColor( const ChangeConsoleTextColor& );
	ChangeConsoleTextColor& operator=( const ChangeConsoleTextColor& );
};

int color_printf( WORD text_color, _In_z_ _Printf_format_string_ const char * __restrict Format, ... )
{
	va_list ap;
	ChangeConsoleTextColor color( text_color );
	va_start( ap, Format );
	int result = vprintf( Format, ap );
	va_end( ap );
	return result;
}
#define printf_red(...) color_printf(FOREGROUND_INTENSITY | FOREGROUND_RED, __VA_ARGS__)
#define printf_green(...) color_printf(FOREGROUND_INTENSITY | FOREGROUND_GREEN, __VA_ARGS__)

int color_puts( WORD text_color, _In_z_ const char * __restrict s )
{
	ChangeConsoleTextColor color( text_color );
	return puts( s );
}
#define puts_red(s) color_puts(FOREGROUND_INTENSITY | FOREGROUND_RED, s)
#define puts_green(s) color_puts(FOREGROUND_INTENSITY | FOREGROUND_GREEN, s)
#define puts_yellow(s) color_puts(FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED, s)