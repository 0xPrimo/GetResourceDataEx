#ifndef _WIN32_H_
# define _WIN32_H_

#include <Windows.h>

PWCHAR	SecureStringCopyW(_Inout_ PWCHAR String1, _In_ LPCWSTR String2, _In_ SIZE_T Size);
INT		StringCompareW(_In_ LPCWSTR String1, _In_ LPCWSTR String2);
SIZE_T	StringLengthW(_In_ LPCWSTR String);
PWCHAR	CaplockStringW(_In_ PWCHAR Ptr);

BOOL	RtlLoadPeHeaders(_Inout_ PIMAGE_DOS_HEADER* Dos, _Inout_ PIMAGE_NT_HEADERS* Nt, _Inout_ PIMAGE_FILE_HEADER* File, _Inout_ PIMAGE_OPTIONAL_HEADER* Optional, _Inout_ PBYTE* ImageBase);

#endif // !1
