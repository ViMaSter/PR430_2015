#include "stdafx.h"
#include "SpinLock.h"

SpinLock::SpinLock()
{
	IsLocked = false;
}

SpinLock::~SpinLock()
{
}

bool SpinLock::Lock()
{
	while (InterlockedCompareExchange((long*)&IsLocked, true, false))
	{
		// Uncomment this - for scientific purposes
		Sleep(10);
	}
	return true;
}

bool SpinLock::Unlock()
{
	InterlockedCompareExchange((long*)&IsLocked, false, true);
	return true;
}
