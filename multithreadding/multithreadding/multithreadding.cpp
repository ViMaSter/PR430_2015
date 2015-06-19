// multithreadding.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "windows.h"

#include "SpinLock.h"

#include <vector>

struct ParamDef {
	long* ThreadCount;
	std::vector<int>* IntVector;
	SpinLock* AssociatedSpinLock;
};


DWORD WINAPI NullThread(LPVOID ptrThreadIsDone)
{
	// int* threadCount = (int*)ptrThreadIsDone;
	ParamDef* ptrParameter = (ParamDef*)ptrThreadIsDone;

	printf("Processor %i (ID: 0x%0x): Threads running atm: %i\r\n", GetCurrentProcessorNumber(), GetCurrentThreadId(), *(ptrParameter->ThreadCount));

	for (int i = 0; i < 100; i++)
	{
		ptrParameter->AssociatedSpinLock->Lock();
		ptrParameter->IntVector->push_back(i);
		ptrParameter->AssociatedSpinLock->Unlock();
		Sleep(10);
	}

	InterlockedDecrement((long*)ptrParameter->ThreadCount);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	SpinLock IntVectorSpinLock = SpinLock();

	SECURITY_ATTRIBUTES SecurityAttributes = SECURITY_ATTRIBUTES();

	int StackSize = 2048;
	printf("Begin\r\n");

	long CurrentThreadCount = 0;

	std::vector<int> IntVector;

	ParamDef parameter;
	parameter.ThreadCount = &CurrentThreadCount;
	parameter.IntVector = &IntVector;
	parameter.AssociatedSpinLock = &IntVectorSpinLock;

	const int ThreadsToStart = 10;
	HANDLE h;
	int ThreadID;

	for (int i = 0; i < 10; i++)
	{
		h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
		if (h == INVALID_HANDLE_VALUE)
		{
			return 1;
		}
		InterlockedIncrement(&CurrentThreadCount);
	}

	while (CurrentThreadCount != 0)
	{
		Sleep(10);
	}

	printf("End\r\n");
	return 0;
}

