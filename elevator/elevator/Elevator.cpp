#include "stdafx.h"

#include "Elevator.h"

// Static methods
DWORD WINAPI Elevator::ThreadWork(LPVOID elevatorParameter)
{
	ElevatorParameter* parameter = (ElevatorParameter*)elevatorParameter;

	printf("Elevator thread ID: %05d running on PID:%i", GetCurrentThreadId(), GetCurrentProcessorNumber());

	return 0;
}

// Instance methods
Elevator::Elevator(const ElevatorParameter& rhs) : Parameter(rhs)
{
	CreateThread(NULL, 0, Elevator::ThreadWork, (LPVOID)&Parameter, 0, (LPDWORD)&AssociatedThreadID);
}

Elevator::Elevator()
{
}

Elevator::~Elevator()
{
}

int Elevator::SetFloor(int floor)
{
	Floor = floor;
	return 0;
}

const int Elevator::GetFloor() const
{
	return Floor;
}
