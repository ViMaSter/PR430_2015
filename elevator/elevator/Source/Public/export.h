#pragma once
#ifdef EXPORTING_ELEVATOR
#define ELEVATOR_API __declspec(dllexport)
#else
#define ELEVATOR_API __declspec(dllimport)
#endif