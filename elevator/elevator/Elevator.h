#pragma once

#include "windows.h"

struct ElevatorParameter
{
private:
	ElevatorParameter& operator=(ElevatorParameter& rhs)
	{
	}

public:
	const int StartFloor;
	ElevatorParameter() : StartFloor(0)
	{
	}
	ElevatorParameter(int startFloor) : StartFloor(startFloor)
	{
	}

	ElevatorParameter(const ElevatorParameter& rhs) : StartFloor(rhs.StartFloor)
	{

	}
};

class Elevator
{
private:
	Elevator();
	int Floor;

public:
	Elevator(const ElevatorParameter& rhs);
	~Elevator();

	ElevatorParameter Parameter;
	DWORD AssociatedThreadID;

	const int GetFloor() const;
	int SetFloor(int floor);
	static DWORD WINAPI ThreadWork(LPVOID parameter);
};