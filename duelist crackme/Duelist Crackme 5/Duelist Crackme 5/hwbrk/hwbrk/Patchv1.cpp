/*Patch Class for Duelist Crackme #5
 *Author: afman
 *Last Updated: 8/15/2011
 *Version: 1.01
 *The patch class contains the data for an individual patch
 *vector. This vector overwrites an existing piece of data
 *in a specified section of memory with client specified
 *data.
 */

#include "Patch.h"
#include "HwBrk.h"
#include <stdio.h>
#include <string>

//HANDLE GetMainThread( DWORD dwOwnerPID );
//HANDLE hardwareBreakpoint, threadHandle;
HANDLE threadHandle;

//Define the constructors for the patch class.
Patch::Patch() {orig=address=patch=0;}
Patch::Patch(DWORD dw, BYTE bt) {orig=0, address=dw; patch=bt;}

LONG __stdcall BreakpointHandler(struct _EXCEPTION_POINTERS *pExceptionInfo) {

	MessageBox(
		NULL,
		(LPCWSTR)L"BreakpointHandler called...",
		(LPCWSTR)L"It's working...",
		MB_OK | MB_ICONEXCLAMATION
	);
	
	//Causes the compiler not to throw a warning. The compiler ends up optimizing this
	//out since it doesn't really do anything.
	UNREFERENCED_PARAMETER(pExceptionInfo);

	//Get the exception code from the EXCEPTION_POINTERS structure.
	LONG exceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;

	//Check if our breakpoint was hit.
	if (exceptionCode == STATUS_SINGLE_STEP) {

		MessageBox(
			NULL,
			(LPCWSTR)L"Single step exception thrown...",
			(LPCWSTR)L"It's working...",
			MB_OK | MB_ICONEXCLAMATION
		);
	
		/*FILE * pFile = fopen ("C:\\Users\\afman\\Documents\\status.txt","w+");
		if (pFile!=NULL)
		{
			fprintf(pFile, "ready");
			std::string status;
			MessageBox(
				NULL,
				(LPCWSTR)L"Waiting for breakpoint removal...",
				(LPCWSTR)L"It's working...",
				MB_OK | MB_ICONEXCLAMATION
			);
			while(TRUE) {
				fscanf(pFile,"%s",&status);
				if (status.compare("standby") == 0) {
					MessageBox(
						NULL,
						(LPCWSTR)L"Breakpoint removed...",
						(LPCWSTR)L"It's working...",
						MB_OK | MB_ICONEXCLAMATION
					);
					fclose(pFile);
					break;
				}
			}
		} else
			exit(0);

		/*DWORD hardwareBreakpoint;
		FILE * pFile = fopen ("C:\\Users\\afman\\Documents\\temp3.txt","r");
		if (pFile!=NULL)
		{
			fscanf(pFile,"%u",&hardwareBreakpoint);
			fclose(pFile);
		} else
			exit(0);

		MessageBox(
			NULL,
			(LPCWSTR)L"Removing hardware breakpoint...",
			(LPCWSTR)L"It's working...",
			MB_OK | MB_ICONEXCLAMATION
		);

		RemoveHardwareBreakpoint((PVOID)hardwareBreakpoint);
		*/
		//SuspendThread(threadHandle);

		PatchProgram(GetCurrentProcess(), GetCurrentThread());

		RemoveVectoredExceptionHandler(BreakpointHandler);

		//ResumeThread(threadHandle);

		CloseHandle(threadHandle);

		MessageBox(
			NULL,
			(LPCWSTR)L"Execution Continuing...",
			(LPCWSTR)L"It's working...",
			MB_OK | MB_ICONEXCLAMATION
		);


		//Resume execution of the program.
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	//Since this wasn't our breakpoint call the next VEH or go to the SEH chain.
	return EXCEPTION_CONTINUE_SEARCH;
}

void SetHandler() {
	MessageBox(
		NULL,
		(LPCWSTR)L"Registering VEH...",
		(LPCWSTR)L"It's working...",
		MB_OK | MB_ICONEXCLAMATION
	);

	/*DWORD input;
	FILE * pFile = fopen ("C:\\Users\\afman\\Documents\\temp.txt","r");
	if (pFile!=NULL)
	{

		fscanf(pFile,"%u",&input);
		fclose(pFile);

		FILE * pFile2 = fopen ("C:\\Users\\afman\\Documents\\temp2.txt","w");
		if (pFile2!=NULL)
		{
			fprintf(pFile2, "%u", input);
			fclose(pFile2);
		} else
			exit(0);

	} else
		exit(0);

	threadHandle = OpenThread(
		THREAD_ALL_ACCESS,
		NULL,
		input
	);

	if(threadHandle == NULL)
		exit(0);

	//The address to place the breakpoint at.
	void* ptr = (void *)0x004010C1;

	MessageBox(NULL, (LPCWSTR)L"Setting Harware Breakpoint.", (LPCWSTR)L"It's working...", MB_OK | MB_ICONEXCLAMATION);

	//Set the hardware breakpoint.
	hardwareBreakpoint = SetHardwareBreakpoint(threadHandle, HWBRK_TYPE_EXECUTE,HWBRK_SIZE_1, ptr);*/

	//Add the vectored exception handler to the front of the VEH linked list.
	AddVectoredExceptionHandler(1, BreakpointHandler);

	MessageBox(
		NULL,
		(LPCWSTR)L"Handler Registered",
		(LPCWSTR)L"It's working...",
		MB_OK | MB_ICONEXCLAMATION
	);

}

BOOL PatchProgram(HANDLE hProcess, HANDLE hThread) {

	//This is the number of patches that will occur to the program.
	const int patchNum = 23;

	//These are the patches to be applied to the program.
	Patch crk[patchNum];
	crk[0] = Patch(0x004010C1, 0x90);
	crk[1] = Patch(0x004010C2, 0x90);
	crk[2] = Patch(0x004010C3, 0x90);
	crk[3] = Patch(0x004010C4, 0x90);
	crk[4] = Patch(0x004010C5, 0x90);
	crk[5] = Patch(0x004010C6, 0x90);
	crk[6] = Patch(0x004010C7, 0x90);
	crk[7] = Patch(0x004010C8, 0x90);
	crk[8] = Patch(0x004010C9, 0x90);
	crk[9] = Patch(0x004010CA, 0x90);
	crk[10] = Patch(0x004010CB, 0x90);
	crk[11] = Patch(0x004010CC, 0x90);
	crk[12] = Patch(0x004010CD, 0x90);
	crk[13] = Patch(0x004010CE, 0x90);
	crk[14] = Patch(0x004010CF, 0x90);
	crk[15] = Patch(0x004010D0, 0x90);
	crk[16] = Patch(0x004010D1, 0x90);
	crk[17] = Patch(0x004010D2, 0x90);
	crk[18] = Patch(0x004010D3, 0x90);
	crk[19] = Patch(0x004010D4, 0x90);
	crk[20] = Patch(0x004010D5, 0x90);
	crk[21] = Patch(0x004010D6, 0x90);
	crk[22] = Patch(0x00401131, 0x50);

	//Find the specified memory locations and patch them.
	unsigned long byteswritten[patchNum];
	unsigned long bytesread[patchNum];

	//Zero out the arrays
	for (int i=0; i<patchNum; i++) {
		bytesread[i] = 0;
		byteswritten[i] = 0;
	}

	for (int idx=0; idx<patchNum; idx++) {
		//Read in the data to be patched
		ReadProcessMemory(hProcess,
			(LPVOID)(crk[idx].address),
			(LPVOID)(&(crk[idx].orig)),
			1,
			&bytesread[idx]);
		//Handle any errors that occured
		if(bytesread[idx] == 0)
			return FALSE;

		//Write the patched data into the process
		WriteProcessMemory(hProcess,
			(LPVOID)(crk[idx].address),
			(LPVOID)(&crk[idx].patch),
			1,
			&byteswritten[idx]);
		//Handle any errors that occured
		if(bytesread[idx] == 0)
			return FALSE;
	}

	MessageBox(
		NULL,
		(LPCWSTR)L"Patching Completed!",
		(LPCWSTR)L"Grant is awesome!",
		MB_OK | MB_ICONEXCLAMATION
	);

	//Patching is complete so return true.
	return TRUE;

}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{

    //If the DLL has just loaded...
    if(fdwReason == DLL_PROCESS_ATTACH) 
    { 
		MessageBox(
			NULL,
			(LPCWSTR)L"SetHandler called...",
			(LPCWSTR)L"It's working...",
			MB_OK | MB_ICONEXCLAMATION
		);
		SetHandler();
    }

    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}