/*Main for Duelist Crackme #5
 *Author: afman
 *Last Updated: 8/15/2011
 *Version: 3.00
 *This patcher is designed to load, execute, and patch the
 *executable file due-cm5.exe associated with Duelist Crackme #5.
 *Patcher works in the following manner:.
 */

#include <Windows.h>
#include <stdio.h>
#include "HwBrk.h"
#include <string>

void InjectDLL(HANDLE Proc);

BOOL main(int argc, char* argv[]) {
	
	PROCESS_INFORMATION process_info;
	STARTUPINFO startup_info;

	//0 out the area for our process' structures
    ZeroMemory( &startup_info, sizeof(startup_info) );
    startup_info.cb = sizeof(startup_info);
    ZeroMemory( &process_info, sizeof(process_info) );

	//Grab the process to be loaded. (In this case that should be
	//the duelist 5 executable file due-cm5.exe
	if( !::CreateProcess(
		"due5.exe",		//LPCTSTR lpApplicationName
							//Specify path/name on command line
		NULL,				//LPTSTR lpCommandLine
							//Returns a pointer to the command line
		NULL,				//LPSECURITY_ATTRIBUTES lpProcessAttributes
							//Process handle not inheritable
		NULL,				//LPSECURITY_ATTRIBUTES lpThreadAttributes
							//Thread handle not inheritable
		FALSE,				//BOOL bInheritHandles
							//Set handle inheritance to FALSE
		CREATE_SUSPENDED,	//DWORD dwCreationFlags
							//Use suspended creation flags
		NULL,				//LPVOID lpEnvironment
							//Use parent's environment block
		NULL,				//LPCTSTR lpCurrentDirectory
							//Use parent's starting directory
		&startup_info,		//LPSTARTUPINFO lpStartupInfo
							//Pointer to STARTUPINFO structure
		&process_info)		//LPPROCESS_INFORMATION lpProcessInformation
							//Pointer to PROCESS_INFORMATION struture.
		) { //If the process can't be created return an error message.
			printf( "CreateProcess failed (%d).\n", GetLastError() );
			return FALSE;
	}
	
	//The address to place the breakpoint at.
	void* ptr = (void *)0x004010C1;

	printf("Setting hardware breakpoint...\n");

	//Set the hardware breakpoint.
	HANDLE hardwareBreakpoint = SetHardwareBreakpoint(process_info.hThread, HWBRK_TYPE_EXECUTE,HWBRK_SIZE_1, ptr);

	/*FILE * pFile = fopen ("C:\\Users\\afman\\Documents\\status.txt","w+");
	if (pFile!=NULL)
	{
		fprintf(pFile, "standby");
		fclose(pFile);
	} else
		exit(0);

	/*FILE * pFile2 = fopen ("C:\\Users\\afman\\Documents\\temp.txt","w");
	if (pFile2!=NULL)
	{
		fprintf(pFile2, "%u", GetThreadId(process_info.hThread));
		fclose(pFile2);
	} else
		exit(0);*/

	printf("Starting injection process...\n");

	//Inject the DLL into the remote thread.
	InjectDLL(process_info.hProcess);

	printf("Resuming thread...\n");

	ResumeThread(process_info.hThread);

	printf("Waiting to remove hardware breakpoint...");

	/*FILE * pFile = fopen ("C:\\Users\\afman\\Documents\\status.txt","r");
	std::string status;
	while(TRUE) {
		fscanf(pFile,"%s",&status);
		if (status.compare("ready") == 0) {
			printf("Removing hardware breakpoint...");
			RemoveHardwareBreakpoint(hardwareBreakpoint);
			fprintf(pFile, "standby");
			fclose(pFile);
			break;
		}
	}*/

	printf("Closing handle to process...");

	CloseHandle(process_info.hProcess);

	return TRUE;

}

void InjectDLL(HANDLE Proc)
{
   //Declare constants.
   char buf[50]={0};
   LPVOID RemoteString, LoadLibAddy, SetHandler;
   LPCSTR DLL_NAME = "HwBrkDLL.dll";

   printf("Getting LoadLibrary handle...\n");

   //Get the address of the LoadLibrary function in kernel32.dll
   LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

   printf("Creating remote string with the name of the DLL to be injected...\n");

   //Allocate space in the remote process to store the name of the library to be loaded.
   RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME) + 1, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

   printf("Writing string to remote memory...\n");

   //Write the name of the DLL into the allocated memory.
   WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME,strlen(DLL_NAME) + 1, NULL);

   printf("Creating remote thread to load DLL...\n");

   //Create a remote thread in the victim process that loads the library.
   WaitForSingleObject(
	   CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL),
	   INFINITE);

   DWORD LastError = GetLastError();

   VirtualFreeEx(
       Proc,
       RemoteString,
       strlen(DLL_NAME) + 1,
       MEM_RELEASE);

}