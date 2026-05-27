/*
	brotlidll.c
	Windows DLL wrapper for Google Brotli.
	Luca Piergentili, 09/2025 (updated 2026)

	Overview:
	This project builds a Windows DLL that exposes two high-level functions to compress and
	decompress files using the Brotli (.br) format, a Google's open-source data compression 
	algorithm: https://www.brotli.org/

	The problem:
	The official Brotli package is designed primarily around Unix/CMake build behaviors and 
	because of this, the raw source code cannot be used directly within a standard Visual 
	Studio project on Windows. This architectural mismatch causes file naming collisions when
	importing the files into VS and requires a set of specific preprocessor macros to be defined
	manually that the official documentation completely omits for native Windows builds.

	The solution:
	The DLL provided here solves this issue by modifying and integrating the original Brotli
	source code into a ready-to-use Windows library in the least invasive way possible, as 
	detailed in the steps below.

	Development notes:
	The Visual Studio version used to build this project is the 2022 edition.
	The project configuration for the DLL relies on a virtual drive (L:) as the root working 
	directory. You can change this path in the project settings or map a local directory to
	the L: drive using the following instruction from the command prompt:
	SUBST L: C:\<YOUR_PATH_HERE>
	The original Brotli package must be downloaded and installed in the 'L:\brotli' directory.
	The Visual Studio project for the DLL must be downloaded and installed in the 'L:\brotlidll'
	directory.

	Step 1: Downloading and installing the official package
	-------------------------------------------------------
	1. Download the Brotli package from the offical repository https://github.com/google/brotli 
	   (Green button "Code" -> "Download ZIP").
	2. Extract the package into: 'L:\brotli'
	3. Open the "Developer Command Prompt for VS 2022", navigate to 'L:\brotli', create a 'build'
	   subdirectory, enter it, and run the following command:
	   cmake .. -G "Visual Studio 17 2022"
	4. Open the generated "brotli.sln" (inside the build directory) using Visual Studio to look
	   at the official "brotli" project and see their command-line utility.

	Step 2: Making required modifications to the official package
	-------------------------------------------------------------
	To prevent object file name shadowing and compilation conflicts within Visual Studio, you must
	duplicate and rename specific source files in 'c\enc' and 'c\dec' subdirectories of 'L:\brotli'
	as follows(*):

	1. Duplicate (copy) 'static_init.*' files into 'static_init_enc.*' and 'static_init_dec.*'

	2. Duplicate (copy) 'encode.c' and 'decode.c' into 'encode_enc.c' and 'decode_dec.c'
	
	3. Update the inclusion directives within the newly created files as follows:
	   static_init_enc.c  -> change: #include "static_init.h" to: #include "static_init_enc.h"
	   encode_enc.c       -> change: #include "static_init.h" to: #include "static_init_enc.h"
	   static_init_dec.c  -> change: #include "static_init.h" to: #include "static_init_dec.h"
	   decode_dec.c       -> change: #include "static_init.h" to: #include "static_init_dec.h"

	(*) the DLL project directory contains 'brotli.1.2.0.zip', which is a pre-patched version of
	    the original Brotli package, provided to bypass the manual modifications detailed above;
	    to use it instead of the original package, unzip the file in the 'L:\brotli' directory

	Step 3: Building the DLL
	------------------------
	1. Start Visual Studio and open the 'brotlidll.sln' solution located in 'L:\brotlidll'.
	2. Rebuild the desired configuration (Debug/Release) to generate the interface library.
	   Note that the output file names will differ depending on the configuration you choose:
	   - 'brotlidll.lib' and 'brotlidll.dll' for the 'Release' configuration
	   - 'brotlidll.d.lib' and 'brotlidll.d.dll' for the 'Debug' configuration
	   To maximize compatibility across Windows platforms, the default target architecture 
	   is set to x86. You can easily add an x64 configuration if needed.
	3. The library and DLL output files are placed into the 'Debug' or 'Release' subdirectories 
	   of 'L:\brotlidll'. Make sure to copy them into a directory within your system PATH 
	   to make them globally accessible.

	Step 4: Testing the DLL and the Brotli interface
	-------------------------------------------------
	In the 'L:\brotlidll' directory there is a sample program you can use to check the DLL and the 
	Brotli interface, is the 'sample.c' file. To build it, open a "Developer command Prompt for VS"
	and type the following command: cl sample.c /Iinclude /link L:\Lib\brotlidll.lib

	DLL project configuration details:
	- The DLL project includes 'brotliapi.c'. This is a modified version of the original standalone
	  'brotli.c' utility, renamed and tailored to serve as the internal wrapper logic while keeping
	  code modifications minimal.
	- The DLL project EXCLUDES the original 'static_init.h', 'encode.c', and 'decode.c' files, replacing
	  them with the renamed duplicates listed in Step 2. This avoids build collisions while leaving the
	  original brotli project structure intact.
	- The following macros MUST be defined at the project level (Project properties, C/C++, Preprocessor):
	  BROTLI_SHARED_COMPILATION
	  BROTLICOMMON_SHARED_COMPILATION
	  BROTLIDEC_SHARED_COMPILATION
	  BROTLIENC_SHARED_COMPILATION
	  Without these macros, the compiler will treat the sources as a static library, and the linker will
	  fail to export the DLL entry points (__declspec(dllexport)).
	- The BROTLIDLL_EXPORTS macro, used to controls the DLL symbols visibility, must be defined only in 
	  the DLL project, NOT by the program using the DLL.
	- The 'brotlidll.h' header handles symbol visibility, import references (__declspec(dllimport)) and
	  library references (#pragma comment(lib,"brotlidll.lib")) for client applications linking against
	  this DLL.
	- The two function exposed by the DLL currently only handle baseline compression and decompression,
	  there is no advanced error handling, etc.

	Life is short, have fun.

	Luca P.
*/
#include <windows.h>
#include "brotlidll.h"

/*
	reuses the original 'brotli.c' source code (now in the 'brotliapi.c' file) by repurposing 'main()' into
	'ex_main()', allowing compression and decompression calls to be executed in the exact same manner as the
	standalone command-line utility
*/
int ex_main(int argc,char** argv);

/*
	DllMain()

	The DLL entry point, nothing of interest here.
*/
BOOL APIENTRY DllMain(HMODULE hModule,DWORD dwCallReason,LPVOID lpReserved)
{
	hModule;
	lpReserved;

	switch(dwCallReason)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		break;
	}

	return(TRUE);
}

/*
	original brotli.c command-line utility syntax (now via the 'ex_main()' call):
	compress 'test.exe' to 'test.out':   brotli -o test.out test.exe
	decompress 'test.out' to 'test.exe': brotli -d -o test.exe test.out
*/

/*
	BrotliCompressFile()
*/
BROTLIDLL_API BOOL BrotliCompressFile(LPCSTR lpcszInputFile,LPCSTR lpcszOutputFile)
{
	DeleteFile(lpcszOutputFile);

	int argc = 4;
	const char* argv[4] = {"brotlidll","-o",lpcszOutputFile,lpcszInputFile};

	return(ex_main(argc,argv));
}


/*
	BrotliUncompressFile()
*/
BROTLIDLL_API BOOL BrotliUncompressFile(LPCSTR lpcszInputFile,LPCSTR lpcszOutputFile)
{
	DeleteFile(lpcszOutputFile);

	int argc = 5;
	const char* argv[5] = {"brotlidll","-d","-o",lpcszOutputFile,lpcszInputFile};

	return(ex_main(argc,argv));
}
