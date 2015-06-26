#pragma once

#include "export.h"

#include "windows.h"
#include <string>
#include <iostream>
#include <mutex>

class ELEVATOR_API ConsoleBufferHelper {
private:
	static std::mutex* ConsoleMutex;
public:
	static std::mutex* GetMutex();
	static void Initialize();

	// Make sure to ConsoleBufferHelper::ConsoleMutex.unlock() after using this
	static void ConsoleBufferHelper::JumpToPosition(COORD coord);
};
