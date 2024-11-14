#include "Win32.h"


BOOL GetResourceDataExW( HMODULE Module, LPCWSTR Type, LPCWSTR Name, PVOID* ResourceRawData, PSIZE_T ResourceDataSize )
{
	ULONGLONG						ImageBase = NULL;
	PIMAGE_OPTIONAL_HEADER			Optional = NULL;
	PIMAGE_RESOURCE_DIRECTORY		ResourceDir = NULL,
		ResourceRootDir = NULL,
		ResourceTypeDir = NULL,
		ResourceNameDir = NULL;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirEntry = NULL;
	PIMAGE_RESOURCE_DATA_ENTRY      ResourceDataEntry = NULL;

	ImageBase = ( ULONGLONG ) Module;
	Optional = ( PIMAGE_OPTIONAL_HEADER ) ( U_PTR( ImageBase ) + ( ( PIMAGE_DOS_HEADER ) ImageBase )->e_lfanew + sizeof( DWORD ) + sizeof( IMAGE_FILE_HEADER ) );
	ResourceDir = ( PIMAGE_RESOURCE_DIRECTORY ) ( U_PTR( ImageBase ) + Optional->DataDirectory[ IMAGE_DIRECTORY_ENTRY_RESOURCE ].VirtualAddress );
	ResourceRootDir = ResourceDir;

	// find resource type directory
	if ( !FindResourceDirectory( ResourceRootDir, ResourceDir, Type, &ResourceTypeDir ) )
		return ( FALSE );

	// find resource name directory
	if ( !FindResourceDirectory( ResourceRootDir, ResourceTypeDir, Name, &ResourceNameDir ) )
		return ( FALSE );

	// get resource raw data
	ResourceDirEntry = ( PIMAGE_RESOURCE_DIRECTORY_ENTRY ) ( ResourceNameDir + 1 );
	ResourceDataEntry = ( PIMAGE_RESOURCE_DATA_ENTRY ) ( U_PTR( ResourceRootDir ) + ( ResourceDirEntry->OffsetToData & 0x7FFFFFFF ) );
	if ( !ResourceDirEntry->DataIsDirectory ) {
		*ResourceRawData = ( PVOID ) ( ImageBase + ( ResourceDataEntry->OffsetToData ) );
		*ResourceDataSize = ResourceDataEntry->Size;
		return ( TRUE );
	}
}

BOOL FindResourceDirectory( PIMAGE_RESOURCE_DIRECTORY RootDir, PIMAGE_RESOURCE_DIRECTORY ResourceDir, LPCWSTR Id, PIMAGE_RESOURCE_DIRECTORY* ppResourceDir )
{
	PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirEntry = NULL;

	ResourceDirEntry = ( PIMAGE_RESOURCE_DIRECTORY_ENTRY ) ( ResourceDir + 1 );
	// resource id is named
	if ( U_PTR( Id ) > MIN_IMAGE_BASE_ADDRESS )
	{
		// search in the named resources
		for ( DWORD Index = 0; Index < ResourceDir->NumberOfNamedEntries; Index++ )
		{
			WCHAR							ResourceDirName[ 256 ] = { 0 };
			PIMAGE_RESOURCE_DIR_STRING_U	ResourceDirString = NULL;

			ResourceDirString = ( PIMAGE_RESOURCE_DIR_STRING_U ) ( U_PTR( RootDir ) + ResourceDirEntry[ Index ].NameOffset );

			lstrcpynW( ResourceDirName, ResourceDirString->NameString, ResourceDirString->Length + 1 );

			if ( !lstrcmpiW( ResourceDirName, Id ) && ResourceDirEntry[ Index ].DataIsDirectory )
			{
				*ppResourceDir = ( PIMAGE_RESOURCE_DIRECTORY ) ( U_PTR( RootDir ) + ( ResourceDirEntry[ Index ].OffsetToDirectory & 0x7FFFFFFF ) );
				return ( TRUE );
			}
		}
	}
	// resource id is predefined
	else
	{
		for ( DWORD Index = ResourceDir->NumberOfNamedEntries; Index < ResourceDir->NumberOfIdEntries + ResourceDir->NumberOfNamedEntries; Index++ ) {
			if ( ResourceDirEntry[ Index ].Id == ( WORD ) Id ) {

				if ( ResourceDirEntry[ Index ].DataIsDirectory )
				{
					*ppResourceDir = ( PIMAGE_RESOURCE_DIRECTORY ) ( U_PTR( RootDir ) + ( ResourceDirEntry[ Index ].OffsetToDirectory & 0x7FFFFFFF ) );
					return ( TRUE );
				}
			}
		}
	}

	return ( FALSE );
}