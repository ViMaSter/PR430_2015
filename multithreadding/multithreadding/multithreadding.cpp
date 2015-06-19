// multithreadding.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "windows.h"

#include <vector>

struct ParamDef {
	int* ThreadCount;
	std::vector<int>* IntVector;
};

DWORD WINAPI NullThread(LPVOID ptrThreadIsDone)
{
	// int* threadCount = (int*)ptrThreadIsDone;
	ParamDef* ptrParameter = (ParamDef*)ptrThreadIsDone;

	printf("Processor %i (ID: 0x%0x): Threads running atm: %i\r\n", GetCurrentProcessorNumber(), GetCurrentThreadId(), *(ptrParameter->ThreadCount));

	for (int i = 0; i < 100; i++) {
		(*(*ptrParameter).IntVector).push_back(i);
	}

	(*(*ptrParameter).ThreadCount)--;
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	SECURITY_ATTRIBUTES SecurityAttributes = SECURITY_ATTRIBUTES();

	int StackSize = 2048;
	printf("Begin\r\n");

	int ThreadCount = 0;
	int ThreadID = 0;

	std::vector<int> IntVector;

	ParamDef parameter;
	parameter.ThreadCount = &ThreadCount;
	parameter.IntVector = &IntVector;

	HANDLE h;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
	if (h == INVALID_HANDLE_VALUE) {
		return 1;
	}
	ThreadCount++;
	h = CreateThread((LPSECURITY_ATTRIBUTES)&SecurityAttributes, (SIZE_T)&StackSize, NullThread, &parameter, 0, (LPDWORD)&ThreadID);
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

