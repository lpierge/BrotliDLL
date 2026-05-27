/*
	sample.c

	Sample program to check the API of the Brotli DLL.
	Open a Visual Studio Developer Prompt console and use the following command line to compile:

		cl sample.c /Iinclude /link L:\Lib\brotlidll.lib

	(replace "L:\Lib" with the actual pathname of your brotlidll.lib)

	Luca Piergentili, 05/26
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "brotlidll.h"

void main(int argc,char* argv[])
{
	int wide = 0;

	/* checks the API on itself */
	char szArgv0[_MAX_PATH+1] = {0};
	GetModuleFileNameA(NULL,szArgv0,sizeof(szArgv0)-1);

	char szBrotliFilename[1024] = {0};
	snprintf(szBrotliFilename,sizeof(szBrotliFilename),"%s.br",szArgv0);
	wide = printf("\rcompressing %s...",szArgv0);

	BOOL bRet = BrotliCompressFile(szArgv0,szBrotliFilename);
	if(bRet)
	{
		char szOriginalFilename[1024] = {0};
		snprintf(szOriginalFilename,sizeof(szOriginalFilename),"%s.original",szArgv0);
		wide = printf("\runcompressing %s...",szBrotliFilename);
		bRet = BrotliUncompressFile(szBrotliFilename,szOriginalFilename);
	}

	printf("\r%*s\rDLL and brotli interface %s working correctly\n",++wide," ",bRet ? "are" : "are NOT");
}
