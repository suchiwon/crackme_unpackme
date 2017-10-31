/*Main for Duelist Crackme #5
 *Author: afman
 *Last Updated: 8/15/2011
 *Version: 2.00
 *This patcher is designed to load, execute, and patch the
 *executable file due-cm5.exe associated with Duelist Crackme #5.
 *Patcher works in the following manner:.
 */

#include <Windows.h>
#include <stdio.h>
#include "Patch.h"
#include "hwbrk.h"

int main(int argc, char* argv[]) {
	
	PROCESS_INFORMATION process_info;
	STARTUPINFO startup_info;

	//0 out the area for our process' structures
    ZeroMemory( &startup_info, sizeof(startup_info) );
    startup_info.cb = sizeof(startup_info);
    ZeroMemory( &process_info, sizeof(process_info) );

	//Grab the process to be loaded. (In this case that should be
	//the duelist 5 executable file due-cm5.exe
	if( !::CreateProcess(
		"C:\\due5.exe",		//LPCTSTR lpApplicationName
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
			return 1;
	}

	//Create a handle to the process called hProcess
	HANDLE hProcess = process_info.hProcess;

	//Actual pointer void* ptr = (void *)0x004010C1;
	void* ptr = (void *)0x004177CC;

	//Actual Call HANDLE hardwareBreakpoint = SetHardwareBreakpoint(hProcess, HWBRK_TYPE_READWRITE,HWBRK_SIZE_1, ptr);
	HANDLE hardwareBreakpoint = SetHardwareBreakpoint(GetCurrentThread(), HWBRK_TYPE_READWRITE,HWBRK_SIZE_1, ptr);

	__try {
		//Actual ResumeThread(process_info.hThread);
		printf("Gayness");
	} __except(GetExceptionCode() == STATUS_SINGLE_STEP) {

			//Suspend the thread again
			SuspendThread(process_info.hThread);

			printf("This is awesome.");

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
				if(bytesread[idx] == 0) {
					printf( "Reading from memory failed (%d).\n", GetLastError() );
					return 1;
				} else
					printf("Patch %d read.\n", idx);

				//Write the patched data into the process
				WriteProcessMemory(hProcess,
					(LPVOID)(crk[idx].address),
					(LPVOID)(&crk[idx].patch),
					1,
					&byteswritten[idx]);
				//Handle any errors that occured
				if(bytesread[idx] == 0) {
					printf( "Writing to memory failed (%d).\n", GetLastError() );
					return 1;
				} else
					printf("Patch %d written.\n", idx);
			}

			ResumeThread(process_info.hThread);

	}
	
	return 0;

}

BOOL InjectDLL(HANDLE Proc)
{
   char buf[50]={0};
   LPVOID RemoteString, LoadLibAddy;

   LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

   RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
   WriteProcessMemory(Proc, (LPVOID)RemoteString, DLL_NAME,strlen(DLL_NAME), NULL);
   CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);   
   
   CloseHandle(Proc);

   return true;
}