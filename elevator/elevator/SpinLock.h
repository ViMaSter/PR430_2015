#pragma once

#include "Windows.h"

class SpinLock
{
public:
	long IsLocked;

	SpinLock();
	~SpinLock();

	bool Lock();
	bool Unlock();
};
