#include "stdafx.h"

#include "Passenger.h"

// Static methods
DWORD WINAPI Passenger::ThreadWork(LPVOID passengerParameter)
{
	PassengerParameter* parameter = (PassengerParameter*)passengerParameter;

	printf("Passenger thread ID: %05d running on PID:%i", GetCurrentThreadId(), GetCurrentProcessorNumber());
	
	return 0;
}

// Instance methods
Passenger::Passenger(const PassengerParameter& rhs) : Parameter(rhs)
{
	CreateThread(NULL, 0, Passenger::ThreadWork, (LPVOID)&Parameter, 0, (LPDWORD)&AssociatedThreadID);
}

Passenger::Passenger()
{
}

Passenger::~Passenger()
{
}

int Passenger::SetFloor(int floor)
{
	Floor = floor;
	return 0;
}

const int Passenger::GetFloor() const
{
	return Floor;
}
