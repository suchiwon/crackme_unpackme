/*Patch Class for Duelist Crackme #5
 *Author: afman
 *Last Updated: 8/15/2011
 *Version: 1.00
 *The patch class contains the data for an individual patch
 *vector. This vector overwrites an existing piece of data
 *in a specified section of memory with client specified
 *data.
 */

#include <Windows.h>

class Patch {

public:
	Patch();
	Patch(DWORD dw, BYTE bt);
	DWORD address;
	BYTE patch;
	BYTE orig;

};

BOOL PatchProgram(HANDLE hProcess, HANDLE hThread);

__declspec(dllexport) void SetHandler(int address, HANDLE hProcess);