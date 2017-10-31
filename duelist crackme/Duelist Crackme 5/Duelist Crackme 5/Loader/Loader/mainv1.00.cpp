/*Main for Duelist Crackme #5
 *Author: afman
 *Last Updated: 8/15/2011
 *Version: 1.00
 *This patcher is designed to load, execute, and patch the
 *executable file due-cm5.exe associated with Duelist Crackme #5.
 *Patcher works in the following manner:
 * 1. Declare a sequence of patches to overwrite the data
 *    inside of the original exe.
 * 2. Use the loader to call due-cm5.exe in a suspended
 *	  state. This allows the loader to modify the 
 *	  due-cm5.exe process while it resides in memory.
 * 3. Resume the thread of the suspended due-cm5.exe process.
 * 4. Search for a handle to the victim's window
 * 5. Once the handle is found search for and replace each
 *    target memory location.
 */

#include <Windows.h>
#include "Patch.h"

void GetLastErrorMsg(char *szBuf);
BOOL CALLBACK EnumWindowsProc;

int main(int argc, char* argv[]) {
	
	//This is the number of patches that will occur to the program.
	const int patchNum = 4;

	//Patch crk[Number of patches here];
	Patch crk[patchNum];
	crk[0] = Patch(0, 0);

	//Becomes a handle to the victim process' dialog window.
	HWND VictimDlghWnd = NULL;
	
	/*
		typedef struct _PROCESS_INFORMATION {
		HANDLE hProcess;
		HANDLE hThread;
		DWORD  dwProcessId;
		DWORD  dwThreadId;
		} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;
	*/
	PROCESS_INFORMATION process_info;

	/*
		typedef struct _STARTUPINFO {
		DWORD  cb;
		LPTSTR lpReserved;
		LPTSTR lpDesktop;
		LPTSTR lpTitle;
		DWORD  dwX;
		DWORD  dwY;
		DWORD  dwXSize;
		DWORD  dwYSize;
		DWORD  dwXCountChars;
		DWORD  dwYCountChars;
		DWORD  dwFillAttribute;
		DWORD  dwFlags;
		WORD   wShowWindow;
		WORD   cbReserved2;
		LPBYTE lpReserved2;
		HANDLE hStdInput;
		HANDLE hStdOutput;
		HANDLE hStdError;
		} STARTUPINFO, *LPSTARTUPINFO;
	*/
	STARTUPINFO startup_info;

	//0 out the area for our process' structures
	memset(&process_info, 0, sizeof(PROCESS_INFORMATION));
	memset(&startup_info, 0, sizeof(STARTUPINFO));

	//Set the cb (size of the structure in bytes)
	startup_info.cb = sizeof(startup_info);

	//Grab the process to be loaded. (In this case that should be
	//the duelist 5 executable file due-cm5.exe
	if( !::CreateProcess(
		NULL,				//LPCTSTR lpApplicationName
							//Specify path/name on command line
		GetCommandLine(),	//LPTSTR lpCommandLine
							//Returns a pointer to the command line
		NULL,				//LPSECURITY_ATTRIBUTES lpProcessAttributes
							//Process handle not inheritable
		NULL,				//LPSECURITY_ATTRIBUTES lpThreadAttributes
							//Thread handle not inheritable
		NULL,				//BOOL bInheritHandles
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
			char errorBuf[80];
			GetLastErrorMsg(errorBuf);
			MessageBox(NULL, errorBuf, "Error", MB_OK);
			return 1;
	}

	ResumeThread(process_info.hThread);

	//Until we find the victim's window...
	while(VictimDlghWnd == NULL) {

		//Grab the next window's handle.
		EnumDesktopWindows(NULL, EnumWindowsProc, (LPARAM)(&VictimDlghWnd));

		//If this is true then we found the victim's window.
		if(VictimDlghWnd != NULL) {

			//Tell the user we found the victim window
			::MessageBox(NULL, "Victim's Window Found", "Victim Found", MB_OK);

			//Create a handle to the process called hProcess
			HANDLE hProcess = process_info.hProcess;

			//Suspend the thread again
			SuspendThread(process_info.hThread);

			//Find the specified memory locations and patch them.
			unsigned long byteswritten[patchNum];
			unsigned long bytesread[patchNum];
			char errors[patchNum][5]; //NEED TO SPECIFY A Y VALUE

			//Zero out the arrays
			for (int i=0; i<patchNum; i++) {
				bytesread[i] = 0;
				byteswritten[i] = 0;
				strcpy(errors[i], "");
			}


			for (int idx=0; idx<patchNum; idx++) { //NEED TO REPLACE THE X
				//Read in the data to be patched
				ReadProcessMemory(hProcess,
					(LPVOID)(crk[idx].address),
					(LPVOID)(&(crk[idx].orig)),
					1,
					&bytesread[idx]);
				//Handle any errors that occured
				if(bytesread[idx] == 0)
					GetLastErrorMsg(errors[idx]);
				else
					strcpy(errors[idx], "OK");

				//Write the patched data into the process
				WriteProcessMemory(hProcess,
					(LPVOID)(crk[idx].address),
					(LPVOID)(&crk[idx].patch),
					1,
					&byteswritten[idx]);
				//Handle any errors that occured
				if(bytesread[idx] == 0)
					GetLastErrorMsg(errors[idx]);
				else
					strcpy(errors[idx], "OK");
			}

			ResumeThread(process_info.hThread);

			break;
		}
	}

	return 0;

}

void GetLastErrorMsg(char *szBuf)
{
	TCHAR szBuf[80];
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );
	wsprintf(szBuf, "Loader failed with error %d: %s", dw, lpMsgBuf);
	LocalFree(lpMsgBuf);
}

BOOL CALLBACK EnumWindowsProc(
	HWND hWnd, // handle to parent window
	LPARAM lParam // application-defined value
) {
	char ClassName[256];

	GetClassName(hWnd,ClassName, 256);
	char caption[256];
	GetWindowText(hWnd, caption,256);
	if(strstr(caption,"Main Target Window Caption")!=0 && _stricmp(ClassName,"TMainForm")==0)
	{
		HWND *hw=(HWND*)lParam;
		*hw=hWnd;
		return FALSE;
	}
	return TRUE;
}