//Q90493
//ms-help://MS.VSCC/MS.MSDNVS/kbwin32/Source/win32sdk/q90493.htm
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#ifndef IMAGE_SIZEOF_NT_OPTIONAL_HEADER
#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER 224
#endif

#define DllExport   __declspec( dllexport )

DllExport BOOL WINAPI DllMain (HANDLE hModule, DWORD dwFunction, LPVOID lpNot){
	return TRUE;
}

DWORD AbsoluteSeek(HANDLE hFile,
	DWORD  offset)
{
	DWORD newOffset;
	if ((newOffset = SetFilePointer(hFile,
		offset,
		NULL,
		FILE_BEGIN)) == 0xFFFFFFFF)
	{
		printf("SetFilePointer failed, error %lu.\n", GetLastError());
		//exit(1);
		return -1;
	}
	return newOffset;
}

int ReadBytes(HANDLE hFile, LPVOID buffer, DWORD  size)
{
	DWORD bytes;
	if (!ReadFile(hFile, buffer, size, &bytes, NULL))
	{
		printf("ReadFile failed, error %lu.\n", GetLastError());
		//exit(1);
		return -1;
	}
	else if (size != bytes)
	{
		printf("Read the wrong number of bytes, expected %lu, got %lu.\n",
			size, bytes);
		// exit(1);
		return -1;
	}
	return 0;
}

/*
   IMAGE_SUBSYSTEM_NATIVE               1
   IMAGE_SUBSYSTEM_WINDOWS_GUI          2
   IMAGE_SUBSYSTEM_WINDOWS_CUI          3
   IMAGE_SUBSYSTEM_OS2_CUI              5
   IMAGE_SUBSYSTEM_POSIX_CUI            7
   IMAGE_SUBSYSTEM_NATIVE_WINDOWS       8
   IMAGE_SUBSYSTEM_WINDOWS_CE_GUI       9 
   IMAGE_SUBSYSTEM_EFI_APPLICATION =     10;  //
   IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER = 11;   //
   IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER =  12;  //
   IMAGE_SUBSYSTEM_EFI_ROM =             13;
   IMAGE_SUBSYSTEM_XBOX =                14;
   IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION = 16;
   IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG =   17;
   on error returns -1
*/

DllExport int GetSubSystem(PTCHAR exeName)
{
	HANDLE hImage; 
	DWORD  bytes;
    DWORD  iSection;
    DWORD  SectionOffset;
    DWORD  CoffHeaderOffset;
    DWORD  MoreDosHeader[16]; 
	ULONG  ntSignature; 
	IMAGE_DOS_HEADER      image_dos_header;
    IMAGE_FILE_HEADER     image_file_header;
    IMAGE_OPTIONAL_HEADER image_optional_header;
    IMAGE_SECTION_HEADER  image_section_header;
	int res = -1;

	if(exeName == NULL) return -1;

	hImage = CreateFile(exeName,
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL); 

	if (INVALID_HANDLE_VALUE == hImage)
    {
        printf("Could not open %s, error %lu\n", exeName, GetLastError());
		//exit(1);
		return -1;
    }

	/*
     *  Read the MS-DOS image header.
     */ 
    res = ReadBytes(hImage, &image_dos_header, sizeof(IMAGE_DOS_HEADER));

	if(res == -1) return -1;
	if (IMAGE_DOS_SIGNATURE != image_dos_header.e_magic)
    {
       printf("Sorry, I do not understand this file.\n");
       // exit(1);
		return -1;
    } 
	/*
     *  Read more MS-DOS header.
	 */ 
    res = ReadBytes(hImage, MoreDosHeader, sizeof(MoreDosHeader)); 
	if (res == -1)
	{
		printf("Reading DOS header failed.\n");
		return -1;
	}

	/*
     *  Get actual COFF header.
     */ 
    CoffHeaderOffset = AbsoluteSeek(hImage, image_dos_header.e_lfanew) + sizeof(ULONG);
	
	if (CoffHeaderOffset == -1)
	{
		printf("Reading COFF header failed.\n");
		return -1;
	}


	res = ReadBytes (hImage, &ntSignature, sizeof(ULONG));
	
	if (res == -1)
	{
		printf("Reading NTSignature failed.\n");
		return -1;
	}

	
	if (IMAGE_NT_SIGNATURE != ntSignature)
    {
     printf("Missing NT signature. Unknown file type.\n");
     //exit(1);
		return -1;
    }

	SectionOffset = CoffHeaderOffset + IMAGE_SIZEOF_FILE_HEADER +
                    IMAGE_SIZEOF_NT_OPTIONAL_HEADER;    ReadBytes(hImage,
              &image_file_header,
              IMAGE_SIZEOF_FILE_HEADER);

	/*
     *  Read optional header.
     */ 
    res = ReadBytes(hImage,
              &image_optional_header,
              IMAGE_SIZEOF_NT_OPTIONAL_HEADER);
	
	if (res == -1)
	{
		printf("Reading optional header failed.\n");
		return -1;
	}

	
	return 
		image_optional_header.Subsystem;
}


