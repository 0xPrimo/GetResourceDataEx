#include "VX-API.h"

BOOL RtlLoadPeHeaders(_Inout_ PIMAGE_DOS_HEADER* Dos, _Inout_ PIMAGE_NT_HEADERS* Nt, _Inout_ PIMAGE_FILE_HEADER* File, _Inout_ PIMAGE_OPTIONAL_HEADER* Optional, _Inout_ PBYTE* ImageBase)
{
	*Dos = (PIMAGE_DOS_HEADER)*ImageBase;
	if ((*Dos)->e_magic != IMAGE_DOS_SIGNATURE)
		return FALSE;

	*Nt = (PIMAGE_NT_HEADERS)((PBYTE)*Dos + (*Dos)->e_lfanew);
	if ((*Nt)->Signature != IMAGE_NT_SIGNATURE)
		return FALSE;

	*File = (PIMAGE_FILE_HEADER)(*ImageBase + (*Dos)->e_lfanew + sizeof(DWORD));
	*Optional = (PIMAGE_OPTIONAL_HEADER)((PBYTE)*File + sizeof(IMAGE_FILE_HEADER));

	return TRUE;
}

SIZE_T StringLengthW(_In_ LPCWSTR String)
{
	LPCWSTR String2;

	for (String2 = String; *String2; ++String2);

	return (String2 - String);
}

PWCHAR SecureStringCopyW(_Inout_ PWCHAR String1, _In_ LPCWSTR String2, _In_ SIZE_T Size)
{
	PWCHAR pChar = String1;

	while (Size-- && (*String1++ = *String2++) != '\0');

	return pChar;
}

INT StringCompareW(_In_ LPCWSTR String1, _In_ LPCWSTR String2)
{
	for (; *String1 == *String2; String1++, String2++)
	{
		if (*String1 == '\0')
			return 0;
	}

	return ((*(LPCWSTR)String1 < *(LPCWSTR)String2) ? -1 : +1);
}

PWCHAR CaplockStringW(_In_ PWCHAR Ptr)
{
	PWCHAR sv = Ptr;
	while (*sv != '\0')
	{
		if (*sv >= 'a' && *sv <= 'z')
			*sv = *sv - ('a' - 'A');

		sv++;
	}
	return Ptr;
}