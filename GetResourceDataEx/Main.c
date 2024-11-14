#include "Win32.h"

#include <stdio.h>

int main( ) {

	HMODULE Module = GetModuleHandle( NULL );
	PVOID	ResourceRawData = NULL;
	SIZE_T	ResourceSize = 0;

	if ( !GetResourceDataExW( Module, RT_MANIFEST, MAKEINTRESOURCEW( 2 ), &ResourceRawData, &ResourceSize ) )
	{
		printf( "[!] Resource not found\n" );
		return ( -1 );
	}
	
	printf( "[+] Resource Data: %p\n", ResourceRawData );
	printf( "[+] Resource Size: %d\n", ResourceSize );

	return ( 0 );
}