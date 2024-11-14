#ifndef _WIN32_H_
# define _WIN32_H_

#include <Windows.h>

#define U_PTR(x)				((ULONGLONG)(x))
#define MIN_IMAGE_BASE_ADDRESS	0x10000

BOOL GetResourceDataExW( HMODULE Module, LPCWSTR Type, LPCWSTR Name, PVOID* ResourceRawData, PSIZE_T ResourceDataSize );
BOOL FindResourceDirectory( PIMAGE_RESOURCE_DIRECTORY RootDir, PIMAGE_RESOURCE_DIRECTORY ResourceDir, LPCWSTR Id, PIMAGE_RESOURCE_DIRECTORY* ppResourceDir );

#endif // !1
