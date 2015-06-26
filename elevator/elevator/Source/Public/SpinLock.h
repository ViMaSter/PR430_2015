#pragma once

#include "export.h"
#include "Windows.h"

class ELEVATOR_API SpinLock
{
public:
	long IsLocked;

	SpinLock();
	~SpinLock();

	bool Lock();
	bool Unlock();
};
