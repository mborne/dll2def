#include <iostream>
#include <cassert>
#include <cstdio>

#include <windows.h>
#include <winnt.h>

/**
 * Dump dll symbols to generate a .def file
 * adapted from http://stackoverflow.com/questions/1128150/win32-api-to-enumerate-dll-export-functions
 */
int main( int argc, char* argv[] ){
	if ( argc != 2 ){
		std::cerr << "dll2def <dllname>" << std::endl ;
		return 1 ;
	}

	const char* name = argv[1] ;

	HMODULE lib = LoadLibraryEx( name, NULL, DONT_RESOLVE_DLL_REFERENCES);
	if ( ! lib ){
		std::cout << "can't open " << name << std::endl ;
		return 1 ;
	}
	assert(((PIMAGE_DOS_HEADER)lib)->e_magic == IMAGE_DOS_SIGNATURE);
	PIMAGE_NT_HEADERS header = (PIMAGE_NT_HEADERS)( (BYTE *)lib + ((PIMAGE_DOS_HEADER)lib)->e_lfanew );
	assert(header->Signature == IMAGE_NT_SIGNATURE);
	assert(header->OptionalHeader.NumberOfRvaAndSizes > 0);
	PIMAGE_EXPORT_DIRECTORY exports = ( PIMAGE_EXPORT_DIRECTORY ) ( 
		(BYTE *)lib + header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress
	) ;
	PVOID names = (BYTE *)lib + exports->AddressOfNames;
	std::cout << "EXPORTS" << std::endl ;
	for (int i = 0; i < exports->NumberOfNames; i++)
		std::cout << "\t" << ( (BYTE *)lib + ((DWORD *)names)[i]) << std::endl ;
	
	return 0 ;
}

