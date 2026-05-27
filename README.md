# Brotli Windows DLL Wrapper

This is a native C project for Windows environments that implements a DLL interface wrapper around [Google Brotli](https://github.com/google/brotli), the open-source data compression library, widely used in client/server HTTP connections.

## The Problem
The official Brotli source distribution is heavily tailored around Unix/CMake build behaviors. Because of this architectural mismatch, the raw source code cannot be compiled, linked, or used directly within a standard Visual Studio project. It suffers from object file naming collisions and lacks the necessary preprocessor definitions required for Windows symbol exporting.

## The Solution (Pre-Patched Package Included)
To bypass the tedious manual task of duplicating, renaming, and modifying the official source files (which is explained in detail inside the comments of `brotlidll.c`), this repository includes a **`brotli.1.2.0.zip`** archive inside the main project directory (`brotlidll`). 

This zip contains the official Brotli version 1.2.0 distribution, **already modified, patched, and ready to be compiled out-of-the-box**.

## Directory Structure & Virtual Drive (L:) Setup
The Visual Studio project is configured to look for source files using a virtual drive **`L:`** as the root working path. 

To use the default configuration:
1. The DLL project files must reside in: `L:\brotlidll`
2. Extract the provided `brotli.1.2.0.zip` (or your manually patched official package) into: `L:\brotli`

### Customizing the Path
If you prefer not to use the `L:` drive, you must manually update the Additional Include Directories and Output Paths within the Visual Studio project properties. 

Alternatively, you can quickly map a local folder to a virtual `L:` drive by opening your Command Prompt and running:
```text
SUBST L: C:\<YOUR_PATH_HERE>
```

## The Interface

### Testing the DLL
Inside the `brotlidll` project directory, you will find a **`sample.c`** file, a very basic C program to test the DLL and demonstrate how to interface with the exported Brotli functions.

### Exposed Interface
The DLL exposes the following high-level C functions. Note that at the moment there is no advanced error handling, etc., just a boolean result:

```text
// Compresses a file into .br format
BROTLIDLL_API BOOL BrotliCompressFile(LPCSTR lpcszInputFile,LPCSTR lpcszOutputFile);

// Decompresses a .br file back to its original state
BROTLIDLL_API BOOL BrotliUncompressFile(LPCSTR lpcszInputFile,LPCSTR lpcszOutputFile);
```
Luca P.
