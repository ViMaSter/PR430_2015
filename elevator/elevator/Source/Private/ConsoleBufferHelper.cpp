#include "../Public/stdafx.h"

#include "isExport.h"

#include "ConsoleBufferHelper.h"

std::mutex* ConsoleBufferHelper::ConsoleMutex;

void ConsoleBufferHelper::Initialize() {
	if (ConsoleBufferHelper::ConsoleMutex == nullptr) {
		ConsoleBufferHelper::ConsoleMutex = new std::mutex();
	}
}

std::mutex* ConsoleBufferHelper::GetMutex() {
	Initialize();
	return ConsoleBufferHelper::ConsoleMutex;
}

void ConsoleBufferHelper::JumpToPosition(COORD coord) {
	while (!ConsoleBufferHelper::GetMutex()->try_lock()) {
	}
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(consoleHandle, coord);
}
