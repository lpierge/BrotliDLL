/*
	brotlidll.h
	Windows DLL wrapper for Google Brotli.
	Luca Piergentili, 09/2025 (updated 2026)

	see notes in brotlidll.c
*/
#ifndef _BROTLIDLL_H
#define _BROTLIDLL_H 1

#include <windows.h>

#ifdef BROTLIDLL_EXPORTS
  /* internally used by the DLL code */
  #define BROTLIDLL_API __declspec(dllexport)
#else
  /* for the program using the DLL */
  #define BROTLIDLL_API __declspec(dllimport)
  #ifdef _DEBUG
    #pragma comment(lib,"brotlidll.d.lib")
  #else
    #pragma comment(lib,"brotlidll.lib")
  #endif
  #ifdef PRAGMA_MESSAGE_VERBOSE
    #pragma message("\t\t\t"__FILE__"("STR(__LINE__)"): automatically linking with brotlidll")
  #endif
#endif

#ifdef __cplusplus
	extern "C" {
#endif

BROTLIDLL_API BOOL BrotliCompressFile	(LPCSTR lpcszInputFile,LPCSTR lpcszOutputFile);
BROTLIDLL_API BOOL BrotliUncompressFile	(LPCSTR lpcszInputFile,LPCSTR lpcszOutputFile);

#ifdef __cplusplus
	}
#endif

#endif // _BROTLIDLL_H
