#include "VX-API.h"

#include <stdio.h>

#define MIN_IMAGE_BASE_ADDRESS 0x10000

PIMAGE_RESOURCE_DIRECTORY UnusedSubroutineFindResourceDirectory(PIMAGE_RESOURCE_DIRECTORY ResourceRootDir, PIMAGE_RESOURCE_DIRECTORY ResourceDir, LPCWSTR ResourceId)
{
	PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(ResourceDir + 1);

	if ((ULONGLONG)ResourceId > MIN_IMAGE_BASE_ADDRESS)
	{
		for (DWORD dwX = 0; dwX < ResourceDir->NumberOfNamedEntries; dwX++)
		{
			WCHAR InitialResourceDirName[256] = { 0 };
			WCHAR IdentifiedResourceDirName[256] = { 0 };

			PIMAGE_RESOURCE_DIR_STRING_U ResourceDirString = (PIMAGE_RESOURCE_DIR_STRING_U)(((PBYTE)ResourceRootDir) + ResourceDirEntry[dwX].NameOffset);

			SecureStringCopyW(InitialResourceDirName, ResourceId, StringLengthW(ResourceId));
			SecureStringCopyW(IdentifiedResourceDirName, ResourceDirString->NameString, ResourceDirString->Length);

			CaplockStringW(InitialResourceDirName);
			CaplockStringW(IdentifiedResourceDirName);

			if (!StringCompareW(InitialResourceDirName, IdentifiedResourceDirName) && ResourceDirEntry[dwX].DataIsDirectory == TRUE)
			{
				return (PIMAGE_RESOURCE_DIRECTORY)(((PBYTE)ResourceRootDir) + (ResourceDirEntry[dwX].OffsetToDirectory & 0x7FFFFFFF));
			}
		}
	}
	else
	{
		for (DWORD dwX = ResourceDir->NumberOfNamedEntries; dwX < ResourceDir->NumberOfIdEntries + ResourceDir->NumberOfNamedEntries; dwX++)
		{
			if (ResourceDirEntry[dwX].Id == (WORD)ResourceId)
			{
				if (ResourceDirEntry[dwX].DataIsDirectory == TRUE)
				{
					return (PIMAGE_RESOURCE_DIRECTORY)(((PBYTE)ResourceRootDir) + (ResourceDirEntry[dwX].OffsetToDirectory & 0x7FFFFFFF));
				}
			}
		}
	}

	return NULL;
}

BOOL GetResourceDataEx(_In_ HMODULE ModuleBase, _In_ LPCWSTR Type, _In_ LPCWSTR Name, _Inout_ PVOID* ResourceRawData, _Inout_ PSIZE_T ResourceDataSize)
{
	PIMAGE_DOS_HEADER Dos = NULL;
	PIMAGE_NT_HEADERS Nt = NULL;
	PIMAGE_FILE_HEADER File = NULL;
	PIMAGE_OPTIONAL_HEADER Optional = NULL;

	PIMAGE_RESOURCE_DIRECTORY ResourceRootDir = NULL;
	PIMAGE_RESOURCE_DIRECTORY ResourceTypeDir = NULL;
	PIMAGE_RESOURCE_DIRECTORY ResourceNameDir = NULL;

	PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirEntry = NULL;
	PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry = NULL;

	if (ModuleBase == NULL)
		return (FALSE);

	RtlLoadPeHeaders(&Dos, &Nt, &File, &Optional, (PBYTE*)&ModuleBase);

	ResourceRootDir = (PIMAGE_RESOURCE_DIRECTORY)((PBYTE)ModuleBase + Optional->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
	ResourceTypeDir = UnusedSubroutineFindResourceDirectory(ResourceRootDir, ResourceRootDir, Type);

	if (ResourceTypeDir == NULL)
		return (FALSE);

	ResourceNameDir = UnusedSubroutineFindResourceDirectory(ResourceRootDir, ResourceTypeDir, Name);
	if (ResourceNameDir == NULL)
		return (FALSE);

	ResourceDirEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(ResourceNameDir + 1);
	ResourceDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(((PBYTE)ResourceRootDir) + (ResourceDirEntry->OffsetToData & 0x7FFFFFFF));

	if (ResourceDirEntry->DataIsDirectory == FALSE)
	{
		*ResourceRawData = (PVOID)(((PBYTE)ModuleBase) + (ResourceDataEntry->OffsetToData));
		*ResourceDataSize = ResourceDataEntry->Size;
		return (TRUE);
	}

	return (FALSE);
}

int main( ) {

	HMODULE Module = GetModuleHandle( NULL );
	PVOID	ResourceRawData = NULL;
	SIZE_T	ResourceSize = 0;

	if ( !GetResourceDataEx( Module, RT_MANIFEST, MAKEINTRESOURCEW( 1 ), &ResourceRawData, &ResourceSize ) )
	{
		printf( "[!] Resource not found\n" );
		return ( -1 );
	}
	
	printf( "[+] Resource Data: %p\n", ResourceRawData );
	printf( "[+] Resource Size: %d\n", ResourceSize );

	return ( 0 );
}