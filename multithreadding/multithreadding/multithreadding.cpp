// multithreadding.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "windows.h"

#include <vector>

DWORD WINAPI NullThread(LPVOID ptrThreadIsDone)
{
	int* threadCount = (int*)ptrThreadIsDone;

	printf("Processor %i (ID: 0x%0x): Threads running atm: %i\r\n", GetCurrentProcessorNumber(), GetCurrentThreadId(), *threadCount);

	(*threadCount)--;
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	SECURITY_ATTRIBUTES SecurityAttributes = SECURITY_ATTRIBUTES();

	int StackSize = 2048;
	printf("Begin\r\n");

	int ThreadCount = 0;
	int ThreadID = 0;

	HANDLE h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &ThreadCount, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;


	while (ThreadCount != 0)
	{
		Sleep(10);
	}

	printf("End\r\n");
	return 0;
}

