// Hardware Breakpoint Functions

#define _WIN32_WINNT _WIN32_WINNT_WINXP
#include <windows.h>
#include "HwBrk.h"

class HWBRK
	{
	public:
		void* a;
		HANDLE hT;
		HWBRK_TYPE Type;
		HWBRK_SIZE Size;
		HANDLE hEv;
		int iReg;
		int Opr;
		bool SUCC;

		HWBRK()
			{
			Opr = 0;		//If 0 then debug registers are
							//being set and if 1 they are being
							//cleared.
			a = 0;			//Address
			hT = 0;			//Handle to a thread
			hEv = 0;		//Handle to event
			iReg = 0;
			SUCC = false;	//Success or failure
			}
	};


void SetBits(DWORD_PTR& dw, int lowBit, int bits, int newValue)
	{
	DWORD_PTR mask = (1 << bits) - 1; 
	dw = (dw & ~(mask << lowBit)) | (newValue << lowBit);
	}

static DWORD WINAPI th(LPVOID lpParameter)
	{

	//lpParameter is passed via the CreateThread parameter so it has
	//to be typecasted back to a HWBRK type
	HWBRK* h = (HWBRK*)lpParameter;
	int j = 0;
	int y = 0;

	//Suspend the target thread so the breakpoint can be set. As a
	//reminder SuspendThread returns a threads SuspendCount
	j = SuspendThread(h->hT);
    y = GetLastError();

	CONTEXT ct = {0};

	//Get the context of the DRX register set.
	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	//If GetThreadContext succeeeds it returns a non-zero number
	//otherwise it returns 0
	j = GetThreadContext(h->hT,&ct);
    y = GetLastError();

	int FlagBit = 0;

	//Set all debug registers as unused
	bool Dr0Busy = false;
	bool Dr1Busy = false;
	bool Dr2Busy = false;
	bool Dr3Busy = false;

	//Each of these correspond to the Global
	//breakpoint enable registers.
	if (ct.Dr7 & 1)
		Dr0Busy = true;
	if (ct.Dr7 & 4)
		Dr1Busy = true;
	if (ct.Dr7 & 16)
		Dr2Busy = true;
	if (ct.Dr7 & 64)
		Dr3Busy = true;

	//Opr set to 1 means remove breakpoints
	if (h->Opr == 1)
		{
		// Clear debug registers
		if (h->iReg == 0)
			{
			FlagBit = 0;
			ct.Dr0 = 0;
			Dr0Busy = false;
			}
		if (h->iReg == 1)
			{
			FlagBit = 2;
			ct.Dr1 = 0;
			Dr1Busy = false;
			}
		if (h->iReg == 2)
			{
			FlagBit = 4;
			ct.Dr2 = 0;
			Dr2Busy = false;
			}
		if (h->iReg == 3)
			{
			FlagBit = 6;
			ct.Dr3 = 0;
			Dr3Busy = false;
			}

		ct.Dr7 &= ~(1 << FlagBit);
		}
	//If opr was 0 then the below sets the first available
	//debug register and marks it as used
	else
		{
		if (!Dr0Busy)
			{
			h->iReg = 0;
			ct.Dr0 = (DWORD_PTR)h->a;
			Dr0Busy = true;
			}
		else
		if (!Dr1Busy)
			{
			h->iReg = 1;
			ct.Dr1 = (DWORD_PTR)h->a;
			Dr1Busy = true;
			}
		else
		if (!Dr2Busy)
			{
			h->iReg = 2;
			ct.Dr2 = (DWORD_PTR)h->a;
			Dr2Busy = true;
			}
		else
		if (!Dr3Busy)
			{
			h->iReg = 3;
			ct.Dr3 = (DWORD_PTR)h->a;
			Dr3Busy = true;
			}
		else
			{
			h->SUCC = false;
			j = ResumeThread(h->hT);
			y = GetLastError();
			SetEvent(h->hEv);
			return 0;
			}
		ct.Dr6 = 0;
		int st = 0;
		if (h->Type == HWBRK_TYPE_EXECUTE)
			st = 0;
		if (h->Type == HWBRK_TYPE_READWRITE)
			st = 3;
		if (h->Type == HWBRK_TYPE_WRITE)
			st = 1;
		int le = 0;
		if (h->Size == HWBRK_SIZE_1)
			le = 0;
		if (h->Size == HWBRK_SIZE_2)
			le = 1;
		if (h->Size == HWBRK_SIZE_4)
			le = 3;
		if (h->Size == HWBRK_SIZE_8)
			le = 2;


		SetBits(ct.Dr7, 16 + h->iReg*4, 2, st);
		SetBits(ct.Dr7, 18 + h->iReg*4, 2, le);
		SetBits(ct.Dr7, h->iReg*2,1,1);
		}



	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	j = SetThreadContext(h->hT,&ct);
    y = GetLastError();

	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	j = GetThreadContext(h->hT,&ct);
    y = GetLastError();

	j = ResumeThread(h->hT);
    y = GetLastError();

	h->SUCC = true;

	SetEvent(h->hEv);
	return 0;
	}

HANDLE SetHardwareBreakpoint(HANDLE hThread,HWBRK_TYPE Type,HWBRK_SIZE Size,void* s)
	{

	//Set constants
	HWBRK* h = new HWBRK; //Create a new hardware breakpoint
	h->a = s;			  //Address of the breakpoint
	h->Size = Size;		  //Size of the breakpoint to be set
	h->Type = Type;		  //Type of breakpoint to set (read/write/execute)
	h->hT = hThread;	  //The thread to set the breakpoint in

	h->hEv = CreateEvent(0,0,0,0);
	h->Opr = 0; // 0 specifies set breakpoint 1 specifies remove breakpoint

	//Create a remote thread to set the hardware breakpoints. Hardware breakpoints
	//are not global in windows. They are specific to the context of one thread.
	HANDLE hY = CreateThread(0,0,th,(LPVOID)h,0,0);

	//Wait for the breakpoint instance's event object to be placed in a signaled state.
	WaitForSingleObject(h->hEv,INFINITE);
	CloseHandle(h->hEv);
	h->hEv = 0;

	if (hThread == GetCurrentThread())
		{
		CloseHandle(h->hT);
		}
	h->hT = hThread;

	if (!h->SUCC)
		{
		delete h;
		return 0;
		}


	return (HANDLE)h;
	}


/*bool RemoveHardwareBreakpoint(HANDLE hBrk)
	{
	HWBRK* h = (HWBRK*)hBrk;
	if (!h)
		return false;

	bool C = false;
	if (h->hT == GetCurrentThread())
		{
		DWORD pid = GetCurrentThreadId();
		h->hT = OpenThread(THREAD_ALL_ACCESS,0,pid);
		C = true;
		}

	h->hEv = CreateEvent(0,0,0,0);
	h->Opr = 1; // Remove Break
	HANDLE hY = CreateThread(0,0,th,(LPVOID)h,0,0);
	WaitForSingleObject(h->hEv,INFINITE);
	CloseHandle(h->hEv);
	h->hEv = 0;

	if (C)
		{
		CloseHandle(h->hT);
		}

	delete h;
	return true;
	}*/

bool RemoveHardwareBreakpoint(HANDLE hBrk)
	{
	HWBRK* h = (HWBRK*)hBrk;
	if (!h)
		return false;

	bool C = false;

	//Suspend the target thread so the breakpoint can be set. As a
	//reminder SuspendThread returns a threads SuspendCount
	SuspendThread(h->hT);

	CONTEXT ct = {0};

	//Get the context of the control register set.
	ct.ContextFlags = CONTEXT_CONTROL;
	//ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	//Get the context of the thread.
	GetThreadContext(h->hT,&ct);

	ct.Eip = 0x004010D4;

	SetThreadContext(h->hT,&ct);

	ResumeThread(h->hT);

	//ct.Dr0 = 0;
	//ct.Dr1 = 0;
	//ct.Dr2 = 0;
	//ct.Dr3 = 0;

	delete h;
	return true;
	}
